#pragma once

#include "Precompiled.h"

#include "Resource.h"
#include "World.h"

class Object : public Resource
{
public:
	enum State
	{
		IDLE		= 0,
		ATTACKING	= 1,
		MOVING		= 2,
		PATROLING	= 4
	};
protected:
	World*				mWorld;
	Ogre::SceneNode*	mSceneNode;
	Ogre::Entity*		mEntity;
	int					mState;

	void addState(unsigned int state)
	{
		mState |= state;
	}

	void removeState(unsigned int state)
	{
		mState &= ~state;
	}

	void setState(unsigned int state)
	{
		mState = state;
	}
public:
	Object(const std::string& name);

	Ogre::Vector3 getPosition() const
	{
		return mSceneNode->getPosition();
	}

	unsigned int getState() const
	{
		return mState;
	}

	bool getState(unsigned int state) const
	{
		return mState & state ? true : false;
	}

	bool load(World* world, const std::string& meshName, const Ogre::Vector3&
		position);

	void setOrientation(const Ogre::Quaternion& orientation) const
	{
		mSceneNode->setOrientation(orientation);
	}

	void setPosition(const Ogre::Vector3& position) const
	{
		mSceneNode->setPosition(position);
	}

	void setScale(const Ogre::Vector3& scale) const
	{
		mSceneNode->setScale(scale);
	}

	void unload();

	virtual bool update(float time);
};