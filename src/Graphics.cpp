#include "Precompiled.h"

#include "GUI.h"
#include "Graphics.h"
#include "ObjectManager.h"
#include "PlayerManager.h"
#include "WorldManager.h"

Graphics::Graphics() : mActive(true)
{
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
}

Graphics::~Graphics()
{
	if (mInputManager)
	{
		mInputManager->destroyInputObject(mMouse);
		mInputManager->destroyInputObject(mKeyboard);

		OIS::InputManager::destroyInputSystem(mInputManager);
	}

	if (mRoot)
		delete mRoot;
}

Ogre::SceneManager* sceneManager;

bool Graphics::init()
{
	// Root

	mRoot = new (std::nothrow) Ogre::Root("Plugins.cfg", "Ogre.cfg", "Ogre.log");

	if (!mRoot)
		return false;

	// Render System

	Ogre::RenderSystemList* renderSystemList = mRoot->getAvailableRenderers();

	for (Ogre::RenderSystemList::iterator i = renderSystemList->begin(); i != renderSystemList->end(); i++)
	{
		if ((*i)->getName() == "Direct3D9 Rendering Subsystem")
		{
			mRoot->setRenderSystem((*i));

			break;
		}
	}

	if (mRoot->getRenderSystem() == NULL)
	{
		Ogre::RenderSystemList* renderSystemList = mRoot->getAvailableRenderers();

		for (Ogre::RenderSystemList::iterator i = renderSystemList->begin(); i != renderSystemList->end(); i++)
		{
			if ((*i)->getName() == "OpenGL Rendering Subsystem")
			{
				mRoot->setRenderSystem((*i));

				break;
			}
		}
	}

	if (!mRoot->getRenderSystem())
		return false;

	// Root

	mRoot->initialise(false);
	mRoot->addFrameListener(this);

	// Resources

	Ogre::ConfigFile cf;
	cf.load("Resources.cfg");

	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	Ogre::String secName;
	Ogre::ConfigFile::SettingsMultiMap* settings;
	Ogre::ConfigFile::SettingsMultiMap::iterator i;

	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		settings = seci.getNext();

		for (i = settings->begin(); i != settings->end(); i++)
		{
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				i->second, i->first, secName);
		}
	}

	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	// Render Window

	mRenderWindow = mRoot->createRenderWindow("RenderWindow", 1280, 1024, false);
	Ogre::WindowEventUtilities::addWindowEventListener(mRenderWindow, this);

	// OIS

	size_t winHandle;
	mRenderWindow->getCustomAttribute("WINDOW", &winHandle);

	mInputManager = OIS::InputManager::createInputSystem(winHandle);

	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
	mKeyboard->setEventCallback(this);

	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));
	mMouse->setEventCallback(this);

	const OIS::MouseState& ms = mMouse->getMouseState();
	ms.width = 1280;
	ms.height = 1024;

	return true;
}

World* w1;
World* w2;
World* w3;

Object* o1;
Object* o2;
Object* o3;

#include "Character.h"

