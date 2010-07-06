#include "OgreTestWidget.h"

OgreTestWidget::OgreTestWidget(int windID, Ogre::String name, QWidget* parent, bool parentHandle,Qt::WindowFlags f)
:OgreWidget(windID, name, parent, parentHandle, f)
{
}

// ----------------------------------------------------------
void OgreTestWidget::OnInit()
{
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	mSceneMgr = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC,"principal");
	// create camera
	mCamera = mSceneMgr->createCamera("PlayerCam_EntityPanel" + mName);
	mCamera->setPosition(Ogre::Vector3(0, 0, -40));
	mCamera->lookAt(Ogre::Vector3(0,0,0));
	mCamera->setAutoAspectRatio(true);
	mCamera->setNearClipDistance(0.1);
	// create viewport
	mViewport = mRenderWindow->addViewport(mCamera);
	mViewport->setBackgroundColour(Ogre::ColourValue(0.1, 0.1, 0.1, 1));
	mCamera->setAspectRatio((mViewport->getActualWidth()) /
		(mViewport->getActualHeight()));

	mViewport->_updateDimensions();

	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.1,0.1,0.1));

	Ogre::Light* luz = mSceneMgr->createLight("Light0");
	luz->setType(Ogre::Light::LT_SPOTLIGHT);
	luz->setPosition(0,100,-100);
	luz->setSpotlightRange(Ogre::Degree(10), Ogre::Degree(90));
	Ogre::Vector3 direccion = -luz->getPosition();
	direccion.normalise();
	luz->setDirection(direccion);
	luz->setDiffuseColour(1,1,1);
	luz->setSpecularColour(0.25, 0.25, 0.25);


	mFocusNodo = mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3::ZERO);
	mFocusNodo->attachObject(mCamera);

	/*
	mNodoEscena = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	Ogre::Entity* entity = mSceneMgr->createEntity("pelota1", "flechaShape.mesh");
	mNodoEscena->attachObject(entity);
	*/
}

// ---------------------------------------------------------------------------------------
void OgreTestWidget::mouseReleaseEvent(QMouseEvent *event)
{
}
// ---------------------------------------------------------------------------------------
void OgreTestWidget::mousePressEvent(QMouseEvent *event)
{
	mPosicionClick = event->pos();

	mCamaraUp = mCamera->getRight();
	mCamaraRight = mCamera->getUp();
	mCamaraDireccion = mCamera->getDirection();

	mCamaraPosicion = mCamera->getPosition();
	mFocusOrientacion = mFocusNodo->getOrientation();
}

// ---------------------------------------------------------------------------------------
void OgreTestWidget::mouseMoveEvent(QMouseEvent *event)
{
	QPoint posicionRelativa = mPosicionClick - event->pos();

	Ogre::Quaternion rotacionX = Ogre::Quaternion(
		Ogre::Degree(posicionRelativa.x()/2),
		Ogre::Vector3::UNIT_Y
	);


	Ogre::Quaternion rotacionY = Ogre::Quaternion(
		Ogre::Degree(posicionRelativa.y()/2),
		mCamera->getDerivedRight()
	);

	Ogre::Vector3 zoom = (-posicionRelativa.y()/20.0)*mCamaraDireccion;
	Ogre::Vector3 offsetX = mCamaraUp*(posicionRelativa.x()/70.0);
	Ogre::Vector3 offsetY = mCamaraRight*(-posicionRelativa.y()/70.0);

	switch(event->buttons()){
		case Qt::LeftButton:
			mFocusNodo->setOrientation(
				mFocusOrientacion * mFocusNodo->_getDerivedOrientation().Inverse() *
				rotacionY * mFocusNodo->_getDerivedOrientation()
			);

			mFocusNodo->setOrientation(
				mFocusNodo->getOrientation() * mFocusNodo->_getDerivedOrientation().Inverse() *
				rotacionX * mFocusNodo->_getDerivedOrientation()
			);
			break;

		case Qt::RightButton:
			mCamera->setPosition(mCamaraPosicion+zoom);
			break;

		case Qt::MidButton:
			mCamera->setPosition(mCamaraPosicion+offsetX+offsetY);
			break;
	}

}

void OgreTestWidget::wheelEvent(QWheelEvent * event){
	int numDegrees = event->delta() / 8;
	int numSteps = numDegrees / 15;

	Ogre::Vector3 zoom = (numSteps)*mCamera->getDirection();
	mCamera->setPosition(mCamera->getPosition()+zoom);
	qDebug() << numSteps ;
}
// ---------------------------------------------------------------------------------------
void OgreTestWidget::keyPressEvent ( QKeyEvent * event )
{

}
// ---------------------------------------------------------------------------------------
void OgreTestWidget::mouseDoubleClickEvent ( QMouseEvent * event )
{
}


void OgreTestWidget::CargarEscenario2d(std::string ruta){
	mEscena2d = new Parser2DS(
		ruta,
		mSceneMgr,
		mCamera
	);
	mEscena2d->actualizar(1);
}

