#include "Parser2DS.h"

#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QDebug>
#include <QtXml/QDomDocument>

#include "Elementos2d.h"
#include "Animacion2d.h"

Parser2DS::Parser2DS(const std::string& ruta, Ogre::SceneManager* sceneManager, Ogre::Camera* camara) {

	QDomDocument documento("toglia3dGUI");
	QFile archivo(ruta.c_str());
	if(archivo.open(QIODevice::ReadOnly | QIODevice::Text)){
		Ogre::LogManager::getSingletonPtr()->logMessage(
			"Archivo Panel XML : " + 
			ruta +
			" encontrado."
		);
		if(!documento.setContent(&archivo)){
			archivo.close();
			throw std::runtime_error(
				"ERROR Parser2DS::Parser2DS; Xml: " +
				ruta +
				" no pudo ser asignado."
			);
		}
	}else{
		throw std::runtime_error(
			"ERROR Parser2DS::Parser2DS; Archivo: " +
			ruta +
			" no fue encontrado."
		);
	}
	//-------------------------------------------------------------
	//-------------------------------------------------------------
	//-------------------------------------------------------------

	QDomElement root = documento.documentElement();
	if (root.tagName() != "panel"){
		throw std::runtime_error(
			"ERROR Parser2DS::Parser2DS; Archivo: " +
			ruta +
			" posee formato incorrecto."
		);
	}
	//-------------------------------------------------------------
	//-------------------------------------------------------------
	//-------------------------------------------------------------

	mCuadroVector.clear();
	mEtiquetaVector.clear();

	QDomElement elemento = root.firstChildElement("elemento");
	for(; !elemento.isNull(); elemento=elemento.nextSiblingElement()){
		if (elemento.attribute("tipo")=="box"){
			cargarCuadro(elemento);
		}else if (elemento.attribute("tipo")=="label"){
			cargarEtiqueta(elemento);
		}
	}

	archivo.close();

	try{
		Ogre::uint zOrder = root.attribute("zOrder").toUInt();
		mGUI = new GUICanvas(camara, sceneManager, zOrder, mTexturas, mFuentes);
		Ogre::LogManager::getSingleton().logMessage(
			"Panel "+
			ruta +
			" creado exitosamente."
		);
	}catch(std::exception * e){
		throw std::runtime_error(
			"ERROR Parser2DS::Parser2DS;" +
			Ogre::StringConverter::toString(e->what())
		);
	}

	CuadroVector::iterator i;
	CuadroVector::iterator inicioCuadros = mCuadroVector.begin();
	CuadroVector::iterator finCuadros =  mCuadroVector.end();

	for(i=inicioCuadros; i!=finCuadros; ++i){
		mGUI->addElement(static_cast<GUIElements::Box*>(*i));
	}

	EtiquetaVector::iterator j;
	EtiquetaVector::iterator inicioEtiquetas = mEtiquetaVector.begin();
	EtiquetaVector::iterator finEtiquetas =  mEtiquetaVector.end();
	for(j=inicioEtiquetas; j!=finEtiquetas; ++j){
		mGUI->addElement(dynamic_cast<GUIElements::Label*>(*j));
	}

	Ogre::LogManager::getSingleton().logMessage(
		"Panel terminó de cargar todos los elementos del archivo "+
		ruta +
		" exitosamente."
	);


}

Parser2DS::~Parser2DS(){
	mGUI->clear();
	delete mGUI;
}

bool Parser2DS::agregarTextura(std::string textura){
	std::vector<std::string>::iterator i;
	std::vector<std::string>::iterator inicio = mTexturas.begin();
	std::vector<std::string>::iterator fin = mTexturas.end();

	for(i = inicio; i!=fin; ++i){
		if ((*i).compare(textura) == 0){
			return false;
		}
	}
	mTexturas.push_back(textura);
	Ogre::LogManager::getSingleton().logMessage(
		"Se agregó textura " + textura + "."
	);
	return true;
}

