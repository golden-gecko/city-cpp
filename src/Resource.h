#pragma once

#include "Precompiled.h"

class Resource
{
protected:
	std::string mName;
public:
	Resource() : mName("")
	{
	}

	Resource(const std::string& name) : mName(name)
	{
	}

	std::string getName()
	{
		return mName;
	}

	void setName(const std::string& name)
	{
		mName = name;
	}
};