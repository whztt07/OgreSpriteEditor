#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_MainWindow.h"
#include "OgreTestWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void updateOGRE();

private:
    QMutex mutex;
    Ui::MainWindowClass ui;
    OgreTestWidget* mOgreWidget;
    QString rutaRecursos;
    QString rutaSprites;


private slots:
    void cargarRecursos();
    void cargarSprites();

};

#endif // MAINWINDOW_H
