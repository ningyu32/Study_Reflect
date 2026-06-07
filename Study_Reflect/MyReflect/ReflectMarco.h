#pragma once
#include "ClassInfo.h"
#include "Property.h"
#include "Registry.h"

// 辅助宏：连接符号
#define REFLECT_CONCAT_IMPL(x, y) x##y
#define REFLECT_CONCAT(x, y) REFLECT_CONCAT_IMPL(x, y)

// 辅助宏：字符串化
#define REFLECT_STRINGIFY(x) #x

#define REGISTER_CLASS(ClassType) \
public: \
    using ThisClass = ClassType; \
    static Object* Create##ClassType##Func() { return new ClassType(); } \
    static inline std::string StaticClassName = std::string(#ClassType); \
    static inline Reflection::ClassInfo ClassType##ClassInfo{ StaticClassName, &Create##ClassType##Func }; \
    static Reflection::ClassInfo* GetStaticClassInfo() { return &ClassType##ClassInfo; } \
    virtual Reflection::ClassInfo* GetObjectClassInfo() override { return &ClassType##ClassInfo; } \
    virtual std::string GetObjectClassName() const override { return #ClassType; } \
    struct ClassType##Register { \
        ClassType##Register() { \
            ClassType##ClassInfo.SetParent(&Object::ObjectClassInfo); \
            Reflection::Register::Get()->RegisterClass(&ClassType::ClassType##ClassInfo); } \
    }; \
    static inline ClassType##Register ClassType##RegisterInstance;

// 带父类（继承自指定基类）
#define REGISTER_CLASS_DERIVED(ClassType, BaseClass) \
public: \
    using ThisClass = ClassType; \
    static Object* Create##ClassType##Func() { return new ClassType(); } \
    static inline std::string StaticClassName = std::string(#ClassType); \
    static inline Reflection::ClassInfo ClassType##ClassInfo{ StaticClassName, &Create##ClassType##Func }; \
    static Reflection::ClassInfo* GetStaticClassInfo() { return &ClassType##ClassInfo; } \
    virtual Reflection::ClassInfo* GetObjectClassInfo() override { return &ClassType##ClassInfo; } \
    virtual std::string GetObjectClassName() const override { return #ClassType; } \
    struct ClassType##Register { \
        ClassType##Register() { \
            ClassType##ClassInfo.SetParent(&BaseClass::BaseClass##ClassInfo); \
            Reflection::Register::Get()->RegisterClass(&ClassType::ClassType##ClassInfo); \
        } \
    }; \
    static inline ClassType##Register ClassType##RegisterInstance;

// 属性注册宏
#define PROPERTY(Type, Name) \
    public: \
        Type Name; \
        Type& get_##Name() { return Name; } \
        const Type& get_##Name() const { return Name; } \
        void set_##Name(const Type& val) { Name = val; } \
        static void __register_prop_##Name() { \
            auto acc = std::make_shared<Reflection::TypedPropertyAccessor<Type>>( \
                #Name, \
                [](Object* obj) -> Type* { \
                    return &static_cast<ThisClass*>(obj)->Name; \
                }, \
                [](Object* obj, const Type& val) { \
                    static_cast<ThisClass*>(obj)->set_##Name(val); \
                }, \
                Reflection::PropertyFlag::DefaultEditor, \
                Reflection::PropertyMetaData() \
            ); \
            ThisClass::GetStaticClassInfo()->AddProperty(acc); \
        } \
        static inline bool __prop_reg_##Name = (__register_prop_##Name(), true);

// 带范围限制的属性
#define PROPERTY_RANGE(Type, Name, Min, Max) \
    public: \
        Type Name; \
        Type& get_##Name() { return Name; } \
        const Type& get_##Name() const { return Name; } \
        void set_##Name(const Type& val) { Name = val; } \
        static void __register_prop_##Name() { \
            Reflection::PropertyMetaData meta; \
            meta.MinValue = Min; \
            meta.MaxValue = Max; \
            auto acc = std::make_shared<Reflection::TypedPropertyAccessor<Type>>( \
                #Name, \
                [](Object* obj) -> Type* { \
                    return &static_cast<ThisClass*>(obj)->Name; \
                }, \
                [](Object* obj, const Type& val) { \
                    static_cast<ThisClass*>(obj)->set_##Name(val); \
                }, \
                Reflection::PropertyFlag::DefaultEditor | Reflection::PropertyFlag::Range, \
                meta \
            ); \
            ThisClass::GetStaticClassInfo()->AddProperty(acc); \
        } \
        static inline bool __prop_reg_##Name = (__register_prop_##Name(), true);

// 只读属性
#define PROPERTY_READONLY(Type, Name) \
    public: \
        Type Name; \
        const Type& get_##Name() const { return Name; } \
        static void __register_prop_##Name() { \
            auto acc = std::make_shared<Reflection::TypedPropertyAccessor<Type>>( \
                #Name, \
                [](Object* obj) -> Type* { \
                    return &static_cast<ThisClass*>(obj)->Name; \
                }, \
                nullptr, \
                Reflection::PropertyFlag::ReadOnlyEditor, \
                Reflection::PropertyMetaData() \
            ); \
            ThisClass::GetStaticClassInfo()->AddProperty(acc); \
        } \
        static inline bool __prop_reg_##Name = (__register_prop_##Name(), true);

// 字符串快捷方式
#define PROPERTY_STRING(Name) PROPERTY(std::string, Name)

// 对象指针属性
#define PROPERTY_OBJECT_PTR(Type, Name) \
    public: \
        Type* Name = nullptr; \
        Type* get_##Name() const { return Name; } \
        void set_##Name(Type* val) { Name = val; } \
        static void __register_prop_##Name() { \
            auto acc = std::make_shared<Reflection::PointerPropertyAccessor<Type>>( \
                #Name, \
                [](Object* obj) -> Type* { \
                    return static_cast<ThisClass*>(obj)->get_##Name(); \
                }, \
                [](Object* obj, Type* val) { \
                    static_cast<ThisClass*>(obj)->set_##Name(val); \
                } \
            ); \
            ThisClass::GetStaticClassInfo()->AddProperty(acc); \
        } \
        static inline bool __prop_reg_##Name = (__register_prop_##Name(), true);

// unique_ptr 属性
#define PROPERTY_UNIQUE_PTR(Type, Name) \
    public: \
        std::unique_ptr<Type> Name; \
        Type* get_##Name() const { return Name.get(); } \
        void set_##Name(Type* val) { Name.reset(val); } \
        static void __register_prop_##Name() { \
            auto acc = std::make_shared<Reflection::PointerPropertyAccessor<Type>>( \
                #Name, \
                [](Object* obj) -> Type* { \
                    return static_cast<ThisClass*>(obj)->get_##Name(); \
                }, \
                [](Object* obj, Type* val) { \
                    static_cast<ThisClass*>(obj)->set_##Name(val); \
                } \
            ); \
            ThisClass::GetStaticClassInfo()->AddProperty(acc); \
        } \
        static inline bool __prop_reg_##Name = (__register_prop_##Name(), true);

// std::vector 容器属性
#define PROPERTY_VECTOR(ElemType, Name) \
    public: \
        std::vector<ElemType> Name; \
        std::vector<ElemType>& get_##Name() { return Name; } \
        const std::vector<ElemType>& get_##Name() const { return Name; } \
        void set_##Name(const std::vector<ElemType>& val) { Name = val; } \
        static void __register_prop_##Name() { \
            auto acc = std::make_shared<Reflection::TypedPropertyAccessor<std::vector<ElemType>>>( \
                #Name, \
                [](Object* obj) -> std::vector<ElemType>* { \
                    return &static_cast<ThisClass*>(obj)->Name; \
                }, \
                [](Object* obj, const std::vector<ElemType>& val) { \
                    static_cast<ThisClass*>(obj)->set_##Name(val); \
                }, \
                Reflection::PropertyFlag::DefaultEditor, \
                Reflection::PropertyMetaData() \
            ); \
            ThisClass::GetStaticClassInfo()->AddProperty(acc); \
        } \
        static inline bool __prop_reg_##Name = (__register_prop_##Name(), true);

// std::vector<unique_ptr<T>> 属性（常用容器组合）
#define PROPERTY_VECTOR_UNIQUE_PTR(ElemType, Name) \
    public: \
        std::vector<std::unique_ptr<ElemType>> Name; \
        std::vector<std::unique_ptr<ElemType>>& get_##Name() { return Name; } \
        const std::vector<std::unique_ptr<ElemType>>& get_##Name() const { return Name; } \
        void add_##Name(ElemType* val) { Name.emplace_back(val); } \
        void remove_##Name(size_t index) { if (index < Name.size()) Name.erase(Name.begin() + index); } \
        size_t get_##Name##_size() const { return Name.size(); } \
        ElemType* get_##Name##_at(size_t index) const { \
            return (index < Name.size()) ? Name[index].get() : nullptr; \
        } \
        static void __register_prop_##Name() { \
            auto acc = std::make_shared<Reflection::TypedPropertyAccessor<std::vector<std::unique_ptr<ElemType>>>>( \
                #Name, \
                [](Object* obj) -> std::vector<std::unique_ptr<ElemType>>* { \
                    return &static_cast<ThisClass*>(obj)->Name; \
                }, \
                nullptr, \
                Reflection::PropertyFlag::DefaultEditor, \
                Reflection::PropertyMetaData() \
            ); \
            ThisClass::GetStaticClassInfo()->AddProperty(acc); \
        } \
        static inline bool __prop_reg_##Name = (__register_prop_##Name(), true)