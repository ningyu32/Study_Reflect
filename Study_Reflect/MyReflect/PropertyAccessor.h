#pragma once
#include <functional>
#include <memory>
#include <string>
#include "PropertyFlags.h"
#include "PropertyValue.h"

class Object;

namespace Reflection {
    class ClassInfo;

    // 属性访问器接口
    class IPropertyAccessor {
    public:
        virtual ~IPropertyAccessor() = default;

        // 读写属性值
        virtual PropertyValue Get(Object* obj) const = 0;
        virtual bool Set(Object* obj, const PropertyValue& value) = 0;

        // 类型信息
        virtual std::string GetPropertyName() const = 0;
        virtual std::string GetTypeName() const = 0;
        virtual std::type_index GetValueType() const = 0;

        // 标志和元数据
        virtual PropertyFlag GetFlags() const { return PropertyFlag::DefaultEditor; }
        virtual const PropertyMetaData& GetMetaData() const 
        {
            static PropertyMetaData empty;
            return empty;
        }
        
        // 编辑器支持
        virtual bool IsVisible() const { return HasFlag(GetFlags(), PropertyFlag::Visible); }
        virtual bool IsEditable() const { return HasFlag(GetFlags(), PropertyFlag::Editable); }
        virtual bool ShouldSerialize() const { return HasFlag(GetFlags(), PropertyFlag::Serialize); }

        //// 特殊类型判断
        //virtual bool IsContainer() const { return false; }
        //virtual bool IsObjectPtr() const { return false; }
        //virtual size_t GetContainerSize(Object* obj) const { return 0; }
    };

    // 普通类型的访问器 (TypedPropertyAccessor)
    template<typename T>
    class TypedPropertyAccessor : public IPropertyAccessor {
    public:
        using GetterFunc = std::function<T* (Object*)>;
        using SetterFunc = std::function<void(Object*, const T&)>;
        TypedPropertyAccessor(
            const std::string& name,
            GetterFunc getter,
            SetterFunc setter,
            PropertyFlag flags,
            const PropertyMetaData& meta)
            : m_Name(name), m_Getter(getter), m_Setter(setter), m_Flags(flags), m_Meta(meta) {}
        PropertyValue Get(Object* obj) const override {
            if (auto ptr = m_Getter(obj)) return PropertyValue(*ptr);
            return PropertyValue();
        }
        bool Set(Object* obj, const PropertyValue& value) override {
            if (m_Setter && value.GetType() == typeid(T)) {
                m_Setter(obj, value.Get<T>());
                return true;
            }
            return false;
        }
        std::string GetPropertyName() const override { return m_Name; }
        std::string GetTypeName() const override {
            if constexpr (std::is_same_v<T, int>) return "int";
            else if constexpr (std::is_same_v<T, float>) return "float";
            else if constexpr (std::is_same_v<T, double>) return "double";
            else if constexpr (std::is_same_v<T, bool>) return "bool";
            else if constexpr (std::is_same_v<T, std::string>) return "string";
            else return "unknown";
        }
        std::type_index GetValueType() const override { return typeid(T); }
        PropertyFlag GetFlags() const override { return m_Flags; }
        const PropertyMetaData& GetMetaData() const override { return m_Meta; }
    private:
        std::string m_Name;
        GetterFunc m_Getter;
        SetterFunc m_Setter;
        PropertyFlag m_Flags;
        PropertyMetaData m_Meta;
    };

    // 对象指针的访问器
    template<typename T>
    class PointerPropertyAccessor : public IPropertyAccessor {
    public:
        using GetterFunc = std::function<T* (Object*)>;
        using SetterFunc = std::function<void(Object*, T*)>;
        PointerPropertyAccessor(const std::string& name, GetterFunc getter, SetterFunc setter)
            : m_Name(name), m_Getter(getter), m_Setter(setter) {
        }
        PropertyValue Get(Object* obj) const override {
            if (auto ptr = m_Getter(obj)) return PropertyValue(ptr);
            return PropertyValue(nullptr);
        }
        bool Set(Object* obj, const PropertyValue& value) override {
            if (m_Setter && value.GetType() == typeid(T*)) {
                m_Setter(obj, value.Get<T*>());
                return true;
            }
            return false;
        }
        std::string GetPropertyName() const override { return m_Name; }
        std::string GetTypeName() const override { return std::string("Object*"); }
        std::type_index GetValueType() const override { return typeid(T*); }
        PropertyFlag GetFlags() const override { return PropertyFlag::DefaultEditor; }
    private:
        std::string m_Name;
        GetterFunc m_Getter;
        SetterFunc m_Setter;
    };
} // namespace Reflection