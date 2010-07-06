#ifndef OGRETESTWIDGET_H_
#define OGRETESTWIDGET_H_

#include "OgreWidget.h"
#include "Parser2DS.h"

// ---------------------------------------------------------------------------------------
class OgreTestWidget : public OgreWidget
{
	//Q_OBJECT
public:
	Ogre::SceneNode* mNodoEscena;
	Ogre::SceneNode* mFocusNodo;
	QPoint mPosicionClick;
	Ogre::Vector3 mCamaraUp;
	Ogre::Vector3 mCamaraRight;
	Ogre::Vector3 mCamaraDireccion;

	Ogre::Vector3 mCamaraPosicion;
	Ogre::Quaternion mFocusOrientacion;

	OgreTestWidget(int windID, Ogre::String name, QWidget* parent=0, bool parentHandle=false, Qt::WindowFlags f=0);
	void CargarEscenario2d(std::string ruta);


protected:
	void OnInit();
	void mouseReleaseEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent * event);
	void keyPressEvent ( QKeyEvent * event );
	void mouseDoubleClickEvent ( QMouseEvent * event );

private:
	Parser2DS* mEscena2d;
};

#endif /* OGRETESTWIDGET_H_ */
