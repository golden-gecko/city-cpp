#pragma once

#include "Precompiled.h"

class Graphics :
	public Ogre::Singleton<Graphics>,
	public Ogre::WindowEventListener,
	public Ogre::FrameListener,
	public OIS::KeyListener,
	public OIS::MouseListener
{
public:
	Ogre::Root*			mRoot;
	Ogre::RenderWindow*	mRenderWindow;
	Ogre::Viewport*		mViewport;

	OIS::InputManager*	mInputManager;
	OIS::Keyboard*		mKeyboard;
	OIS::Mouse*			mMouse;

	bool				mActive;
public:
	Graphics();
	~Graphics();

	bool init();
	void run();
	void quit();

	Ogre::RenderWindow* getRenderWindow()
	{
		return mRenderWindow;
	}

	Ogre::Viewport* addViewport(Ogre::Camera* cam, int ZOrder, float
		left = 0.0f, float top = 0.0f, float width = 1.0f, float
		height = 1.0f)
	{
		return mRenderWindow->addViewport(
			cam, ZOrder, left, top, width, height);
	}

	Ogre::SceneManager* createSceneManager(
		const std::string& instanceName = Ogre::StringUtil::BLANK,
		Ogre::SceneType typeMask = Ogre::ST_EXTERIOR_CLOSE)
	{
		return mRoot->createSceneManager(typeMask, instanceName);
	}

	void destroySceneManager(Ogre::SceneManager* sceneManager)
	{
		if (sceneManager)
			mRoot->destroySceneManager(sceneManager);
	}

	// Inherited from Ogre::FrameListener.

	bool frameStarted(const Ogre::FrameEvent& evt);

	// Inherited from Ogre::WindowEventListener.

	void windowClosed(Ogre::RenderWindow* rw);

	// Inherited from OIS::KeyListener.

	bool keyPressed(const OIS::KeyEvent& arg);
	bool keyReleased(const OIS::KeyEvent& arg);

	// Inherited from OIS::MouseListener.

	bool mouseMoved(const OIS::MouseEvent& arg);
	bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);

	bool RaycastFromPoint(Ogre::SceneManager* sceneManager, Ogre::Ray& ray,
		Ogre::Vector3 &result);
void GetMeshInformation(const Ogre::MeshPtr mesh,
                                size_t &vertex_count,
                                Ogre::Vector3* &vertices,
                                size_t &index_count,
                                unsigned long* &indices,
                                const Ogre::Vector3 &position,
                                const Ogre::Quaternion &orient,
                                const Ogre::Vector3 &scale);
};