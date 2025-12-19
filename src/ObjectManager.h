#pragma once

#include "Precompiled.h"

#include "ResourceManager.h"
#include "Object.h"

class ObjectManager : public ResourceManager<Object>, public Ogre::Singleton<ObjectManager>
{
public:
	ObjectManager();

	void update(float time);
};