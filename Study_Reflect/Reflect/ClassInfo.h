#pragma once

#include "Registry.h"

namespace Reflect
{
	template<typename T>
	struct AutoRegister
	{
		explicit AutoRegister(const TypeInfo* Info)
		{
			Registry::Get().Register(std::type_index(typeid(T)), Info);
		}
	};

	template<typename OwnerType, typename PropertyType>
	void RegisterProperty(TypeInfo& OwnerInfo, std::string_view Name, PropertyType OwnerType::* Member, std::uint32_t Version = 1, std::uint32_t Flags = 0)
	{
		PropertyInfo Property;
		Property.Name = Name;
		Property.Type = TypeResolver<PropertyType>::Get();
		Property.Offset = static_cast<std::size_t>(-1);
		Property.Version = Version;
		Property.Flags = Flags;
		Property.GetMutableValue = [Member](void* Instance) -> void*
		{
			return &(static_cast<OwnerType*>(Instance)->*Member);
		};
		Property.GetConstValue = [Member](const void* Instance) -> const void*
		{
			return &(static_cast<const OwnerType*>(Instance)->*Member);
		};
		OwnerInfo.Properties.push_back(Property);
	}
}
