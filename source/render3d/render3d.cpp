#include "render3d.h"
#include <OGRE-Next/Ogre.h>

namespace tt
{
	void c_render3d::go_render3d() const
	{
		// Initialize Root with no plugin configuration
		Ogre::Root* root = new Ogre::Root();

		//// Load the RenderSystem_GL3Plus plugin explicitly
		//static Ogre::GL3PlusPlugin* gl3PlusPlugin = new Ogre::GL3PlusPlugin();
		//root->installPlugin(gl3PlusPlugin);

		//// Choose the OpenGL 3+ RenderSystem
		//Ogre::RenderSystem* renderSystem = root->getRenderSystemByName("OpenGL 3+ Rendering Subsystem");
		//if (!renderSystem)
		//{
		//	throw std::runtime_error("Failed to locate RenderSystem_GL3Plus.");
		//}

		//root->setRenderSystem(renderSystem);

		//// Initialize Root and create a window
		//root->initialise(false); // Don't auto-create a window
		//Ogre::NameValuePairList params;
		//params["FSAA"] = "4"; // Anti-aliasing
		//params["vsync"] = "true";
		//Ogre::RenderWindow* window = root->createRenderWindow("OgreNext Window", 1280, 720, false, &params);

		// Clean up
		delete root;
	}
}
