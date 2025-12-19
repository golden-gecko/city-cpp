#pragma once

#include "Precompiled.h"

#include "Resource.h"

class Object;

class World : public Resource
{
protected:
	Ogre::SceneManager*		mSceneManager;
	Ogre::Camera*			mCamera;
	Ogre::RaySceneQuery*	mRaySceneQuery;
	size_t					mSize;
	size_t					mScale;
	Ogre::SceneNode***		mSceneNodes;
	Ogre::Entity***			mEntities;
public:
	World(const std::string& name);

	Ogre::Entity* createEntity(const std::string& entityName, const std::string&
		meshName) const
	{
		return mSceneManager->createEntity(entityName, meshName);
	}

	Ogre::SceneNode* createSceneNode() const
	{
		return mSceneManager->getRootSceneNode()->createChildSceneNode();
	}

	Ogre::Camera* getCamera() const
	{
		return mCamera;
	}

	Ogre::SceneManager* getSceneManager() const
	{
		return mSceneManager;
	}

	bool load(const std::string& heightMapName, const std::string& materialName,
		size_t scale);

	Object* raycastObject(const Ogre::Ray& ray) const;
	
	Ogre::Vector3 raycastPoint(const Ogre::Ray& ray) const;

	void unload();

	virtual bool update(float time);
};