#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include "MainWindow.h"


class Application : public QApplication
{
    Q_OBJECT

public:
    Application(int argc, char *argv[]);
    ~Application();
    void showMainWidget();

private:
    MainWindow* mMainWindow;
    int mMainTimerID;
    QMutex mutex;

private slots:
    void OnClose();

protected:
    bool eventFilter(QObject* o, QEvent* event);
};

#endif // APPLICATION_H
