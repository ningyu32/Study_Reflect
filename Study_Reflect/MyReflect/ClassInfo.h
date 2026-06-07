#pragma once
#include <map>
#include <string>
#include <vector>
#include <memory>
#include "PropertyAccessor.h"

class Object;

using CreateFunc = Object * (*)();

namespace Reflection {
    class ClassInfo {
    public:
        ClassInfo() = default;
        ClassInfo(const std::string& name, CreateFunc func)
            : m_ClassName(name), m_CreateFunc(func) {}

        // 基本属性
        const std::string& GetClassName() const { return m_ClassName; }
        CreateFunc GetCreateFunc() const { return m_CreateFunc; }

        // 属性管理
        void AddProperty(std::shared_ptr<IPropertyAccessor> accessor) 
        {
            if (accessor) 
            {
                m_Properties[accessor->GetPropertyName()] = accessor;
            }
        }

        std::shared_ptr<IPropertyAccessor> GetProperty(const std::string& name)
        {
            auto it = m_Properties.find(name);
            return it != m_Properties.end() ? it->second : nullptr;
        }

        const std::map<std::string, std::shared_ptr<IPropertyAccessor>>& GetProperties() const 
        {
            return m_Properties;
        }

        // 获取可序列化的属性
        std::vector<std::shared_ptr<IPropertyAccessor>> GetSerializableProperties() const 
        {
            std::vector<std::shared_ptr<IPropertyAccessor>> result;
            for (const auto& [name, prop] : m_Properties) 
            {
                if (prop->ShouldSerialize()) 
                {
                    result.push_back(prop);
                }
            }
            return result;
        }

        // 继承支持
        void SetParent(ClassInfo* parent) { m_Parent = parent; }
        ClassInfo* GetParent() const { return m_Parent; }

        // 检查是否继承自指定类
        bool IsDerivedFrom(const std::string& baseClassName) const 
        {
            const ClassInfo* current = this;
            while (current) 
            {
                if (current->GetClassName() == baseClassName) 
                {
                    return true;
                }
                current = current->GetParent();
            }
            return false;
        }

        // 获取所有属性（包括继承的）
        std::map<std::string, std::shared_ptr<IPropertyAccessor>> GetAllProperties() const 
        {
            std::map<std::string, std::shared_ptr<IPropertyAccessor>> allProps = m_Properties;
            if (m_Parent)
            {
                //递归向父类调用
                auto parentProps = m_Parent->GetAllProperties();
                allProps.insert(parentProps.begin(), parentProps.end());
            }
            return allProps;
        }
    private:
        std::string m_ClassName;
        CreateFunc m_CreateFunc = nullptr;
        ClassInfo* m_Parent = nullptr;
        std::map<std::string, std::shared_ptr<IPropertyAccessor>> m_Properties;
    };

} // namespace Reflection