bool Parser2DS::buscarAlturaFuente(FontFaceDefinition& fuente, unsigned int altura){
	std::vector<Ogre::uint>::iterator i;
	std::vector<Ogre::uint>::iterator inicio = fuente.sizes.begin();
	std::vector<Ogre::uint>::iterator fin = fuente.sizes.end();
	for (i = inicio; i != fin; ++i){
		if (*i == altura){
			return true;
		}
	}
	return false;
}

bool Parser2DS::agregarFuente(std::string& fuente, unsigned int altura){
	std::vector<FontFaceDefinition>::iterator i;
	std::vector<FontFaceDefinition>::iterator inicio = mFuentes.begin();
	std::vector<FontFaceDefinition>::iterator fin = mFuentes.end();

	for(i = inicio; i!=fin; ++i){

		if (!buscarAlturaFuente(*i,altura)){
			i->addSize(altura);
			Ogre::LogManager::getSingleton().logMessage(
				"Se agregó altura "+Ogre::StringConverter::toString(altura)+" a fuente "+fuente+"."
			);
		}
	}
	FontFaceDefinition aux(fuente);
	aux.addSize(altura);
	mFuentes.push_back(aux);
	Ogre::LogManager::getSingleton().logMessage(
		"Se agregó fuente " + fuente + " "+Ogre::StringConverter::toString(altura)+"."
	);
	return true;
}

Ogre::Vector2 Parser2DS::cargarDimension(QDomElement& elemento){
	Ogre::Vector2 dimension(Ogre::Vector2::ZERO);

	if(!elemento.isNull()){
		dimension.x = Ogre::StringConverter::parseInt(
			elemento.attribute("ancho").toStdString()
		);
		dimension.y = Ogre::StringConverter::parseInt(
			elemento.attribute("alto").toStdString()
		);
	}else{
		throw std::runtime_error("ERROR Parser2DS::cargarDimension; dimensión no existe.");
	}


	return dimension;
}

Ogre::Vector2 Parser2DS::cargarPosicion(QDomElement& elemento){
	Ogre::Vector2 posicion(Ogre::Vector2::ZERO);

	if(!elemento.isNull()){
		posicion.x = Ogre::StringConverter::parseInt(
			elemento.attribute("x").toStdString()
		);
		posicion.y = Ogre::StringConverter::parseInt(
			elemento.attribute("y").toStdString()
		);
	}else{
		throw std::runtime_error("ERROR Parser2DS::cargarPosicion; posición no existe.");
	}

	return posicion;
}




Ogre::ColourValue Parser2DS::cargarColor(QDomElement& elemento){
	std::string color("");

	if(!elemento.isNull()){
		color = elemento.text().toStdString();
	}else{
		throw std::runtime_error("ERROR Parser2DS::cargarColor; color no existe.");
	}

	return parseHexColor(color);
}

Ogre::ColourValue Parser2DS::cargarSombra(QDomElement& elemento){
	std::string color("");

	if(!elemento.isNull()){
		color = elemento.text().toStdString();
	}else{
		throw std::runtime_error("ERROR Parser2DS::cargarSombra; sombra no existe.");
	}

	return parseHexColor(color);
}

RelativePosition Parser2DS::cargarAlineacion(QDomElement& elemento){
	std::string alineacion("");

	if(!elemento.isNull()){
		alineacion = elemento.text().toStdString();
		if(alineacion == "izquierda"){
			return Left;
		}else if(alineacion == "derecha"){
			return Right;
		}else if(alineacion == "centro"){
			return Center;
		}else if(alineacion == "superiorIzquierda"){
			return TopLeft;
		}else if(alineacion == "superiorCentro"){
			return TopCenter;
		}else if(alineacion == "superiorDerecha"){
			return TopRight;
		}else if(alineacion == "inferiorIzquierda"){
			return BottomLeft;
		}else if(alineacion == "inferiorCentro"){
			return BottomCenter;
		}else if(alineacion == "inferiorDerecha"){
			return BottomRight;
		}
		throw std::runtime_error("ERROR Parser2DS::cargarAlineacion; alineacion no existe.");
	}else{
		throw std::runtime_error("ERROR Parser2DS::cargarAlineacion; alineacion no existe.");
	}

	return Left;
}

