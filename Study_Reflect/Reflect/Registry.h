#pragma once

#include "TypeInfo.h"

#include <string_view>
#include <type_traits>
#include <typeindex>
#include <unordered_map>

namespace Reflect
{
	class Registry
	{
	public:
		static Registry& Get();

		const TypeInfo* Find(std::string_view Name) const;
		const TypeInfo* Find(std::type_index Type) const;
		void Register(std::type_index Type, const TypeInfo* Info);

	private:
		std::unordered_map<std::string_view, const TypeInfo*> TypesByName;
		std::unordered_map<std::type_index, const TypeInfo*> TypesByIndex;
	};

	namespace Detail
	{
		template<typename T, typename = void>
		struct HasStaticReflectType : std::false_type
		{
		};

		template<typename T>
		struct HasStaticReflectType<T, std::void_t<decltype(T::StaticReflectType())>> : std::true_type
		{
		};
	}

	template<typename T>
	struct TypeResolver
	{
		static const TypeInfo* Get()
		{
			using RawType = std::remove_cv_t<std::remove_reference_t<T>>;
			if constexpr (Detail::HasStaticReflectType<RawType>::value)
			{
				return RawType::StaticReflectType();
			}
			else
			{
				return Registry::Get().Find(std::type_index(typeid(RawType)));
			}
		}
	};

	template<typename T>
	const TypeInfo* GetType()
	{
		return TypeResolver<T>::Get();
	}
}
