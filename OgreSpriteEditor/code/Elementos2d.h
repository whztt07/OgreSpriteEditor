#ifndef TIPOS2D_H_
#define TIPOS2D_H_

#include <map>
#include <string>
#include <OgreVector2.h>
#include <OgreVector3.h>
#include <OgreColourValue.h>
#include <OgreAnimation.h>

#include "GUICanvas.h"
#include "GUIBox.h"
#include "GUILabel.h"
#include "Animacion2d.h"

#include <QtCore/QPoint>
#include <QtCore/QEasingCurve>
#include <QtCore/QPropertyAnimation>

struct Fuente {
	std::string mNombre;
	unsigned int mAltura;
	Fuente()
	:
		mNombre(""),
		mAltura(0)
	{
	}

	Fuente(
		std::string nombre,
		unsigned int altura
	):
		mNombre(nombre),
		mAltura(altura)
	{
	}
};

struct Cuadro : public GUIElements::Box {
	std::string mNombre;

	typedef QMap<std::string, std::string> Texturas;
	Texturas mTexturas;

	typedef QMap<std::string, Animacion2d*> AnimacionMap;
	AnimacionMap mAnimaciones;

	Position mPosicionActual;
	Animacion2d* mAnimacionActual;

	Cuadro(
		const Ogre::String& nombre,
		Ogre::Vector2& dimension,
		Ogre::Vector2& posicion
	):
		GUIElements::Box(dimension.x, dimension.y),
		mNombre(nombre),
		mPosicionActual(posicion.x,posicion.y)
	{
		setPosition(mPosicionActual);
		mAnimacionActual = NULL;
	}

	void agregarTextura(const std::string& nombre, const std::string& valor){
		Texturas::iterator i = mTexturas.find(nombre);
		if(i!=mTexturas.end()){
			throw std::runtime_error("ERROR Cuadro::agregarTextura; textura duplicada.");
		}
		if(mTexturas.size()==0){
			setBackground(Fill(valor));
		}
		mTexturas[nombre] = valor;
	}

	void setTextura(const std::string& nombre){
		Texturas::iterator i = mTexturas.find(nombre);
		if(i==mTexturas.end()){
			throw std::runtime_error("ERROR Cuadro::setTextura; textura no existe.");
		}
		setBackground(Fill(i.value()));
	}


	void agregarAnimacion(const std::string& nombre, Animacion2d* animacion){
		mAnimaciones.insert(nombre, animacion);
		mAnimacionActual = animacion;
	}

	void actualizar(unsigned int deltaTiempo){
		if (mAnimacionActual){
			mAnimacionActual->actualizar(deltaTiempo,mPosicionActual);
			setPosition(mPosicionActual);
		}
	}

	void iniciarAnimacion(const std::string& nombre){
		AnimacionMap::iterator animacion = mAnimaciones.find(nombre);
		if(animacion == mAnimaciones.end()){
			throw std::runtime_error("ERROR Cuadro::iniciarAnimacion; animacion no existe.");
		}
		mAnimacionActual = animacion.value();
		mAnimacionActual->iniciar();
	}

};

struct Etiqueta : public GUIElements::Label {
	std::string mNombre;

	typedef std::map<std::string, Animacion2d*> AnimacionMap;
	typedef std::pair<std::string, Animacion2d*> AnimacionPar;
	AnimacionMap mAnimaciones;


	Etiqueta(
		const std::string& nombre,
		const Ogre::Vector2& dimension,
		const Ogre::Vector2 posicion,
		const Fuente& fuente,
		const Ogre::ColourValue& color,
		const Ogre::ColourValue& sombra,
		RelativePosition alineacion,
		const Ogre::String& texto
	):
		GUIElements::Label(fuente.mNombre,fuente.mAltura,texto),
		mNombre(nombre)
	{
		setSize((int)dimension.x,(int)dimension.y);
		setPosition(Position(alineacion, posicion.x,posicion.y));
		setFontColor(color);
		setFontShadowColor(sombra);
	};
};


#endif
