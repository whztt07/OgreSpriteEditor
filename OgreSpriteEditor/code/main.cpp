#include "Application.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{

    Application* app = new Application(argc, argv);

    //Initialize Ogre and the OpenGL Render system.
    Ogre::Root* mRoot = new Ogre::Root();

    mRoot->loadPlugin("RenderSystem_GL");
	mRoot->loadPlugin("Plugin_CgProgramManager");
	mRoot->loadPlugin("Plugin_ParticleFX");

	Ogre::RenderSystemList renderers = mRoot->getAvailableRenderers();
	Ogre::RenderSystem * mRenderSystem = *(renderers.begin());
	mRoot->setRenderSystem(mRenderSystem);
	mRoot->initialise(false);

    // load resources
    Ogre::ConfigFile cf;
    cf.load("recursos.cfg");

    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                                archName, typeName, secName);
        }
    }

    // show main widget
    app->showMainWidget();

    // main loop
    int loop = app->exec();

    // clean up
    delete mRoot;
    delete app;
    return loop;
}
