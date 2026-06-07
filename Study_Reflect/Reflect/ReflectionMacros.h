#pragma once

#include "ClassInfo.h"

#define REFLECT_CLASS(Type) \
public: \
	using ThisClass = Type; \
	static Reflect::TypeInfo& StaticMutableReflectType() \
	{ \
		static Reflect::TypeInfo Info = { #Type, Reflect::TypeKind::Class, sizeof(Type), alignof(Type), 1, {} }; \
		static Reflect::AutoRegister<Type> Register(&Info); \
		(void)Register; \
		return Info; \
	} \
	static const Reflect::TypeInfo* StaticReflectType() \
	{ \
		return &StaticMutableReflectType(); \
	} \
	virtual const Reflect::TypeInfo* GetReflectType() const \
	{ \
		return Type::StaticReflectType(); \
	} \
private:

#define GENERATED_REFLECT_BODY(Type) REFLECT_CLASS(Type)

#define REGISTER_CLASS_PROPERTY(ClassType, PropertyType, PropertyName) \
	struct AutoRegisterProperty_##PropertyName \
	{ \
		AutoRegisterProperty_##PropertyName() \
		{ \
			Reflect::RegisterProperty<ClassType, PropertyType>(ClassType::StaticMutableReflectType(), #PropertyName, &ClassType::PropertyName); \
		} \
	}; \
	inline static AutoRegisterProperty_##PropertyName AutoRegisterPropertyInstance_##PropertyName

#define REFLECT_PROPERTY(ClassType, PropertyType, PropertyName) \
	REGISTER_CLASS_PROPERTY(ClassType, PropertyType, PropertyName)

#define REGISTER_CLASS_PROPERTY_EX(ClassType, PropertyType, PropertyName, Version, Flags) \
	struct AutoRegisterProperty_##PropertyName \
	{ \
		AutoRegisterProperty_##PropertyName() \
		{ \
			Reflect::RegisterProperty<ClassType, PropertyType>(ClassType::StaticMutableReflectType(), #PropertyName, &ClassType::PropertyName, Version, Flags); \
		} \
	}; \
	inline static AutoRegisterProperty_##PropertyName AutoRegisterPropertyInstance_##PropertyName

#define REFLECT_PROPERTY_EX(ClassType, PropertyType, PropertyName, Version, Flags) \
	REGISTER_CLASS_PROPERTY_EX(ClassType, PropertyType, PropertyName, Version, Flags)
