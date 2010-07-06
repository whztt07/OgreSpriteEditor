#include "MainWindow.h"
#include "DomModel.h"

MainWindow::MainWindow(
	QWidget *parent
):
QMainWindow(parent),
rutaRecursos("")
{
	ui.setupUi(this);

	mOgreWidget = new OgreTestWidget(0,"OgreWidget",this);
	ui.layoutOgre->addWidget(mOgreWidget);
	//ui.arbolXML->setStyle


	connect( ui.actionCargarRecursos, SIGNAL(triggered()), this, SLOT(cargarRecursos()) );
	connect( ui.actionCargarSprites, SIGNAL(triggered()), this, SLOT(cargarSprites()) );

}

MainWindow::~MainWindow()
{
}

void MainWindow::cargarRecursos()
{
	QSettings settings("toglia3d","SpriteSceneViewer");
	QString rutaRecursosSetting = settings.value("rutaRecursos", QString("/home")).toString();

	QFileDialog openDialog;
	//openDialog.exec();
	QString filtro = tr("*.cfg");
	rutaRecursos = openDialog.getOpenFileName(
		this,
		tr("Abrir"),
		rutaRecursosSetting,
		filtro
	);

	if(!rutaRecursos.isEmpty()){
		settings.setValue("rutaRecursos",rutaRecursos);

		Ogre::ConfigFile cf;
		cf.load(rutaRecursos.toStdString());

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

		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	}

}

void MainWindow::cargarSprites()
{

	QSettings settings("toglia3d","SpriteSceneViewer");
	QString rutaSpritesSetting = settings.value("rutaSprites", QString("/home")).toString();

	QFileDialog openDialog;
	//openDialog.exec();
	QString filtro = tr("*.2ds");
	rutaSprites = openDialog.getOpenFileName(
		this,
		tr("Abrir"),
		rutaSpritesSetting,
		filtro
	);

	if(!rutaSprites.isEmpty()){
		settings.setValue("rutaSprites",rutaSprites);
		mOgreWidget->CargarEscenario2d(rutaSprites.toStdString());

		QFile file(rutaSprites);
		if (file.open(QIODevice::ReadOnly)) {
			QDomDocument document;
			if (document.setContent(&file)) {
				DomModel *newModel = new DomModel(document, this);
				ui.arbolXML->setModel(newModel);
			}
			file.close();
		}
	}
}



void MainWindow::updateOGRE()
{
	QMutexLocker locker(&mutex);
	static bool updateGUI=true;


	Ogre::Root* mRoot = Ogre::Root::getSingletonPtr();
	mRoot->_fireFrameStarted();

	// loop through ogre widgets and update animation
	QList<OgreWidget*> rendlist = this->findChildren<OgreWidget*>();
	foreach (OgreWidget* w, rendlist)
	{

		// update animation for OgreWidget's sceneManager
		if (w->mRenderWindow && w->updatesEnabled())
		{
			// update OgreWidget
			w->update();
			//emit w->paintEvent(new QPaintEvent(w->rect()));
			for (Ogre::SceneManager::MovableObjectIterator mit = w->getSceneManager()->getMovableObjectIterator("Entity");
				mit.hasMoreElements(); mit.moveNext() )
			{
				Ogre::Entity *entity = static_cast<Ogre::Entity*>(mit.peekNextValue());
				if (updateGUI) {
					updateGUI = false;

				}
				// check has skeleton to avoid crash for non animable entities
				if (entity->hasSkeleton())
				{
					for (Ogre::AnimationStateIterator animIt = entity->getAllAnimationStates()->getAnimationStateIterator();
						animIt.hasMoreElements(); animIt.moveNext() )
					{
						Ogre::AnimationState *animState = animIt.peekNextValue();
						if ( animState->getEnabled() )
						{
							//std::cout << entity->getName() << " ZZZZZZZZZZZ " << animState->getAnimationName();
							animState->addTime(w->getRenderWindow()->getBestFPS()/10000);
						}
					}
				}
			}
		}
	}
	mRoot->_fireFrameRenderingQueued();
	mRoot->_fireFrameEnded();
}
