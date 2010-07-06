#ifndef OgreWidget_H
#define OgreWidget_H

#include <QMainWindow>
#include <Ogre.h>
#include <QFileInfo>
#include <OgreRenderWindow.h>
#include <OgreRoot.h>
#include <OgreStringConverter.h>
#include <QSettings>
#include <QDockWidget>
#include <QTreeView>
#include <QSplitter>
#include <QVBoxLayout>
#include <QListView>
#include <QListWidget>
#if defined(Q_WS_X11)
#include <QX11Info>
#endif
#include <QEvent>
#include <QX11EmbedWidget>
#include <QPainter>
#include "QDebug"
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QStringList>
#include <QApplication>
#include <QtGui>
#include <QtCore>

#if defined( _MSC_VER )
typedef unsigned long HANDLE;
#define showerror(e)  throw std::exception(e)
#elif defined( __GNUC__ )
typedef size_t HANDLE;
#define showerror(e) throw std::runtime_error(e)
#endif



// ---------------------------------------------------------------------------------------
class OgreWidget : public QWidget
{
	Q_OBJECT

public:
	OgreWidget(int windID, Ogre::String name, QWidget* parent=0, bool parentHandle=false, Qt::WindowFlags f=0);
	~OgreWidget();

	Ogre::Camera* getCamera();
	Ogre::SceneManager* getSceneManager();
	int  getWindowID() { return mIndex; }
	Ogre::String  getWindowName() { return mName; }
	//void update();
	virtual void OnInit()=0;
	Ogre::RenderWindow* createOgreRenderWindow(QObject* parent, int width, int height, const Ogre::String& name, bool parentHandle);
	Ogre::RenderWindow* getRenderWindow() const;

protected:
	QPaintEngine *paintEngine() const;
	void paintEvent(QPaintEvent* evt);
	void resizeEvent(QResizeEvent* evt);

protected:
	Ogre::Viewport* mViewport;
	Ogre::Camera* mCamera;
	Ogre::SceneManager* mSceneMgr;
	Ogre::String mName;
	int mIndex;
	bool mParentHandle;
	QPoint curPos;

public:
	Ogre::RenderWindow* mRenderWindow;
	Ogre::Root* mOgreRoot;
	bool mIsInitialised;
};

#endif // OgreWidget_H
