#ifndef DigitalPhotoFrame_PaintArea_h
#define DigitalPhotoFrame_PaintArea_h

#include <QtGui>

#define PATH "/Users/wps_7758/Documents/Xcode/LearnQT/DigitalPhotoFrame"
#define PI 3.14159265
#define DEVICE_WIDTH   1024
#define DEVICE_HEIGHT  768
#define BUTTONS_HEIGHT 46

class PaintArea : public QWidget
{
    Q_OBJECT
public:
    PaintArea();
    
    void setImage(QImage);
    void setLastImage(QImage);
    void setMatrix(QMatrix);
    void setLastMatrix(QMatrix);
    void setIsChange(bool);
    void setAlphaValue(int);
    void setAreaSize(int, int);
    void paintEvent(QPaintEvent *);
    void shadeChange();
    void turnChange();
    
    int     areaWidth;
    int     areaHeight;
    
private:
    QImage  image;
    QImage  imageTra;
    QImage  lastImage;
    QMatrix matrix;
    QMatrix lastMatrix;
    bool    changeFlag;
    int     alphaValue;
};



#endif
