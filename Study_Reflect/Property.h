#pragma once
#include <string>

template<typename ClassType, typename PropertyType>
class Property
{
public:
	std::string PropertyName;
	PropertyType ClassType::* member;
	Property* Next;
};