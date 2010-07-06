#pragma once
#ifndef TOGLIA3D_2DSPARSER_H
#define TOGLIA3D_2DSPARSER_H

#include <QtXml/QDomDocument>
#include <map>
#include <vector>

#include "GUICanvas.h"
#include "GUILabel.h"
#include "GUIBox.h"

#include "Elementos2d.h"


class Parser2DS {
private:
	GUICanvas * mGUI;
	std::vector<std::string> mTexturas;
	std::vector<FontFaceDefinition> mFuentes;

	typedef std::map<std::string, Cuadro> CuadroMap;
	typedef std::pair<std::string, Cuadro> CuadroPar;
	typedef std::map<std::string, Etiqueta> EtiquetaMap;
	typedef std::pair<std::string, Etiqueta> EtiquetaPar;

	typedef std::vector<Cuadro*> CuadroVector;
	typedef std::vector<Etiqueta*> EtiquetaVector;

	CuadroMap mCuadroMap;
	CuadroVector mCuadroVector;
	EtiquetaMap mEtiquetaMap;
	EtiquetaVector mEtiquetaVector;

	bool agregarTextura(std::string textura);
	bool buscarAlturaFuente(FontFaceDefinition& fuente, unsigned int altura);
	bool agregarFuente(std::string& fuente, unsigned int altura);

	Ogre::Vector2 cargarDimension(QDomElement& elemento);
	Ogre::Vector2 cargarPosicion(QDomElement& elemento);

	Ogre::ColourValue cargarColor(QDomElement& elemento);
	Ogre::ColourValue cargarSombra(QDomElement& elemento);
	RelativePosition cargarAlineacion(QDomElement& elemento);
	std::string cargarTexto(QDomElement& elemento);
	Fuente cargarFuente(QDomElement& elemento);

	void cargarTexturas(QDomElement& elemento, Cuadro* cuadro);
	void cargarAnimaciones(QDomElement& elemento, Cuadro* cuadro);
	void cargarCuadro(QDomElement& elemento);
	void cargarEtiqueta(QDomElement& elemento);

public:
	Parser2DS(const std::string&, Ogre::SceneManager*, Ogre::Camera*);
	~Parser2DS();

	inline Cuadro* getCuadro(const std::string& nombre);
	inline Etiqueta* getEtiqueta(const std::string& nombre);
	inline void actualizar(unsigned int deltaTiempo);
};

inline Cuadro* Parser2DS::getCuadro(const std::string& nombre){
	return &(mCuadroMap.find(nombre)->second);
}

inline Etiqueta* Parser2DS::getEtiqueta(const std::string& nombre){
	return &(mEtiquetaMap.find(nombre)->second);
}

inline void Parser2DS::actualizar(unsigned int deltaTiempo){
	mGUI->update();
}

#endif 
