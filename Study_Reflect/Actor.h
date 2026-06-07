#pragma once
#include <vector>
#include <memory>
#include "Object.h"
#include "ActorComponent.h"
#include "MyReflect/ReflectMarco.h"
#include "SceneComponent.h"

class Actor : public Object
{
    REGISTER_CLASS(Actor)
public:
    PROPERTY(int, Health)
    PROPERTY_RANGE(float, Speed, 0.0f, 100.0f)
    PROPERTY_STRING(Name)
    PROPERTY_OBJECT_PTR(Actor, Parent)
    PROPERTY_UNIQUE_PTR(ActorComponent, RootComponent)
    PROPERTY_VECTOR(int, Inventory)
protected:
    //SceneComponent* RootComponent;
	//std::vector<std::unique_ptr<Actor>> ChildActors;
	//std::vector<std::unique_ptr<ActorComponent>> Components;
    PROPERTY_VECTOR_UNIQUE_PTR(ActorComponent, Components)
};