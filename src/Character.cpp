#include "Precompiled.h"

#include "Character.h"

Character::Character(const std::string& name) : Object(name)
{
}

bool Character::update(float time)
{
	return Object::update(time);
}