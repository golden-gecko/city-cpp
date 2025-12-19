#include "Precompiled.h"
#include "Graphics.h"
#include "GUI.h"
#include "World.h"

GUI::~GUI()
{
	if (mSystem)
		delete mSystem;

	if (mRenderer)
		delete mRenderer;
}

bool GUI::init()
{
	// Create Scene Manager, Camera and Viewport.

	mSceneManager = Graphics::getSingleton().createSceneManager();
	mCamera = mSceneManager->createCamera("GuiCamera");
	mViewport = Graphics::getSingleton().addViewport(mCamera, 1);
	mViewport->setClearEveryFrame(false);

	// Create Ogre CEGUI Renderer.

	mRenderer = new (std::nothrow) CEGUI::OgreCEGUIRenderer(
		Graphics::getSingleton().getRenderWindow(),
		Ogre::RENDER_QUEUE_OVERLAY,
		false,
		3000,
		mSceneManager);

	if (!mRenderer)
		return false;

	// Create and setup CEGUI System.

	mSystem = new CEGUI::System(
		mRenderer, NULL, NULL, NULL, "Gui.config");

	if (!mSystem)
		return false;

	CEGUI::System::getSingleton().setDefaultMouseCursor(
		"TaharezLook", "MouseArrow");

	// Set mouse position.

	CEGUI::MouseCursor::getSingleton().setPosition(
		CEGUI::Point(CEGUI::Vector2(0.0f, 0.0f)));

	return true;
}