#pragma once
#include <cstdint>
#include <string>

namespace Reflection
{
	class ClassInfo;
}
using G_ID = uint64_t;

class Object
{
public:
	Object() = default;
	virtual ~Object() = default;
	static Object* NewObject();
	static Reflection::ClassInfo ObjectClassInfo;
	virtual Reflection::ClassInfo* GetObjectClassInfo();
	virtual std::string GetObjectClassName() const;

	// 序列化支持
	virtual void Serialize(class ISerializer* serializer) {}
	virtual void Deserialize(class ISerializer* serializer) {}

	std::string GetName() const { return m_Name; }
	void SetName(std::string& InName) { m_Name = InName; }

	G_ID GetID() const { return m_Id; }
	void SetID(G_ID InId) { m_Id = InId; }
protected:
	G_ID m_Id = 0;
	std::string m_Name;
};
