#pragma once

#include "Precompiled.h"

class Order
{
public:
	enum Type
	{
		ATTACK,
		MOVE,
		FOLLOW,
		STOP,
		PATROL
	};
protected:
	Type mType;
public:
	Order(Type type) : mType(type)
	{
	}

	Type getType() const
	{
		return mType;
	}
};