std::string Parser2DS::cargarTexto(QDomElement& elemento){
	std::string texto("");

	if(!elemento.isNull()){
		texto = elemento.text().toStdString();
	}else{
		throw std::runtime_error("ERROR Parser2DS::cargarTexto; texto no existe.");
	}

	return texto;
}

Fuente Parser2DS::cargarFuente(QDomElement& elemento){
	Fuente fuente;

	if(!elemento.isNull()){
		fuente.mNombre = elemento.attribute("nombre").toStdString();
		fuente.mAltura = Ogre::StringConverter::parseUnsignedInt(
			elemento.attribute("altura").toStdString()
		);
		agregarFuente(fuente.mNombre, fuente.mAltura);
	}else{
		throw std::runtime_error("ERROR Parser2DS::cargarFuente; fuente no existe.");
	}

	return fuente;
}

void Parser2DS::cargarTexturas(QDomElement& elemento, Cuadro* cuadro){

	if(!elemento.isNull()){
		for(;
			!elemento.isNull();
			elemento=elemento.nextSiblingElement()
		){
			std::string archivo = elemento.attribute("archivo").toStdString();
			cuadro->agregarTextura(
				elemento.attribute("nombre").toStdString(),
				archivo
			);
			agregarTextura(archivo);
		}
	}else{
		throw std::runtime_error("ERROR Parser2DS::cargarTextura; textura no existe.");
	}

}


void Parser2DS::cargarAnimaciones(QDomElement& elemento, Cuadro* cuadro){

	if (!elemento.isNull()){
		for(;!elemento.isNull();elemento=elemento.nextSiblingElement()){

			std::string nombre = elemento.attribute("nombre").toStdString();

			bool ok = false;
			unsigned int duracion = elemento.attribute("duracion").toUInt(&ok);
			if(!ok){
				throw std::runtime_error("ERROR Parser2DS::cargarAnimacion; duración inválida.");
			}

			Animacion2d::TIPO_BUCLE bucle = Animacion2d::NINGUNO;
			if (elemento.attribute("bucle")=="directo"){
				bucle = Animacion2d::DIRECTO;
			}else if(elemento.attribute("bucle")=="zigzag") {
				bucle = Animacion2d::ZIGZAG;
			}


			bool esconder = false;
			if (elemento.attribute("esconder")=="true"){
				esconder = true;
			}else{
				esconder = false;
			}

			float xA = elemento.attribute("xA").toFloat(&ok);
			if(!ok){
				throw std::runtime_error("ERROR Parser2DS::cargarAnimacion; posición xA inválida.");
			}
			float yA = elemento.attribute("yA").toFloat(&ok);
			if(!ok){
				throw std::runtime_error("ERROR Parser2DS::cargarAnimacion; posición yA inválida.");
			}
			QPointF puntoA(xA,yA);

			float xB = elemento.attribute("xB").toFloat(&ok);
			if(!ok){
				throw std::runtime_error("ERROR Parser2DS::cargarAnimacion; posición xB inválida.");
			}
			float yB = elemento.attribute("yB").toFloat(&ok);
			if(!ok){
				throw std::runtime_error("ERROR Parser2DS::cargarAnimacion; posición yB inválida.");
			}
			QPointF puntoB(xB,yB);

			unsigned int interpolacion = elemento.attribute("interpolacion").toUInt(&ok);
			if(!ok){
				throw std::runtime_error("ERROR Parser2DS::cargarAnimacion; interpolación inválida.");
			}

			if(interpolacion>QEasingCurve::NCurveTypes){
				throw std::runtime_error(
					"ERROR Parser2DS::cargarAnimacion; interpolación inválida debe ser mayor a 0 menor a "+
					Ogre::StringConverter::toString(QEasingCurve::NCurveTypes)+"."
				);
			}

			Animacion2d* animacion = new Animacion2d(
				duracion,
				puntoA,
				puntoB,
				(QEasingCurve::Type)interpolacion,
				bucle,
				esconder
			);

			cuadro->agregarAnimacion(nombre, animacion);

			/*+
							"{"+Ogre::StringConverter::toString(puntoA.x())+","+Ogre::StringConverter::toString(puntoA.y())+"}"+
							"{"+Ogre::StringConverter::toString(puntoB.x())+","+Ogre::StringConverter::toString(puntoB.y())+"}"+
							" "+Ogre::StringConverter::toString(interpolacion)+" "+
							" "+Ogre::StringConverter::toString(bucle)+" "
							" "+Ogre::StringConverter::toString(esconder)+"."
			 */
			QString mensaje("");
			mensaje.append("Se agregó animación ");
			mensaje.append(QString("duración:%1 ").arg(duracion));
			mensaje.append(QString("A:{%1,%2} ").arg(puntoA.x()).arg(puntoA.y()));
			mensaje.append(QString("B:{%1,%2} ").arg(puntoB.x()).arg(puntoB.y()));
			mensaje.append(QString("interpolación:%1 ").arg(interpolacion));
			mensaje.append(QString("bucle:%1 ").arg(bucle));
			mensaje.append(QString("esconder:%1.").arg(esconder));

			Ogre::LogManager::getSingleton().logMessage(mensaje.toStdString());
		}
	}

}

