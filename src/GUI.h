#pragma once

#include "Precompiled.h"

class GUI : public Ogre::Singleton<GUI>
{
public:
	CEGUI::OgreCEGUIRenderer*	mRenderer;
	CEGUI::System*				mSystem;

	Ogre::SceneManager*			mSceneManager;
	Ogre::Camera*				mCamera;
	Ogre::Viewport*				mViewport;
public:
	~GUI();

	bool init();

	void setVisible(bool visible)
	{
		mSystem->getGUISheet()->setVisible(visible);
	}
};