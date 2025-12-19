#include "Precompiled.h"

#include "Graphics.h"
#include "World.h"

World::World(const std::string& name) : Resource(name), mSceneManager(NULL),
	mCamera(NULL), mRaySceneQuery(NULL), mSize(0), mScale(0), mSceneNodes(NULL),
	mEntities(NULL)
{
}

extern Ogre::SceneManager* sceneManager;

bool World::load(const std::string& heightMapName, const std::string&
	materialName, size_t scale)
{
	unload();

	try
	{
		sceneManager = mSceneManager = Graphics::getSingleton().createSceneManager(
			mName + " - scene manager");
		mSceneManager->setShadowFarDistance(512.0f);
		mSceneManager->setShadowUseInfiniteFarPlane(false);
		mSceneManager->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);
		mSceneManager->setShadowTextureSize(512);
		mSceneManager->setShadowTextureSelfShadow(false);
		mSceneManager->setFog(Ogre::FOG_LINEAR,
			Ogre::ColourValue(0.25f, 0.5f, 0.75f), 0.0f, 512.0f, 1024.0f);

		// Camera

		mCamera = mSceneManager->createCamera(mName + " - camera");
		mCamera->setNearClipDistance(0.5f);
		mCamera->setFarClipDistance(1024.0f);
		mCamera->setPosition(100.0f, 100.0f, 100.0f);
		mCamera->lookAt(0, 0, 0);

		// Ray Scene Query

		mRaySceneQuery = mSceneManager->createRayQuery(Ogre::Ray());
		mRaySceneQuery->setSortByDistance(true);

		// Terrain

		Ogre::Image image;

		image.load(
			heightMapName,
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		const Ogre::uchar* data = image.getData();
		mSize = image.getWidth();
		mScale = scale;

		Ogre::SceneNode* sn = mSceneManager->getRootSceneNode();

		mSceneNodes = new Ogre::SceneNode**[(mSize - 1) / mScale];
		for (unsigned int i = 0; i < (mSize - 1) / mScale; i++)
			mSceneNodes[i] = new Ogre::SceneNode*[(mSize - 1) / mScale];

		mEntities = new Ogre::Entity**[(mSize - 1) / mScale];
		for (unsigned int i = 0; i < (mSize - 1) / mScale; i++)
			mEntities[i] = new Ogre::Entity*[(mSize - 1) / mScale];

		if (image.getWidth() == image.getHeight() && image.getFormat() == Ogre::PF_L8)
		{
			for (size_t field_z = 0; field_z < (mSize - 1) / mScale; field_z++)
			{
				for (size_t field_x = 0; field_x < (mSize - 1) / mScale; field_x++)
				{
					std::stringstream s;

					s << mName << " - " << field_x << ":" << field_z;

					Ogre::ManualObject* m = mSceneManager->createManualObject(s.str());

					m->begin(materialName, Ogre::RenderOperation::OT_TRIANGLE_LIST);

					for (size_t z = field_z * mScale; z < ((field_z + 1) * mScale) + 1; z++)
						for (size_t x = field_x * mScale; x < ((field_x + 1) * mScale) + 1; x++)
						{
							m->position(x * 10, data[z * mSize + x] * 0.5, z * 10);
							m->textureCoord(x / (float)(mSize - 1), z / (float)(mSize - 1));
							m->textureCoord(x / (float)(mScale * 0.25f), z / (float)(mScale * 0.25f));
						}

					for (size_t z = 0; z < mScale; z++)
						for (size_t x = 0; x < mScale; x++)
						{
							m->index(z * (mScale + 1) + x);
							m->index((z + 1) * (mScale + 1) + x);
							m->index(z * (mScale + 1) + x + 1);

							m->index((z + 1) * (mScale + 1) + x);
							m->index((z + 1) * (mScale + 1) + x + 1);
							m->index(z * (mScale + 1) + x + 1);
						}

					m->end();

					m->convertToMesh(s.str());
					mSceneManager->destroyManualObject(m);

					mEntities[field_x][field_z] = mSceneManager->createEntity(s.str(), s.str());
					mEntities[field_x][field_z]->setUserAny(Ogre::Any(0));

					mSceneNodes[field_x][field_z] = mSceneManager->getRootSceneNode()->createChildSceneNode();
					mSceneNodes[field_x][field_z]->attachObject(mEntities[field_x][field_z]);
				}
			}
		}
	}
	catch (...)
	{
		return false;
	}

	return true;
}

Object* World::raycastObject(const Ogre::Ray& ray) const
{
	mRaySceneQuery->setRay(ray);

	Ogre::RaySceneQueryResult& result = mRaySceneQuery->execute();

	for (Ogre::RaySceneQueryResult::iterator itr = result.begin(); itr != result.end(); itr++)
	{
		if (itr->movable->getUserAny().getType() == typeid(Object*))
		{
			return Ogre::any_cast<Object*>(itr->movable->getUserAny());
		}
	}

	return NULL;
}

Ogre::Vector3 World::raycastPoint(const Ogre::Ray& ray) const
{
	mRaySceneQuery->setRay(ray);

	Ogre::Vector3 result;
	Ogre::Vector3 v[3];
	Ogre::Entity *e = NULL;
	int ii[3];

	if (mRaySceneQuery->execute().size())
	{

    // at this point we have raycast to a series of different objects bounding boxes.
    // we need to test these different objects to see which is the first polygon hit.
    // there are some minor optimizations (distance based) that mean we wont have to
    // check all of the objects most of the time, but the worst case scenario is that
    // we need to test every triangle of every object.
    Ogre::Real closest_distance = -1.0f;
    Ogre::Vector3 closest_result;
    Ogre::RaySceneQueryResult &query_result = mRaySceneQuery->getLastResults();
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
			Graphics::getSingleton().GetMeshInformation(pentity->getMesh(), vertex_count, vertices, index_count, indices,             
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

		return result;

//		e->getParentSceneNode()->showBoundingBox(true);
    }


	}

	return Ogre::Vector3(-1.0f);
}

void World::unload()
{
	Graphics::getSingleton().destroySceneManager(mSceneManager);

	if (mSceneNodes)
	{
		for (unsigned int i = 0; i < (mSize - 1) / mScale; i++)
			delete[] mSceneNodes[i];

		delete[] mSceneNodes;
	}

	if (mEntities)
	{
		for (unsigned int i = 0; i < (mSize - 1) / mScale; i++)
			delete[] mEntities[i];

		delete[] mEntities;
	}

	mSceneManager = NULL;
	mCamera = NULL;
	mSize = 0;
	mScale = 0;
	mSceneNodes = NULL;
	mEntities = NULL;
}

bool World::update(float time)
{
	return true;
}