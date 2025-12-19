#include "Precompiled.h"

#include "Graphics.h"

Graphics graphics;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)

#else

int main(int argc, char** argv)

#endif

{
	if (graphics.init())
	{
		graphics.run();
	}

	return 0;
}