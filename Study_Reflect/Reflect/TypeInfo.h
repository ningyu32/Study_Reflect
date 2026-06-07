#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <string_view>
#include <vector>

namespace Reflect
{
	enum class TypeKind
	{
		Unknown,
		Bool,
		Int8,
		UInt8,
		Int16,
		UInt16,
		Int32,
		UInt32,
		Int64,
		UInt64,
		Float,
		Double,
		Class
	};

	struct TypeInfo;

	struct PropertyInfo
	{
		std::string_view Name;
		const TypeInfo* Type = nullptr;
		std::size_t Offset = 0;
		std::uint32_t Version = 1;
		std::uint32_t Flags = 0;
		std::function<void*(void*)> GetMutableValue;
		std::function<const void*(const void*)> GetConstValue;

		void* GetValuePtr(void* Instance) const;
		const void* GetValuePtr(const void* Instance) const;
	};

	struct TypeInfo
	{
		std::string_view Name;
		TypeKind Kind = TypeKind::Unknown;
		std::size_t Size = 0;
		std::size_t Alignment = 0;
		std::uint32_t Version = 1;
		std::vector<PropertyInfo> Properties;

		bool IsBasic() const;
		bool IsClass() const;
	};
}