void Graphics::run()
{
	ObjectManager objectManager;
	objectManager.registerType<Character>("character");

	PlayerManager playerManager;

	WorldManager worldManager;

	w1 = WorldManager::getSingleton().create("world", "earth");
	w1->load("earth.png", "earth", 64);
	w1->getCamera()->setPosition(50, 250, 50);
	w1->getCamera()->lookAt(200, 200, 200);

	mViewport = mRenderWindow->addViewport(w1->getCamera());
	mViewport->setBackgroundColour(w1->getSceneManager()->getFogColour());


	//mRoot->renderOneFrame();

	GUI *gui = new GUI();
	gui->init();
	gui->setVisible(false);

	CEGUI::Listbox* menu = (CEGUI::Listbox*)
		CEGUI::WindowManager::getSingleton().getWindow("menu");

	CEGUI::ListboxTextItem* item;

	item = new CEGUI::ListboxTextItem(" test");
	item->setSelectionBrushImage("TaharezLook", "ListboxSelectionBrush");
	item->setSelectionColours(CEGUI::colour(0.1, 0.3, 0.6));
	menu->addItem(item);

	item = new CEGUI::ListboxTextItem(" test");
	item->setSelectionBrushImage("TaharezLook", "ListboxSelectionBrush");
	item->setSelectionColours(CEGUI::colour(0.1, 0.3, 0.6));
	menu->addItem(item);

	item = new CEGUI::ListboxTextItem(" test");
	item->setSelectionBrushImage("TaharezLook", "ListboxSelectionBrush");
	item->setSelectionColours(CEGUI::colour(0.1, 0.3, 0.6));
	menu->addItem(item);

	//*
	o1 = ObjectManager::getSingleton().create("character", "o1");
	o1->load(w1, "c_yKid_f.mesh", Ogre::Vector3(70, 0, 70));
	//*/

	/*
	Ogre::RenderTexture* ogreTexture =
		mRoot->getRenderSystem()->createRenderTexture(
			"RenderToTexure", 512, 512, Ogre::TEX_TYPE_2D, Ogre::PF_R8G8B8);

	Ogre::Viewport* view = ogreTexture->addViewport(mCamera);
	view->setBackgroundColour(Ogre::ColourValue(0.25f, 0.5f, 0.75f));

	CEGUI::Texture* ceguiTexture =
		GUI::getSingleton().mRenderer->createTexture(
			"RenderToTexure");

	CEGUI::Imageset* imageSet =
		CEGUI::ImagesetManager::getSingleton().createImageset(
			"Imageset", ceguiTexture);

	imageSet->defineImage("Image",
		CEGUI::Point(0.0f, 0.0f),
		CEGUI::Size(ceguiTexture->getWidth(), ceguiTexture->getHeight()),
		CEGUI::Point(0.0f, 0.0f));

	CEGUI::Window* m_image =
		CEGUI::WindowManager::getSingleton().getWindow("world/image");
	m_image->setProperty("Image", "set:Imageset image:Image");
	//*/
/*
	CEGUI::FrameWindow* window = (CEGUI::FrameWindow*)
		CEGUI::WindowManager::getSingleton().getWindow("world");

	window->setAlpha(0.75);
	window->setVisible(false);

	CEGUI::Listbox* list = (CEGUI::Listbox*)
		CEGUI::WindowManager::getSingleton().getWindow("world/list");

	CEGUI::ListboxTextItem* item;
	
	item = new CEGUI::ListboxTextItem(" adfasdf");
	item->setSelectionBrushImage("TaharezLook", "ListboxSelectionBrush");
	list->addItem(item);

	item = new CEGUI::ListboxTextItem(" adafg  df db");
	item->setSelectionBrushImage("TaharezLook", "ListboxSelectionBrush");
	list->addItem(item);

	item = new CEGUI::ListboxTextItem(" sdfg dfsgdf c");
	item->setSelectionBrushImage("TaharezLook", "ListboxSelectionBrush");
	list->addItem(item);
	*/

	/*mRoot->getRenderSystem()->_initRenderTargets();

	mRoot->clearEventTimes();

        mRoot->_fireFrameStarted();
        mRoot-> _updateAllRenderTargets();
        mRoot->_fireFrameEnded();

		Ogre::Vector3 r(0, 0, 0);
		RaycastFromPoint(
			mViewport->getCamera()->getSceneManager(),
			Ogre::Ray(mViewport->getCamera()->getPosition() + Ogre::Vector3(5, 1000, 5), Ogre::Vector3::NEGATIVE_UNIT_Y),
			r);
		mViewport->getCamera()->setPosition(r + Ogre::Vector3(0, 18, 0));

        while(true)
        {
			//Pump messages in all registered RenderWindow windows
			Ogre::WindowEventUtilities::messagePump();

			if (!mRoot->renderOneFrame())
                break;
        }*/
/*
	w1->getSceneManager()->setWorldGeometry("terrain.cfg");


	//mRoot->getRenderSystem()->_initRenderTargets();
	mRoot->getRenderSystem()->_updateAllRenderTargets();

	Ogre::RaySceneQuery* raySceneQuery = w1->getSceneManager()->createRayQuery(Ogre::Ray());
	Ogre::Ray ray(Ogre::Vector3(50, 1000.0f, 50), Ogre::Vector3::NEGATIVE_UNIT_Y);

	raySceneQuery->setRay(ray);

	Ogre::RaySceneQueryResult& raySceneQueryResult = raySceneQuery->execute();
	Ogre::RaySceneQueryResult::iterator itr = raySceneQueryResult.begin();

	for (; itr != raySceneQueryResult.end(); itr++)
		if (itr->movable)
			itr->movable->getParentSceneNode()->showBoundingBox(true);//*/

	//if (itr != raySceneQueryResult.end() && itr->worldFragment)
		//mViewport->getCamera()->setPosition(50, itr->worldFragment->singleIntersection.y, 50);


	mRoot->startRendering();
}

