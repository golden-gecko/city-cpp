#pragma once

#include "Precompiled.h"

#include "ResourceManager.h"
#include "World.h"

class WorldManager : public ResourceManager<World>, public Ogre::Singleton<WorldManager>
{
public:
	WorldManager();

	void update(float time);
};