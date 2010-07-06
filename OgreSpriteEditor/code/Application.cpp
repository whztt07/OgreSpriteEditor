#include "Application.h"

Application::Application(int argc, char *argv[])
         : QApplication(argc, argv)
{
    connect( this, SIGNAL(lastWindowClosed()), this, SLOT(OnClose()) );
}

// ----------------------------------------------------------
Application::~Application()
{
}

// ----------------------------------------------------------
void Application::showMainWidget()
{
    mMainWindow = new MainWindow();
    mMainTimerID = this->startTimer(5);
    installEventFilter(this);
    mMainWindow->show();
}

// ----------------------------------------------------------
void Application::OnClose()
{
    this->killTimer(mMainTimerID);
    printf("Quitting Application\n");
    delete mMainWindow;
    QApplication::quit();
}

// ----------------------------------------------------------
bool Application::eventFilter(QObject* o, QEvent* event)
{
    switch ( event->type() )
    {
        case QEvent::KeyPress:
        {
            int key = (static_cast<QKeyEvent *>(event))->key();
            if (key == Qt::Key_Escape) {
                removeEventFilter(this);
                mMainWindow->close();
                return true; //eat event
            }
            else {
                return false;
            }
        }
        break;

        case QEvent::Timer:
        {
            QTimerEvent *timerEvent = static_cast<QTimerEvent*>(event);
            if (timerEvent->timerId() == mMainTimerID)
            {
                mMainWindow->updateOGRE();
                return true; //eat event
            }
        }
        break;
    }
    return QApplication::eventFilter(o, event);
}
