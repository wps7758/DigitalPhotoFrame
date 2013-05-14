#include <QApplication>
#include "MainWindow.h"
#include "iostream"

using namespace std;

int main(int argc, char * argv[])
{
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    QString path = PATH;
    
    QApplication app(argc, argv);
    
    //启动画面
    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(path + "/images/start.jpg"));
    splash->show();
    
    //延长启动画面时间
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < 2000) {
        splash -> repaint();
    }
    
    //背景画面，加载gif
    QSplashScreen *backSplash = new QSplashScreen;
    backSplash->setPixmap(QPixmap(path + "/images/start.gif"));
    
    QLabel *label = new QLabel(backSplash);
    QMovie *movie = new QMovie(path + "/images/start.gif");
    
    label -> setMovie(movie);
    movie -> start();
    
    backSplash -> show ();
    backSplash -> setCursor(Qt::BlankCursor); 
    backSplash -> move ((QApplication::desktop()->width()  - backSplash->width())/2,
                        (QApplication::desktop()->height() - backSplash->height())/2);
    
    
    MainWindow * mainWindow = new MainWindow();
    mainWindow -> show();
    mainWindow -> move ((QApplication::desktop()->width()  - mainWindow->width())/2,
                        (QApplication::desktop()->height() - mainWindow->height())/2);
    
    //主程序启动成功，销毁启动画面
    splash->finish(mainWindow);
    delete splash;
    
    //test
    cout << "the main window : " 
    << mainWindow -> x() << " "
    << mainWindow -> y() << " "
    << mainWindow -> width() << " "
    << mainWindow -> height() << " "
    << endl;
    //test
    
    return app.exec();
}