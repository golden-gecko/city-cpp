#pragma once

#include "Precompiled.h"

#include "ResourceManager.h"
#include "Player.h"

class PlayerManager : public ResourceManager<Player>, public Ogre::Singleton<PlayerManager>
{
public:
	PlayerManager();

	void update(float time);
};