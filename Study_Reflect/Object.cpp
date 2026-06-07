#include "Object.h"
#include "MyReflect/ClassInfo.h"

Object* Object::NewObject()
{
	return new Object();
}
Reflection::ClassInfo Object::ObjectClassInfo(std::string("Object"), &NewObject);

Reflection::ClassInfo* Object::GetObjectClassInfo()
{
	return &ObjectClassInfo;
}

std::string Object::GetObjectClassName() const
{
	return "Object";
}
