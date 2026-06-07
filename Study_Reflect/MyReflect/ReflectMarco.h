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
        static inline bool __prop_reg_##Name = (__register_prop_##Name(), true);
#define PROPERTY_VECTOR_UNIQUE_PTR(ElemType, Name) \
    public: \
        std::vector<std::unique_ptr<ElemType>> Name; \
        std::vector<ElemType*> Name##_raw; \
        \
        std::vector<std::unique_ptr<ElemType>>& get_##Name() { return Name; } \
        const std::vector<std::unique_ptr<ElemType>>& get_##Name() const { return Name; } \
        \
        void add_##Name(ElemType* val) { \
            Name.emplace_back(val); \
            Name##_raw.push_back(val); \
        } \
        \
        void remove_##Name(size_t index) { \
            if (index < Name.size()) { \
                Name.erase(Name.begin() + index); \
                Name##_raw.erase(Name##_raw.begin() + index); \
            } \
        } \
        \
        size_t get_##Name##_size() const { return Name.size(); } \
        ElemType* get_##Name##_at(size_t index) const { \
            return (index < Name.size()) ? Name[index].get() : nullptr; \
        } \
        \
        static void __register_prop_##Name() { \
            auto acc = std::make_shared<Reflection::TypedPropertyAccessor<std::vector<ElemType*>>>( \
                #Name, \
                [](Object* obj) -> std::vector<ElemType*>* { \
                    return &static_cast<ThisClass*>(obj)->Name##_raw; \
                }, \
                nullptr, \
                Reflection::PropertyFlag::DefaultEditor, \
                Reflection::PropertyMetaData() \
            ); \
            ThisClass::GetStaticClassInfo()->AddProperty(acc); \
        } \
        static inline bool __prop_reg_##Name = (__register_prop_##Name(), true);

// 带显示名称的属性
#define PROPERTY_DISPLAY(Type, Name, DisplayStr) \
    private: Type Name; \
    public: \
        Type& get_##Name() { return Name; } \
        const Type& get_##Name() const { return Name; } \
        void set_##Name(const Type& val) { Name = val; } \
        static void __register_prop_##Name() { \
            Reflection::PropertyMetaData meta; \
            meta.DisplayName = DisplayStr; \
            auto acc = std::make_shared<Reflection::TypedPropertyAccessor<Type>>( \
                #Name, \
                [](Object* obj) -> Type* { return &static_cast<ThisClass*>(obj)->Name; }, \
                [](Object* obj, const Type& val) { static_cast<ThisClass*>(obj)->set_##Name(val); }, \
                Reflection::PropertyFlag::DefaultEditor, \
                meta \
            ); \
            ThisClass::GetStaticClassInfo()->AddProperty(acc); \
        } \
        static inline bool __prop_reg_##Name = (__register_prop_##Name(), true);

// 带提示文本的属性
#define PROPERTY_TOOLTIP(Type, Name, TooltipStr) \
    private: Type Name; \
    public: \
        Type& get_##Name() { return Name; } \
        const Type& get_##Name() const { return Name; } \
        void set_##Name(const Type& val) { Name = val; } \
        static void __register_prop_##Name() { \
            Reflection::PropertyMetaData meta; \
            meta.TooltipText = TooltipStr; \
            auto acc = std::make_shared<Reflection::TypedPropertyAccessor<Type>>( \
                #Name, \
                [](Object* obj) -> Type* { return &static_cast<ThisClass*>(obj)->Name; }, \
                [](Object* obj, const Type& val) { static_cast<ThisClass*>(obj)->set_##Name(val); }, \
                Reflection::PropertyFlag::DefaultEditor, \
                meta \
            ); \
            ThisClass::GetStaticClassInfo()->AddProperty(acc); \
        } \
        static inline bool __prop_reg_##Name = (__register_prop_##Name(), true);

