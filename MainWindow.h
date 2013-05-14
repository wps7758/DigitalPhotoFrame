#ifndef DigitalPhotoFrame_MainWindow_h
#define DigitalPhotoFrame_MainWindow_h

#include <QtGui>
#include "PaintArea.h"

class MainWindow: public QWidget
{
    Q_OBJECT
    
public:
    MainWindow();
    ~MainWindow();
    
    void createButtons();
    void createLayout();
    void createConnect();
    
private:
    QImage img;
    float scale;
    float rotate;
    
    QToolButton *openFileButton;
    QToolButton *zoomInButton;
    QToolButton *zoomOutButton;
    QToolButton *lastPhotoButton;
    QToolButton *playPhotoButton;
    QToolButton *nextPhotoButton;
    QToolButton *rotate90Button;
    QToolButton *rotate270Button;
    QToolButton *closeButton;
    
    PaintArea *area;
    
    int imageNo;
    QStringList imagePathList;
    QTimer *playTimer;
    
    int alphaTemp;
    int alpha;
    QTimer *changeTimer;
    
    bool pauseFlag;
    
    void openImage();
    void pausePhoto();
    void showButtons(bool);
    void mousePressEvent(QMouseEvent *);
    
private slots:
    void slotOpenFile();
    void slotLastPhoto();
    void slotNextPhoto();
    void slotPlayPhoto();
    void slotChange();
    void slotOncePlayPhoto();
    void slotZoomIn();
    void slotZoomOut();
    void slotRotate90();
    void slotRotate270();
    void slotClose();
};


#endif
