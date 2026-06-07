#pragma once
#include "Object.h"
#include "MyReflect/ReflectMarco.h"

class ActorComponent : public Object
{
	REGISTER_CLASS(ActorComponent)
public:
	ActorComponent() {}
	virtual ~ActorComponent(){}
};