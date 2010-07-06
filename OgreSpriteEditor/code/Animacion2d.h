#ifndef ANIMATION2D_H
#define ANIMATION2D_H

#include <QtCore/QPropertyAnimation>
#include <QtCore/QEasingCurve>
#include <QtCore/QPointF>
#include <QtCore/QDebug>

#include "GUICanvas.h"

class myAnimacion : public QVariantAnimation{
public:
	myAnimacion() : QVariantAnimation(NULL)
	{
	}

	void updateCurrentValue( const QVariant & value ) {
	}
};

class Animacion2d {
public:
	enum TIPO_BUCLE {
		NINGUNO,
		DIRECTO,
		ZIGZAG
	};

private:
	QPropertyAnimation mAnimation;
	int mDuracion;
	int mTiempo;
	bool mPlay;
	TIPO_BUCLE mBucle;
	short mDireccion;

public:
	Animacion2d(
		int duracion,
		QPointF& posicionA,
		QPointF& posicionB,
		QEasingCurve::Type easingCurve,
		TIPO_BUCLE bucle,
		bool esconder
	):
		mAnimation(),
		mDuracion(duracion),
		mTiempo(0),
		mPlay(false),
		mBucle(bucle),
		mDireccion(1)
	{
		mAnimation.setDuration(duracion);
		mAnimation.setStartValue(posicionA);
		mAnimation.setEndValue(posicionB);
		mAnimation.setEasingCurve(QEasingCurve(easingCurve));
	}

	~Animacion2d(){
	}


	void actualizar(unsigned int deltaTiempo, Position& posicion){
		if (mPlay){

			mTiempo = mTiempo +mDireccion*deltaTiempo;

			if(mTiempo>=mDuracion){
				switch (mBucle){
					case NINGUNO:
						mPlay = false;
						mTiempo = mDuracion;
						break;
					case DIRECTO:
						mTiempo=0;
						break;
					case ZIGZAG:
						mTiempo = mDuracion-(mTiempo-mDuracion);
						mDireccion = -1*mDireccion;
						break;
				}
			}

			if(mTiempo<=0){
				mTiempo = abs(mTiempo);
				mDireccion = -1*mDireccion;
			}

			mAnimation.setCurrentTime(mTiempo);
			posicion.data.abs.left = mAnimation.currentValue().toPointF().x();
			posicion.data.abs.top = mAnimation.currentValue().toPointF().y();
		}
	}

	void iniciar(){
		mTiempo = 0;
		mPlay = true;
	}

};

#endif // ANIMATION_H
