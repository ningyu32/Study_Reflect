#pragma once
#include "ActorComponent.h"
#include "MyReflect/ReflectMarco.h"

class SceneComponent : public ActorComponent
{
	REGISTER_CLASS_DERIVED(SceneComponent, ActorComponent)
public:
};