#pragma once
#include <map>
#include <string>
#include <memory>
#include "ClassInfo.h"

namespace Reflection {

    // 反射注册管理器
    class Register {
    public:
        static Register* Get() 
        {
            static Register instance;
            return &instance;
        }

        // 注册类信息
        void RegisterClass(ClassInfo* classInfo) 
        {
            if (classInfo && m_Classes.find(classInfo->GetClassName()) == m_Classes.end()) 
            {
                m_Classes[classInfo->GetClassName()] = classInfo;
            }
        }

        // 获取类信息
        ClassInfo* GetClass(const std::string& className) 
        {
            auto it = m_Classes.find(className);
            return it != m_Classes.end() ? it->second : nullptr;
        }

        const ClassInfo* GetClass(const std::string& className) const 
        {
            auto it = m_Classes.find(className);
            return it != m_Classes.end() ? it->second : nullptr;
        }

        // 获取所有已注册的类
        const std::map<std::string, ClassInfo*>& GetAllClasses() const 
        {
            return m_Classes;
        }

        // 通过名称创建对象
        Object* CreateObject(const std::string& className)
        {
            ClassInfo* info = GetClass(className);
            if (info && info->GetCreateFunc()) {
                return info->GetCreateFunc()();
            }
            return nullptr;
        }

        // 模板版本的创建
        template<typename T>
        T* CreateObject() 
        {
            static_assert(std::is_base_of_v<Object, T>, "T must be derived from Object");
            ClassInfo* info = GetClass(T::StaticClassName);
            if (info && info->GetCreateFunc()) 
            {
                return static_cast<T*>(info->GetCreateFunc()());
            }
            return nullptr;
        }
    private:
        Register() = default;
        ~Register() = default;
        Register(const Register&) = delete;
        Register& operator=(const Register&) = delete;

        std::map<std::string, ClassInfo*> m_Classes;
    };
} // namespace Reflection