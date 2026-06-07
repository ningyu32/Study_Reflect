#pragma once
#include <any>
#include <typeindex>
#include <string>
#include <vector>
#include <map>

class Object;

namespace Reflection 
{
    // 可序列化的值类型
    class PropertyValue {
    public:
        PropertyValue() = default;

        template<typename T>
        PropertyValue(T value) : m_Value(value), m_TypeIndex(typeid(T)) {}

        // 类型转换
        template<typename T>
        T Get() const {
            try {
                return std::any_cast<T>(m_Value);
            }
            catch (const std::bad_any_cast&) {
                return T{};
            }
        }

        template<typename T>
        void Set(const T& value) {
            m_Value = value;
            m_TypeIndex = typeid(T);
        }

        std::type_index GetType() const { return m_TypeIndex; }
        bool IsValid() const { return m_Value.has_value(); }
        bool IsNull() const { return !m_Value.has_value(); }

        // 序列化支持
        std::string ToString() const;
        bool FromString(const std::string& str);

    private:
        std::any m_Value;
        std::type_index m_TypeIndex = typeid(void);
    };
} // namespace Reflection