// 带分类的属性
#define PROPERTY_CATEGORY(Type, Name, CategoryStr) \
    private: Type Name; \
    public: \
        Type& get_##Name() { return Name; } \
        const Type& get_##Name() const { return Name; } \
        void set_##Name(const Type& val) { Name = val; } \
        static void __register_prop_##Name() { \
            Reflection::PropertyMetaData meta; \
            meta.Category = CategoryStr; \
            auto acc = std::make_shared<Reflection::TypedPropertyAccessor<Type>>( \
                #Name, \
                [](Object* obj) -> Type* { return &static_cast<ThisClass*>(obj)->Name; }, \
                [](Object* obj, const Type& val) { static_cast<ThisClass*>(obj)->set_##Name(val); }, \
                Reflection::PropertyFlag::DefaultEditor, \
                meta \
            ); \
            ThisClass::GetStaticClassInfo()->AddProperty(acc); \
        } \
        static inline bool __prop_reg_##Name = (__register_prop_##Name(), true);

// 完整元数据版本（显示名 + 分类 + 提示）
#define PROPERTY_FULL(Type, Name, DisplayStr, CategoryStr, TooltipStr) \
    private: Type Name; \
    public: \
        Type& get_##Name() { return Name; } \
        const Type& get_##Name() const { return Name; } \
        void set_##Name(const Type& val) { Name = val; } \
        static void __register_prop_##Name() { \
            Reflection::PropertyMetaData meta; \
            meta.DisplayName = DisplayStr; \
            meta.Category = CategoryStr; \
            meta.TooltipText = TooltipStr; \
            auto acc = std::make_shared<Reflection::TypedPropertyAccessor<Type>>( \
                #Name, \
                [](Object* obj) -> Type* { return &static_cast<ThisClass*>(obj)->Name; }, \
                [](Object* obj, const Type& val) { static_cast<ThisClass*>(obj)->set_##Name(val); }, \
                Reflection::PropertyFlag::DefaultEditor, \
                meta \
            ); \
            ThisClass::GetStaticClassInfo()->AddProperty(acc); \
        } \
        static inline bool __prop_reg_##Name = (__register_prop_##Name(), true);


// 带元数据的范围属性
#define PROPERTY_RANGE_DISPLAY(Type, Name, MinVal, MaxVal, DisplayStr) \
    private: Type Name; \
    public: \
        Type& get_##Name() { return Name; } \
        const Type& get_##Name() const { return Name; } \
        void set_##Name(const Type& val) { Name = val; } \
        static void __register_prop_##Name() { \
            Reflection::PropertyMetaData meta; \
            meta.DisplayName = DisplayStr; \
            meta.MinValue = MinVal; \
            meta.MaxValue = MaxVal; \
            auto acc = std::make_shared<Reflection::TypedPropertyAccessor<Type>>( \
                #Name, \
                [](Object* obj) -> Type* { return &static_cast<ThisClass*>(obj)->Name; }, \
                [](Object* obj, const Type& val) { static_cast<ThisClass*>(obj)->set_##Name(val); }, \
                Reflection::PropertyFlag::DefaultEditor | Reflection::PropertyFlag::Range, \
                meta \
            ); \
            ThisClass::GetStaticClassInfo()->AddProperty(acc); \
        } \
        static inline bool __prop_reg_##Name = (__register_prop_##Name(), true);

#define PROPERTY_RANGE_CATEGORY(Type, Name, MinVal, MaxVal, CategoryStr) \
    private: Type Name; \
    public: \
        Type& get_##Name() { return Name; } \
        const Type& get_##Name() const { return Name; } \
        void set_##Name(const Type& val) { Name = val; } \
        static void __register_prop_##Name() { \
            Reflection::PropertyMetaData meta; \
            meta.Category = CategoryStr; \
            meta.MinValue = MinVal; \
            meta.MaxValue = MaxVal; \
            auto acc = std::make_shared<Reflection::TypedPropertyAccessor<Type>>( \
                #Name, \
                [](Object* obj) -> Type* { return &static_cast<ThisClass*>(obj)->Name; }, \
                [](Object* obj, const Type& val) { static_cast<ThisClass*>(obj)->set_##Name(val); }, \
                Reflection::PropertyFlag::DefaultEditor | Reflection::PropertyFlag::Range, \
                meta \
            ); \
            ThisClass::GetStaticClassInfo()->AddProperty(acc); \
        } \
        static inline bool __prop_reg_##Name = (__register_prop_##Name(), true);


