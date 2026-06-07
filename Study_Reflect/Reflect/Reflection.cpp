#include "Reflection.h"

namespace Reflect
{
	void* PropertyInfo::GetValuePtr(void* Instance) const
	{
		if (GetMutableValue)
		{
			return GetMutableValue(Instance);
		}
		return static_cast<unsigned char*>(Instance) + Offset;
	}

	const void* PropertyInfo::GetValuePtr(const void* Instance) const
	{
		if (GetConstValue)
		{
			return GetConstValue(Instance);
		}
		return static_cast<const unsigned char*>(Instance) + Offset;
	}

	bool TypeInfo::IsBasic() const
	{
		return Kind >= TypeKind::Bool && Kind <= TypeKind::Double;
	}

	bool TypeInfo::IsClass() const
	{
		return Kind == TypeKind::Class;
	}

	Registry& Registry::Get()
	{
		static Registry Instance;
		return Instance;
	}

	const TypeInfo* Registry::Find(std::string_view Name) const
	{
		const auto It = TypesByName.find(Name);
		return It != TypesByName.end() ? It->second : nullptr;
	}

	const TypeInfo* Registry::Find(std::type_index Type) const
	{
		const auto It = TypesByIndex.find(Type);
		return It != TypesByIndex.end() ? It->second : nullptr;
	}

	void Registry::Register(std::type_index Type, const TypeInfo* Info)
	{
		TypesByIndex[Type] = Info;
		TypesByName[Info->Name] = Info;
	}
}

REFLECT_DEFINE_BASIC_TYPE(bool, "bool", Reflect::TypeKind::Bool);
REFLECT_DEFINE_BASIC_TYPE(std::int8_t, "int8", Reflect::TypeKind::Int8);
REFLECT_DEFINE_BASIC_TYPE(std::uint8_t, "uint8", Reflect::TypeKind::UInt8);
REFLECT_DEFINE_BASIC_TYPE(std::int16_t, "int16", Reflect::TypeKind::Int16);
REFLECT_DEFINE_BASIC_TYPE(std::uint16_t, "uint16", Reflect::TypeKind::UInt16);
REFLECT_DEFINE_BASIC_TYPE(std::int32_t, "int32", Reflect::TypeKind::Int32);
REFLECT_DEFINE_BASIC_TYPE(std::uint32_t, "uint32", Reflect::TypeKind::UInt32);
REFLECT_DEFINE_BASIC_TYPE(std::int64_t, "int64", Reflect::TypeKind::Int64);
REFLECT_DEFINE_BASIC_TYPE(std::uint64_t, "uint64", Reflect::TypeKind::UInt64);
REFLECT_DEFINE_BASIC_TYPE(float, "float", Reflect::TypeKind::Float);
REFLECT_DEFINE_BASIC_TYPE(double, "double", Reflect::TypeKind::Double);