void Graphics::quit()
{
	mActive = false;
}

Ogre::Vector2 m_mouse2DPos;



// Get the mesh information for the given mesh.
// Code found on this forum link: http://www.ogre3d.org/wiki/index.php/RetrieveVertexData
void Graphics::GetMeshInformation(const Ogre::MeshPtr mesh,
                                size_t &vertex_count,
                                Ogre::Vector3* &vertices,
                                size_t &index_count,
                                unsigned long* &indices,
                                const Ogre::Vector3 &position,
                                const Ogre::Quaternion &orient,
                                const Ogre::Vector3 &scale)
{
    bool added_shared = false;
    size_t current_offset = 0;
    size_t shared_offset = 0;
    size_t next_offset = 0;
    size_t index_offset = 0;

    vertex_count = index_count = 0;

    // Calculate how many vertices and indices we're going to need
    for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh( i );

        // We only need to add the shared vertices once
        if(submesh->useSharedVertices)
        {
            if( !added_shared )
            {
                vertex_count += mesh->sharedVertexData->vertexCount;
                added_shared = true;
            }
        }
        else
        {
            vertex_count += submesh->vertexData->vertexCount;
        }

        // Add the indices
        index_count += submesh->indexData->indexCount;
    }


    // Allocate space for the vertices and indices
    vertices = new Ogre::Vector3[vertex_count];
    indices = new unsigned long[index_count];

    added_shared = false;

    // Run through the submeshes again, adding the data into the arrays
    for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);

        Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

        if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
        {
            if(submesh->useSharedVertices)
            {
                added_shared = true;
                shared_offset = current_offset;
            }

            const Ogre::VertexElement* posElem =
                vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

            Ogre::HardwareVertexBufferSharedPtr vbuf =
                vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

            unsigned char* vertex =
                static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

            // There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
            //  as second argument. So make it float, to avoid trouble when Ogre::Real will
            //  be comiled/typedefed as double:
            //      Ogre::Real* pReal;
            float* pReal;

            for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
            {
                posElem->baseVertexPointerToElement(vertex, &pReal);

                Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);

                vertices[current_offset + j] = (orient * (pt * scale)) + position;
            }

            vbuf->unlock();
            next_offset += vertex_data->vertexCount;
        }


        Ogre::IndexData* index_data = submesh->indexData;
        size_t numTris = index_data->indexCount / 3;
        Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

        bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

        unsigned long*  pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);


        size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

        if ( use32bitindexes )
        {
            for ( size_t k = 0; k < numTris*3; ++k)
            {
                indices[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
            }
        }
        else
        {
            for ( size_t k = 0; k < numTris*3; ++k)
            {
                indices[index_offset++] = static_cast<unsigned long>(pShort[k]) +
                    static_cast<unsigned long>(offset);
            }
        }

        ibuf->unlock();
        current_offset = next_offset;
    }
} 

