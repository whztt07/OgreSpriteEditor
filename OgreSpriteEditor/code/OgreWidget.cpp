#include "OgreWidget.h"

// ----------------------------------------------------------
// Base Ogre Widget
// ----------------------------------------------------------
OgreWidget::OgreWidget(int windID, Ogre::String name, QWidget* parent, bool parentHandle,Qt::WindowFlags f)
:QWidget(parent,  f | Qt::MSWindowsOwnDC), mParentHandle(parentHandle)
, mIndex(windID)
, mName(name)
, mIsInitialised(false)
, mRenderWindow(0)
{
	setObjectName(name.c_str());

	QPalette palette = this->palette();
	palette.setColor(QPalette::Active, QPalette::WindowText, Qt::black);
	palette.setColor(QPalette::Active, QPalette::Window, Qt::black);
	setPalette(palette);

	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);
	setFocusPolicy(Qt::StrongFocus);

	mOgreRoot = Ogre::Root::getSingletonPtr();
	// Confirm Ogre::Root created
	if (!mOgreRoot)
	{
		showerror("No instance of Root exists, cannot create Ogre View");
	}
	if (!mOgreRoot->isInitialised())
	{
		showerror("Root hasn't been initialised!");
	}
	setAcceptDrops(true);
}

// ---------------------------------------------------------------------------------------
OgreWidget::~OgreWidget()
{
}

// ---------------------------------------------------------------------------------------
Ogre::Camera* OgreWidget::getCamera()
{
	return mCamera;
}
// ---------------------------------------------------------------------------------------
Ogre::SceneManager* OgreWidget::getSceneManager()
{
	return  mSceneMgr;
}
// ---------------------------------------------------------------------------------------
Ogre::RenderWindow* OgreWidget::getRenderWindow() const
{
	return mRenderWindow;
}

// ---------------------------------------------------------------------------------------
QPaintEngine *OgreWidget::paintEngine() const
{
	return QWidget::paintEngine();
}
// ---------------------------------------------------------------------------------------
Ogre::RenderWindow* OgreWidget::createOgreRenderWindow(QObject* parent, int width, int height, const Ogre::String& name, bool parentHandle)
{
	Ogre::NameValuePairList params;
	QWidget *q_parent = dynamic_cast <QWidget *> (parent);

	Ogre::String windowHandleType = mParentHandle ? "parentWindowHandle" : "externalWindowHandle";
	params[windowHandleType] = Ogre::StringConverter::toString((HANDLE)(q_parent->winId()));

	Ogre::RenderWindow* r = mOgreRoot->createRenderWindow(name, width, height,false, &params);

	// it's a parent window take over the ogre created window.
	/*
    if (mParentHandle)
    {
        WId window_id;
        mRenderWindow->getCustomAttribute("WINDOW", &window_id);
        QWidget::create(window_id);
    }
	 */
	return r;
}

// ---------------------------------------------------------------------------------------
void OgreWidget::paintEvent(QPaintEvent* evt)
{
	if(mRenderWindow == NULL)
	{
		mRenderWindow = createOgreRenderWindow(this, width(), height(), mName,false);
		OnInit();
	}
	else
	{
		if (updatesEnabled()){
			//nodo->translate(Ogre::Vector3(0.001,0,0));
			mRenderWindow->update();
		}
	}
}

// ---------------------------------------------------------------------------------------

void OgreWidget::resizeEvent(QResizeEvent* evt)
{
	if(mRenderWindow == NULL) return;

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
	mRenderWindow->resize(evt->size().width(), evt->size().height());
#endif
	mRenderWindow->windowMovedOrResized();
	for(int ct = 0; ct < mRenderWindow->getNumViewports(); ++ct)
	{
		Ogre::Viewport* pViewport = mRenderWindow->getViewport(ct);
		Ogre::Camera* pCamera = pViewport->getCamera();
		pCamera->setAspectRatio(static_cast<Ogre::Real>(pViewport->getActualWidth()) / static_cast<Ogre::Real>(pViewport->getActualHeight()));
		pViewport->_updateDimensions();
	}
}

// ---------------------------------------------------------------------------------------
