#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>

class Object;

namespace Reflection {
    class ClassInfo;
    class IPropertyAccessor;

    // 序列化器接口
    class ISerializer {
    public:
        virtual ~ISerializer() = default;

        // 序列化对象
        virtual std::string Serialize(Object* obj) = 0;
        virtual bool Deserialize(const std::string& data, Object* obj) = 0;

        // 序列化存档
        virtual std::string SerializeArchive(const std::map<std::string, Object*>& archive) = 0;
        virtual bool DeserializeArchive(const std::string& data, std::map<std::string, Object*>& archive) = 0;

        // 获取格式名称
        virtual std::string GetFormatName() const = 0;
    };

} // namespace Reflection