Ogre::Vector3 v[3];
Ogre::Entity *e = NULL;
int ii[3];

// raycast from a point in to the scene.
// returns success or failure.
// on success the point is returned in the result.
bool Graphics::RaycastFromPoint(Ogre::SceneManager* sceneManager, Ogre::Ray& ray,
                                        Ogre::Vector3 &result)
{

// create the ray scene query object
	static Ogre::RaySceneQuery* m_pray_scene_query =
		sceneManager->createRayQuery(Ogre::Ray());

    m_pray_scene_query->setSortByDistance(true); 

    // check we are initialised
    if (m_pray_scene_query != NULL)
	{
        // create a query object
        m_pray_scene_query->setRay(ray);

        // execute the query, returns a vector of hits
        if (m_pray_scene_query->execute().size() <= 0)
		{
            // raycast did not hit an objects bounding box
			result = Ogre::Vector3::ZERO;
            return (false);
        }
    }
      

    // at this point we have raycast to a series of different objects bounding boxes.
    // we need to test these different objects to see which is the first polygon hit.
    // there are some minor optimizations (distance based) that mean we wont have to
    // check all of the objects most of the time, but the worst case scenario is that
    // we need to test every triangle of every object.
    Ogre::Real closest_distance = -1.0f;
    Ogre::Vector3 closest_result;
    Ogre::RaySceneQueryResult &query_result = m_pray_scene_query->getLastResults();
    for (size_t qr_idx = 0; qr_idx < query_result.size(); qr_idx++)
    {
        // stop checking if we have found a raycast hit that is closer
        // than all remaining entities
        if ((closest_distance >= 0.0f) &&
            (closest_distance < query_result[qr_idx].distance))
        {
             break;
        }


        // only check this result if its a hit against an entity
        if ((query_result[qr_idx].movable != NULL) &&
            (query_result[qr_idx].movable->getMovableType().compare("Entity") == 0))
		{
            // get the entity to check
            Ogre::Entity *pentity = static_cast<Ogre::Entity*>(query_result[qr_idx].movable);

			if (pentity->getUserAny().isEmpty() == false)
			{
				if (pentity->getUserAny().getType() == typeid(int))
				{


            // mesh data to retrieve         
            size_t vertex_count;
            size_t index_count;
            Ogre::Vector3 *vertices;
            unsigned long *indices;

            // get the mesh information
         GetMeshInformation(pentity->getMesh(), vertex_count, vertices, index_count, indices,             
                              pentity->getParentNode()->getWorldPosition(),
                              pentity->getParentNode()->getWorldOrientation(),
                              pentity->getParentNode()->getScale());

            // test for hitting individual triangles on the mesh
            bool new_closest_found = false;
            for (int i = 0; i < static_cast<int>(index_count); i += 3)
            {
                // check for a hit against this triangle
				std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(ray, vertices[indices[i]],
                    vertices[indices[i+1]], vertices[indices[i+2]], true, false);

                // if it was a hit check if its the closest
                if (hit.first)
                {
                    if ((closest_distance < 0.0f) ||
                        (hit.second < closest_distance))
                    {
                        // this is the closest so far, save it off
                        closest_distance = hit.second;
                        new_closest_found = true;

						e = pentity;
						v[0] = vertices[indices[i]];
						v[1] = vertices[indices[i+1]];
						v[2] = vertices[indices[i+2]];
						ii[0] = indices[i];
						ii[1] = indices[i+1];
						ii[2] = indices[i+2];
                    }
                }
            }

         // free the verticies and indicies memory
            delete[] vertices;
            delete[] indices;

            // if we found a new closest raycast for this object, update the
            // closest_result before moving on to the next object.
            if (new_closest_found)
            {
                closest_result = ray.getPoint(closest_distance);               
            }
		}
	}
        }       
    }

    // return the result
    if (closest_distance >= 0.0f)
    {
        // raycast success
        result = closest_result;

		e->getParentSceneNode()->showBoundingBox(true);
    }


	return true;
}

