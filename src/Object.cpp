#include "Precompiled.h"

#include "Graphics.h"
#include "Object.h"

Object::Object(const std::string& name) : Resource(name), mSceneNode(NULL), mState(IDLE)
{
}

bool Object::load(World* world, const std::string& meshName, const
	Ogre::Vector3& position)
{
	try
	{
		mWorld = world;

		mEntity = mWorld->createEntity(mName + " entity", meshName);

		mSceneNode = mWorld->createSceneNode();
		mSceneNode->attachObject(mEntity);
		mSceneNode->setPosition(position);
	}
	catch(...)
	{
		return false;
	}

	return true;
}

void Object::unload()
{
}

bool Object::update(float time)
{
	setPosition(
		mWorld->raycastPoint(
			Ogre::Ray(
				getPosition() + Ogre::Vector3(0.0f, 1000.0f, 0.0f),
				Ogre::Vector3::NEGATIVE_UNIT_Y)));

	return true;
}