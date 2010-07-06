/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created: Mon Jul 5 17:14:27 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QAction *actionCargarRecursos;
    QAction *actionSalir;
    QAction *actionCargarSprites;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QSplitter *splitter;
    QTreeView *arbolXML;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *layoutOgre;
    QGroupBox *GroupPropiedades;
    QMenuBar *menubar;
    QMenu *menuArchivo;
    QMenu *menuEscena;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QString::fromUtf8("MainWindowClass"));
        MainWindowClass->resize(800, 600);
        actionCargarRecursos = new QAction(MainWindowClass);
        actionCargarRecursos->setObjectName(QString::fromUtf8("actionCargarRecursos"));
        actionSalir = new QAction(MainWindowClass);
        actionSalir->setObjectName(QString::fromUtf8("actionSalir"));
        actionCargarSprites = new QAction(MainWindowClass);
        actionCargarSprites->setObjectName(QString::fromUtf8("actionCargarSprites"));
        centralwidget = new QWidget(MainWindowClass);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        splitter = new QSplitter(centralwidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        arbolXML = new QTreeView(splitter);
        arbolXML->setObjectName(QString::fromUtf8("arbolXML"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(arbolXML->sizePolicy().hasHeightForWidth());
        arbolXML->setSizePolicy(sizePolicy);
        arbolXML->setMaximumSize(QSize(400, 16777215));
        splitter->addWidget(arbolXML);
        verticalLayoutWidget = new QWidget(splitter);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        layoutOgre = new QVBoxLayout(verticalLayoutWidget);
        layoutOgre->setObjectName(QString::fromUtf8("layoutOgre"));
        layoutOgre->setSizeConstraint(QLayout::SetMaximumSize);
        layoutOgre->setContentsMargins(0, 0, 0, 0);
        splitter->addWidget(verticalLayoutWidget);
        GroupPropiedades = new QGroupBox(splitter);
        GroupPropiedades->setObjectName(QString::fromUtf8("GroupPropiedades"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(GroupPropiedades->sizePolicy().hasHeightForWidth());
        GroupPropiedades->setSizePolicy(sizePolicy1);
        GroupPropiedades->setMaximumSize(QSize(200, 16777215));
        GroupPropiedades->setStyleSheet(QString::fromUtf8("QGroupBox {font-weight: bold;  border: 2px groove gray;  border-radius: 5px;  padding: 0 2px;}   QGroupBox::title{top: 5px; left: 8px;}"));
        splitter->addWidget(GroupPropiedades);

        verticalLayout_2->addWidget(splitter);

        MainWindowClass->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindowClass);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 25));
        menuArchivo = new QMenu(menubar);
        menuArchivo->setObjectName(QString::fromUtf8("menuArchivo"));
        menuEscena = new QMenu(menubar);
        menuEscena->setObjectName(QString::fromUtf8("menuEscena"));
        MainWindowClass->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindowClass);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindowClass->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindowClass);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainWindowClass->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menuArchivo->menuAction());
        menubar->addAction(menuEscena->menuAction());
        menuArchivo->addAction(actionCargarRecursos);
        menuEscena->addAction(actionCargarSprites);

        retranslateUi(MainWindowClass);

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionCargarRecursos->setText(QApplication::translate("MainWindowClass", "Cargar Recursos", 0, QApplication::UnicodeUTF8));
        actionSalir->setText(QApplication::translate("MainWindowClass", "Salir", 0, QApplication::UnicodeUTF8));
        actionCargarSprites->setText(QApplication::translate("MainWindowClass", "Cargar Sprites", 0, QApplication::UnicodeUTF8));
        GroupPropiedades->setTitle(QApplication::translate("MainWindowClass", "Propiedades", 0, QApplication::UnicodeUTF8));
        menuArchivo->setTitle(QApplication::translate("MainWindowClass", "Archivo", 0, QApplication::UnicodeUTF8));
        menuEscena->setTitle(QApplication::translate("MainWindowClass", "Escena", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("MainWindowClass", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
