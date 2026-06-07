#pragma once
#include <map>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include "ISerializer.h"

namespace Reflection {

    // ¥Êµµπ‹¿Ì
    class Archive {
    public:
        static Archive& Get() {
            static Archive instance;
            return instance;
        }

        void SetSerializer(std::unique_ptr<ISerializer> serializer) {
            m_Serializer = std::move(serializer);
        }

        bool Save(const std::string& filePath, const std::map<std::string, Object*>& objects) {
            if (!m_Serializer) return false;

            std::string data = m_Serializer->SerializeArchive(objects);
            std::ofstream file(filePath);
            if (!file.is_open()) return false;

            file << data;
            return true;
        }

        bool Load(const std::string& filePath, std::map<std::string, Object*>& objects) {
            if (!m_Serializer) return false;

            std::ifstream file(filePath);
            if (!file.is_open()) return false;

            std::stringstream buffer;
            buffer << file.rdbuf();
            return m_Serializer->DeserializeArchive(buffer.str(), objects);
        }

        template<typename T>
        T* CreateAndLoad(const std::string& filePath) {
            T* obj = new T();
            if (Load(filePath, obj)) {
                return obj;
            }
            delete obj;
            return nullptr;
        }

    private:
        std::unique_ptr<ISerializer> m_Serializer;
    };

} // namespace Reflection