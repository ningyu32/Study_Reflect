#pragma once

#include "ClassInfo.h"

namespace Reflect::Detail
{
	template<typename T>
	struct BasicTypeStorage
	{
		static TypeInfo Info;
		static AutoRegister<T> Register;
	};
}

#define REFLECT_DEFINE_BASIC_TYPE(Type, Name, KindValue) \
	template<> Reflect::TypeInfo Reflect::Detail::BasicTypeStorage<Type>::Info = { Name, KindValue, sizeof(Type), alignof(Type), 1, {} }; \
	template<> Reflect::AutoRegister<Type> Reflect::Detail::BasicTypeStorage<Type>::Register(&Reflect::Detail::BasicTypeStorage<Type>::Info); \
	template<> struct Reflect::TypeResolver<Type> \
	{ \
		static const Reflect::TypeInfo* Get() \
		{ \
			(void)Reflect::Detail::BasicTypeStorage<Type>::Register; \
			return &Reflect::Detail::BasicTypeStorage<Type>::Info; \
		} \
	}