// 字符串属性的元数据版本
#define PROPERTY_STRING_DISPLAY(Name, DisplayStr) \
    PROPERTY_DISPLAY(std::string, Name, DisplayStr)

#define PROPERTY_STRING_CATEGORY(Name, CategoryStr) \
    PROPERTY_CATEGORY(std::string, Name, CategoryStr)

#define PROPERTY_STRING_FULL(Name, DisplayStr, CategoryStr, TooltipStr) \
    PROPERTY_FULL(std::string, Name, DisplayStr, CategoryStr, TooltipStr)

#define PROPERTY_OBJECT_PTR_DISPLAY(Type, Name, DisplayStr) \
    private: Type* Name = nullptr; \
    public: \
        Type* get_##Name() const { return Name; } \
        void set_##Name(Type* val) { Name = val; } \
        static void __register_prop_##Name() { \
            Reflection::PropertyMetaData meta; \
            meta.DisplayName = DisplayStr; \
            auto acc = std::make_shared<Reflection::PointerPropertyAccessor<Type>>( \
                #Name, \
                [](Object* obj) -> Type* { return static_cast<ThisClass*>(obj)->get_##Name(); }, \
                [](Object* obj, Type* val) { static_cast<ThisClass*>(obj)->set_##Name(val); }, \
                Reflection::PropertyFlag::DefaultEditor, \
                meta \
            ); \
            ThisClass::GetStaticClassInfo()->AddProperty(acc); \
        } \
        static inline bool __prop_reg_##Name = (__register_prop_##Name(), true);

#define PROPERTY_OBJECT_PTR_CATEGORY(Type, Name, CategoryStr) \
    private: Type* Name = nullptr; \
    public: \
        Type* get_##Name() const { return Name; } \
        void set_##Name(Type* val) { Name = val; } \
        static void __register_prop_##Name() { \
            Reflection::PropertyMetaData meta; \
            meta.Category = CategoryStr; \
            auto acc = std::make_shared<Reflection::PointerPropertyAccessor<Type>>( \
                #Name, \
                [](Object* obj) -> Type* { return static_cast<ThisClass*>(obj)->get_##Name(); }, \
                [](Object* obj, Type* val) { static_cast<ThisClass*>(obj)->set_##Name(val); }, \
                Reflection::PropertyFlag::DefaultEditor, \
                meta \
            ); \
            ThisClass::GetStaticClassInfo()->AddProperty(acc); \
        } \
        static inline bool __prop_reg_##Name = (__register_prop_##Name(), true);

// 下拉选择属性（枚举风格）
#define PROPERTY_DROPDOWN(Type, Name, OptionsStr) \
    private: Type Name; \
    public: \
        Type& get_##Name() { return Name; } \
        const Type& get_##Name() const { return Name; } \
        void set_##Name(const Type& val) { Name = val; } \
        static void __register_prop_##Name() { \
            Reflection::PropertyMetaData meta; \
            std::string opts = OptionsStr; \
            size_t start = 0, end = 0; \
            while ((end = opts.find(',', start)) != std::string::npos) { \
                meta.DropdownOptions.push_back(opts.substr(start, end - start)); \
                start = end + 1; \
            } \
            meta.DropdownOptions.push_back(opts.substr(start)); \
            auto acc = std::make_shared<Reflection::TypedPropertyAccessor<Type>>( \
                #Name, \
                [](Object* obj) -> Type* { return &static_cast<ThisClass*>(obj)->Name; }, \
                [](Object* obj, const Type& val) { static_cast<ThisClass*>(obj)->set_##Name(val); }, \
                Reflection::PropertyFlag::DefaultEditor | Reflection::PropertyFlag::Dropdown, \
                meta \
            ); \
            ThisClass::GetStaticClassInfo()->AddProperty(acc); \
        } \
        static inline bool __prop_reg_##Name = (__register_prop_##Name(), true);

// 下拉选择 + 显示名称
#define PROPERTY_DROPDOWN_DISPLAY(Type, Name, OptionsStr, DisplayStr) \
    private: Type Name; \
    public: \
        Type& get_##Name() { return Name; } \
        const Type& get_##Name() const { return Name; } \
        void set_##Name(const Type& val) { Name = val; } \
        static void __register_prop_##Name() { \
            Reflection::PropertyMetaData meta; \
            meta.DisplayName = DisplayStr; \
            std::string opts = OptionsStr; \
            size_t start = 0, end = 0; \
            while ((end = opts.find(',', start)) != std::string::npos) { \
                meta.DropdownOptions.push_back(opts.substr(start, end - start)); \
                start = end + 1; \
            } \
            meta.DropdownOptions.push_back(opts.substr(start)); \
            auto acc = std::make_shared<Reflection::TypedPropertyAccessor<Type>>( \
                #Name, \
                [](Object* obj) -> Type* { return &static_cast<ThisClass*>(obj)->Name; }, \
                [](Object* obj, const Type& val) { static_cast<ThisClass*>(obj)->set_##Name(val); }, \
                Reflection::PropertyFlag::DefaultEditor | Reflection::PropertyFlag::Dropdown, \
                meta \
            ); \
            ThisClass::GetStaticClassInfo()->AddProperty(acc); \
        } \
        static inline bool __prop_reg_##Name = (__register_prop_##Name(), true);

//版本2
/*
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

#define REFLECT_CONCAT_IMPL(x, y) x##y
#define REFLECT_CONCAT(x, y) REFLECT_CONCAT_IMPL(x, y)
#define REFLECT_STRINGIFY(x) #x

#define REFLECT_GET_ARG_COUNT(...) \
    REFLECT_GET_ARG_COUNT_IMPL(__VA_ARGS__, 5,4,3,2,1)
#define REFLECT_GET_ARG_COUNT_IMPL(_1,_2,_3,_4,_5,N,...) N

// 基础属性宏（内部使用）
#define PROPERTY_INTERNAL(Type, Name, Flags, Meta) \
    private: Type Name; \
    public: \
        Type& get_##Name() { return Name; } \
        const Type& get_##Name() const { return Name; } \
        void set_##Name(const Type& val) { Name = val; } \
        static void __register_prop_##Name() { \
            auto acc = std::make_shared<Reflection::TypedPropertyAccessor<Type>>( \
                #Name, \
                [](Object* obj) -> Type* { return &static_cast<ThisClass*>(obj)->Name; }, \
                [](Object* obj, const Type& val) { static_cast<ThisClass*>(obj)->set_##Name(val); }, \
                Flags, Meta \
            ); \
            ThisClass::GetStaticClassInfo()->AddProperty(acc); \
        } \
        static inline bool __prop_reg_##Name = (__register_prop_##Name(), true)

#define PROPERTY_RANGE_INTERNAL(Type, Name, MinVal, MaxVal, Flags, Meta) \
    private: Type Name; \
    public: \
        Type& get_##Name() { return Name; } \
        const Type& get_##Name() const { return Name; } \
        void set_##Name(const Type& val) { Name = val; } \
        static void __register_prop_##Name() { \
            Meta.MinValue = MinVal; \
            Meta.MaxValue = MaxVal; \
            auto acc = std::make_shared<Reflection::TypedPropertyAccessor<Type>>( \
                #Name, \
                [](Object* obj) -> Type* { return &static_cast<ThisClass*>(obj)->Name; }, \
                [](Object* obj, const Type& val) { static_cast<ThisClass*>(obj)->set_##Name(val); }, \
                Flags | Reflection::PropertyFlag::Range, Meta \
            ); \
            ThisClass::GetStaticClassInfo()->AddProperty(acc); \
        } \
        static inline bool __prop_reg_##Name = (__register_prop_##Name(), true)


// 对象指针属性宏
#define PROPERTY_OBJECT_PTR_INTERNAL(Type, Name, Flags, Meta) \
    private: Type* Name = nullptr; \
    public: \
        Type* get_##Name() const { return Name; } \
        void set_##Name(Type* val) { Name = val; } \
        static void __register_prop_##Name() { \
            auto acc = std::make_shared<Reflection::PointerPropertyAccessor<Type>>( \
                #Name, \
                [](Object* obj) -> Type* { return static_cast<ThisClass*>(obj)->get_##Name(); }, \
                [](Object* obj, Type* val) { static_cast<ThisClass*>(obj)->set_##Name(val); }, \
                Flags, Meta \
            ); \
            ThisClass::GetStaticClassInfo()->AddProperty(acc); \
        } \
        static inline bool __prop_reg_##Name = (__register_prop_##Name(), true)


// UniquePtr 属性宏
#define PROPERTY_UNIQUE_PTR_INTERNAL(Type, Name, Flags, Meta) \
    private: std::unique_ptr<Type> Name; \
    public: \
        Type* get_##Name() const { return Name.get(); } \
        void set_##Name(Type* val) { Name.reset(val); } \
        static void __register_prop_##Name() { \
            auto acc = std::make_shared<Reflection::PointerPropertyAccessor<Type>>( \
                #Name, \
                [](Object* obj) -> Type* { return static_cast<ThisClass*>(obj)->get_##Name(); }, \
                [](Object* obj, Type* val) { static_cast<ThisClass*>(obj)->set_##Name(val); }, \
                Flags, Meta \
            ); \
            ThisClass::GetStaticClassInfo()->AddProperty(acc); \
        } \
        static inline bool __prop_reg_##Name = (__register_prop_##Name(), true)


// Vector 属性宏
#define PROPERTY_VECTOR_INTERNAL(ElemType, Name, Flags, Meta) \
    private: std::vector<ElemType> Name; \
    public: \
        std::vector<ElemType>& get_##Name() { return Name; } \
        const std::vector<ElemType>& get_##Name() const { return Name; } \
        void set_##Name(const std::vector<ElemType>& val) { Name = val; } \
        static void __register_prop_##Name() { \
            auto acc = std::make_shared<Reflection::TypedPropertyAccessor<std::vector<ElemType>>>( \
                #Name, \
                [](Object* obj) -> std::vector<ElemType>* { return &static_cast<ThisClass*>(obj)->Name; }, \
                [](Object* obj, const std::vector<ElemType>& val) { static_cast<ThisClass*>(obj)->set_##Name(val); }, \
                Flags, Meta \
            ); \
            ThisClass::GetStaticClassInfo()->AddProperty(acc); \
        } \
        static inline bool __prop_reg_##Name = (__register_prop_##Name(), true)

// Vector<UniquePtr> 属性宏
#define PROPERTY_VECTOR_UNIQUE_INTERNAL(ElemType, Name, Flags, Meta) \
    private: std::vector<std::unique_ptr<ElemType>> Name; \
    public: \
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
                nullptr, Flags, Meta \
            ); \
            ThisClass::GetStaticClassInfo()->AddProperty(acc); \
        } \
        static inline bool __prop_reg_##Name = (__register_prop_##Name(), true)


// 统一的 UPROPERTY 宏（推荐使用）

#define UPROPERTY(...) \
    REFLECT_CONCAT(UPROPERTY_DISPATCH, REFLECT_GET_ARG_COUNT(__VA_ARGS__))(__VA_ARGS__)

// 1个参数：自动推断类型（使用默认值初始化）
#define UPROPERTY_DISPATCH_1(NameAndDefault) \
    UPROPERTY_AUTO_IMPL NameAndDefault

#define UPROPERTY_AUTO_IMPL(Name, DefaultValue) \
    private: auto Name = DefaultValue; \
    public: \
        decltype(Name)& get_##Name() { return Name; } \
        const decltype(Name)& get_##Name() const { return Name; } \
        void set_##Name(const decltype(Name)& val) { Name = val; } \
        static void __register_prop_##Name() { \
            Reflection::PropertyMetaData meta; \
            using Type = decltype(Name); \
            auto acc = std::make_shared<Reflection::TypedPropertyAccessor<Type>>( \
                #Name, \
                [](Object* obj) -> Type* { return &static_cast<ThisClass*>(obj)->Name; }, \
                [](Object* obj, const Type& val) { static_cast<ThisClass*>(obj)->set_##Name(val); }, \
                Reflection::PropertyFlag::DefaultEditor, meta \
            ); \
            ThisClass::GetStaticClassInfo()->AddProperty(acc); \
        } \
        static inline bool __prop_reg_##Name = (__register_prop_##Name(), true)

// 2个参数：基础属性
#define UPROPERTY_DISPATCH_2(Type, Name) \
    PROPERTY_INTERNAL(Type, Name, Reflection::PropertyFlag::DefaultEditor, PropertyMetaData())

// 3个参数：带修饰符
#define UPROPERTY_DISPATCH_3(Type, Name, Modifier) \
    REFLECT_CONCAT(UPROPERTY_MOD_, Modifier)(Type, Name)

// 4个参数：带修饰符和额外参数
#define UPROPERTY_DISPATCH_4(Type, Name, Modifier, Extra) \
    REFLECT_CONCAT(UPROPERTY_MOD_, Modifier)(Type, Name, Extra)

// 5个参数：带修饰符和两个额外参数
#define UPROPERTY_DISPATCH_5(Type, Name, Modifier, Extra1, Extra2) \
    REFLECT_CONCAT(UPROPERTY_MOD_, Modifier)(Type, Name, Extra1, Extra2)


// 修饰符实现

// 范围限制
#define UPROPERTY_MOD_Range(Type, Name, Min, Max) \
    PROPERTY_RANGE_INTERNAL(Type, Name, Min, Max, Reflection::PropertyFlag::DefaultEditor, Reflection::PropertyMetaData())

#define UPROPERTY_MOD_Range2(Type, Name, Min, Max) \
    UPROPERTY_MOD_Range(Type, Name, Min, Max)

// 分类
#define UPROPERTY_MOD_Category(Type, Name, Category) \
    Reflection::PropertyMetaData REFLECT_CONCAT(meta_, Name); \
    REFLECT_CONCAT(meta_, Name).Category = Category; \
    PROPERTY_INTERNAL(Type, Name, Reflection::PropertyFlag::DefaultEditor, REFLECT_CONCAT(meta_, Name))

// 显示名称
#define UPROPERTY_MOD_Display(Type, Name, DisplayName) \
    Reflection::PropertyMetaData REFLECT_CONCAT(meta_, Name); \
    REFLECT_CONCAT(meta_, Name).DisplayName = DisplayName; \
    PROPERTY_INTERNAL(Type, Name, Reflection::PropertyFlag::DefaultEditor, REFLECT_CONCAT(meta_, Name))

// 提示文本
#define UPROPERTY_MOD_Tooltip(Type, Name, TooltipText) \
    Reflection::PropertyMetaData REFLECT_CONCAT(meta_, Name); \
    REFLECT_CONCAT(meta_, Name).TooltipText = TooltipText; \
    PROPERTY_INTERNAL(Type, Name, Reflection::PropertyFlag::DefaultEditor, REFLECT_CONCAT(meta_, Name))

// 只读
#define UPROPERTY_MOD_ReadOnly(Type, Name) \
    PROPERTY_INTERNAL(Type, Name, Reflection::PropertyFlag::ReadOnlyEditor, Reflection::PropertyMetaData())

// 不序列化
#define UPROPERTY_MOD_Transient(Type, Name) \
    PROPERTY_INTERNAL(Type, Name, Reflection::PropertyFlag::Visible | Reflection::PropertyFlag::Editable, Reflection::PropertyMetaData())

// 对象指针
#define UPROPERTY_MOD_ObjectPtr(Type, Name) \
    PROPERTY_OBJECT_PTR_INTERNAL(Type, Name, Reflection::PropertyFlag::DefaultEditor, Reflection::PropertyMetaData())

// UniquePtr
#define UPROPERTY_MOD_UniquePtr(Type, Name) \
    PROPERTY_UNIQUE_PTR_INTERNAL(Type, Name, Reflection::PropertyFlag::DefaultEditor, Reflection::PropertyMetaData())

// Vector
#define UPROPERTY_MOD_Vector(ElemType, Name) \
    PROPERTY_VECTOR_INTERNAL(ElemType, Name, Reflection::PropertyFlag::DefaultEditor, Reflection::PropertyMetaData())

// VectorUnique
#define UPROPERTY_MOD_VectorUnique(ElemType, Name) \
    PROPERTY_VECTOR_UNIQUE_INTERNAL(ElemType, Name, Reflection::PropertyFlag::DefaultEditor, Reflection::PropertyMetaData())

// 颜色
#define UPROPERTY_MOD_Color(Name) \
    private: float Name[4] = {1.0f, 1.0f, 1.0f, 1.0f}; \
    public: \
        float* get_##Name() { return Name; } \
        const float* get_##Name() const { return Name; } \
        static void __register_prop_##Name() { \
            Reflection::Reflection::PropertyMetaData meta; \
            auto acc = std::make_shared<Reflection::TypedPropertyAccessor<std::array<float, 4>>>( \
                #Name, \
                [](Object* obj) -> std::array<float, 4>* { \
                    return reinterpret_cast<std::array<float, 4>*>(static_cast<ThisClass*>(obj)->get_##Name()); \
                }, \
                nullptr, Reflection::PropertyFlag::DefaultEditor, meta \
            ); \
            ThisClass::GetStaticClassInfo()->AddProperty(acc); \
        } \
        static inline bool __prop_reg_##Name = (__register_prop_##Name(), true)

// 多行文本
#define UPROPERTY_MOD_Multiline(Type, Name) \
    Reflection::PropertyMetaData REFLECT_CONCAT(meta_, Name); \
    REFLECT_CONCAT(meta_, Name).IsMultiline = true; \
    PROPERTY_INTERNAL(Type, Name, Reflection::PropertyFlag::DefaultEditor, REFLECT_CONCAT(meta_, Name))

// 下拉选择
#define UPROPERTY_MOD_Dropdown(Type, Name, Options) \
    Reflection::PropertyMetaData REFLECT_CONCAT(meta_, Name); \
    std::string REFLECT_CONCAT(opts_, Name) = Options; \
    size_t REFLECT_CONCAT(start_, Name) = 0, REFLECT_CONCAT(end_, Name) = 0; \
    while ((REFLECT_CONCAT(end_, Name) = REFLECT_CONCAT(opts_, Name).find(',', REFLECT_CONCAT(start_, Name))) != std::string::npos) { \
        REFLECT_CONCAT(meta_, Name).DropdownOptions.push_back(REFLECT_CONCAT(opts_, Name).substr(REFLECT_CONCAT(start_, Name), REFLECT_CONCAT(end_, Name) - REFLECT_CONCAT(start_, Name))); \
        REFLECT_CONCAT(start_, Name) = REFLECT_CONCAT(end_, Name) + 1; \
    } \
    REFLECT_CONCAT(meta_, Name).DropdownOptions.push_back(REFLECT_CONCAT(opts_, Name).substr(REFLECT_CONCAT(start_, Name))); \
    PROPERTY_INTERNAL(Type, Name, Reflection::PropertyFlag::DefaultEditor | Reflection::PropertyFlag::Dropdown, REFLECT_CONCAT(meta_, Name))

*/