void Parser2DS::cargarCuadro(QDomElement& elemento){
	std::string nombre = elemento.attribute("nombre").toStdString();

	QDomElement dimensionElement = elemento.firstChildElement("dimension");
	Ogre::Vector2 dimension(
		cargarDimension(
			dimensionElement
		)
	);

	QDomElement posicionElemento = elemento.firstChildElement("posicion");
	Ogre::Vector2 posicion(
		cargarPosicion(
			posicionElemento
		)
	);

	Cuadro cuadro(nombre, dimension, posicion);

	QDomElement texturaElemento = elemento.firstChildElement("texturas").firstChildElement("textura");
	cargarTexturas(texturaElemento, &cuadro);

	QDomElement animacionElemento = elemento.firstChildElement("animaciones").firstChildElement("animacion");
	cargarAnimaciones(animacionElemento, &cuadro);

	mCuadroMap.insert(
		CuadroPar(nombre,cuadro)
	);

	mCuadroVector.push_back(&mCuadroMap.find(nombre)->second);

	Ogre::LogManager::getSingleton().logMessage(
		"Se agregó elemento box: "+
		nombre+
		"."
	);

}

void Parser2DS::cargarEtiqueta(QDomElement& elemento){

	std::string nombre = elemento.attribute("nombre").toStdString();
	QDomElement dimensionElemento = elemento.firstChildElement("dimension");
	QDomElement posicionElemento = elemento.firstChildElement("posicion");
	QDomElement fuenteElemento = elemento.firstChildElement("fuente");
	QDomElement colorElemento= elemento.firstChildElement("color");
	QDomElement sombraElemento= elemento.firstChildElement("sombra");
	QDomElement alineacionElemento = elemento.firstChildElement("alineacion");
	QDomElement textoElemento = elemento.firstChildElement("texto");

	Ogre::Vector2 dimension(cargarDimension(dimensionElemento));
	Ogre::Vector2 posicion(cargarPosicion(posicionElemento));
	Fuente fuente(cargarFuente(fuenteElemento));
	Ogre::ColourValue color = cargarColor(colorElemento);
	Ogre::ColourValue sombra = cargarSombra(sombraElemento);
	RelativePosition alineacion = cargarAlineacion(alineacionElemento);
	std::string texto = cargarTexto(textoElemento);

	Etiqueta etiqueta(
		nombre,
		dimension,
		posicion,
		fuente,
		color,
		sombra,
		alineacion,
		texto
	);


	mEtiquetaMap.insert(
		EtiquetaPar(nombre,	etiqueta)
	);
	mEtiquetaVector.push_back(&mEtiquetaMap.find(nombre)->second);

	Ogre::LogManager::getSingleton().logMessage(
		"Se agregó elemento label: "+
		nombre+
		"."
	);

}