bool Graphics::frameStarted(const Ogre::FrameEvent& evt)
{
	ObjectManager::getSingleton().update(evt.timeSinceLastFrame);
	PlayerManager::getSingleton().update(evt.timeSinceLastFrame);
	WorldManager::getSingleton().update(evt.timeSinceLastFrame);

	mKeyboard->capture();
	mMouse->capture();

	if (mKeyboard->isKeyDown(OIS::KC_ESCAPE))
		quit();


	Ogre::Vector3 r = w1->raycastPoint(w1->getCamera()->getCameraToViewportRay(m_mouse2DPos.x, m_mouse2DPos.y));

	static Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().create("red", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	mat->getTechnique(0)->getPass(0)->setAmbient(1, 0, 0);
	mat->getTechnique(0)->getPass(0)->setDiffuse(1, 0, 0, 1);
	mat->getTechnique(0)->getPass(0)->setPolygonMode(Ogre::PM_POINTS);
	mat->getTechnique(0)->getPass(0)->setPointSize(5.0);

	{
	//*
		static Ogre::ManualObject* m = w1->getSceneManager()->createManualObject("p");

		m->clear();
		m->begin("", Ogre::RenderOperation::OT_TRIANGLE_LIST);
		m->position(v[0] + Ogre::Vector3(0, 0.1, 0));
		m->position(v[1] + Ogre::Vector3(0, 0.1, 0));
		m->position(v[2] + Ogre::Vector3(0, 0.1, 0));
		m->end();

		static Ogre::SceneNode* sn = NULL;
		
		if (!sn)
		{
			sn = w1->getSceneManager()->getRootSceneNode()->createChildSceneNode();
			sn->attachObject(m);
		}
	}

	{
		static Ogre::ManualObject* m = w1->getSceneManager()->createManualObject("pa");

		m->clear();
		m->begin("red", Ogre::RenderOperation::OT_POINT_LIST);
		m->position(r + Ogre::Vector3(0, 0.2, 0));
		m->end();


		static Ogre::SceneNode* sn = NULL;
		
		if (!sn)
		{
			sn = w1->getSceneManager()->getRootSceneNode()->createChildSceneNode();
			sn->attachObject(m);
		}
	}
	//*/

	//*
	{
		float speed = 50;

		if (mKeyboard->isKeyDown(OIS::KC_W))
			mViewport->getCamera()->moveRelative(Ogre::Vector3::NEGATIVE_UNIT_Z * speed * evt.timeSinceLastFrame);

		if (mKeyboard->isKeyDown(OIS::KC_S))
			mViewport->getCamera()->moveRelative(Ogre::Vector3::UNIT_Z * speed * evt.timeSinceLastFrame);

		if (mKeyboard->isKeyDown(OIS::KC_A))
			mViewport->getCamera()->moveRelative(Ogre::Vector3::NEGATIVE_UNIT_X * speed * evt.timeSinceLastFrame);

		if (mKeyboard->isKeyDown(OIS::KC_D))
			mViewport->getCamera()->moveRelative(Ogre::Vector3::UNIT_X * speed * evt.timeSinceLastFrame);

		if (mKeyboard->isKeyDown(OIS::KC_Q))
			mViewport->getCamera()->moveRelative(Ogre::Vector3::UNIT_Y * speed * evt.timeSinceLastFrame);

		if (mKeyboard->isKeyDown(OIS::KC_Z))
			mViewport->getCamera()->moveRelative(Ogre::Vector3::NEGATIVE_UNIT_Y * speed * evt.timeSinceLastFrame);
	}

	{
	static Ogre::Entity* e = w1->createEntity("rat", "c_yBegger.mesh");
	static Ogre::SceneNode* sn = NULL;
	if (!sn)
	{
		sn = w1->createSceneNode();
		sn->attachObject(e);
		sn->setPosition(50, 100, 70);
		sn->setScale(0.05, 0.05, 0.05);
	}
	static Ogre::AnimationState* as = e->getAnimationState("walk");
	as->setEnabled(true);
	as->setLoop(true);
	as->addTime(evt.timeSinceLastFrame);
	sn->setOrientation(Ogre::Quaternion(Ogre::Radian(Ogre::Degree(225)), Ogre::Vector3::UNIT_Y));
	}

	

	return mActive;
}

void Graphics::windowClosed(Ogre::RenderWindow* rw)
{
	quit();
}

bool Graphics::keyPressed(const OIS::KeyEvent& arg)
{
	CEGUI::System::getSingleton().injectKeyDown(arg.key);

	return true;
}

bool Graphics::keyReleased(const OIS::KeyEvent& arg)
{
	CEGUI::System::getSingleton().injectKeyUp(arg.key);

	return true;
}

bool Graphics::mouseMoved(const OIS::MouseEvent& arg)
{
	m_mouse2DPos.x = Ogre::Real(arg.state.X.abs) / GUI::getSingleton().mRenderer->getWidth();
	m_mouse2DPos.y = Ogre::Real(arg.state.Y.abs) / GUI::getSingleton().mRenderer->getHeight();

	if (mMouse->getMouseState().buttonDown(OIS::MB_Middle))
	{
		mViewport->getCamera()->yaw(Ogre::Degree(arg.state.X.rel * -0.15f));
		mViewport->getCamera()->pitch(Ogre::Degree(arg.state.Y.rel * -0.15f));
	}

	Ogre::Vector3 result;

	RaycastFromPoint(
		sceneManager,
		mViewport->getCamera()->getCameraToViewportRay(m_mouse2DPos.x, m_mouse2DPos.y),
		result);


	CEGUI::System::getSingleton().injectMouseMove(
		arg.state.X.rel, arg.state.Y.rel);

	return true;
}

CEGUI::MouseButton convertOISMouseButtonToCegui(int buttonID)
{
    switch (buttonID)
    {
		case 0: return CEGUI::LeftButton;
		case 1: return CEGUI::RightButton;
		case 2:	return CEGUI::MiddleButton;
    }

	return CEGUI::LeftButton;
}

bool Graphics::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	/*
	if (e)
	{
		Ogre::VertexData* vertex_data = e->getMesh()->getSubMesh(1)->vertexData;

		const Ogre::VertexElement* posElem =
			vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

		Ogre::HardwareVertexBufferSharedPtr vbuf =
			vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

		unsigned char* vertex =
			static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_NORMAL));

		float* pReal[4];

		posElem->baseVertexPointerToElement(vertex + vbuf->getVertexSize() * ii[0], &pReal[0]);
		posElem->baseVertexPointerToElement(vertex + vbuf->getVertexSize() * ii[1], &pReal[1]);
		posElem->baseVertexPointerToElement(vertex + vbuf->getVertexSize() * ii[2], &pReal[2]);

		if (pReal[0][2] == pReal[1][2])
		{
			posElem->baseVertexPointerToElement(vertex + vbuf->getVertexSize() * (ii[2] - 1), &pReal[3]);

			pReal[0][1] += 1;
			pReal[1][1] += 1;
			pReal[2][1] += 1;

			pReal[3][1] += 1;
		}
		else
		{
			posElem->baseVertexPointerToElement(vertex + vbuf->getVertexSize() * (ii[1] + 1), &pReal[3]);

			pReal[0][1] += 1;
			pReal[1][1] += 1;
			pReal[2][1] += 1;
			pReal[3][1] += 1;
		}

		vbuf->unlock();
	}
	*/

	CEGUI::System::getSingleton().injectMouseButtonDown(
		convertOISMouseButtonToCegui(id));

	return true;
}

bool Graphics::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{	
	CEGUI::System::getSingleton().injectMouseButtonUp(
		convertOISMouseButtonToCegui(id));

	return true;
}