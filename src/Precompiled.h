#pragma once

#include <CEGUI/CEGUI.h>
#include <OgreCEGUIRenderer.h>

#include <Ogre.h>
#include <OIS/OIS.h>

template <typename T> T* Ogre::Singleton<T>::ms_Singleton = NULL;

#include <tinyxml.h>