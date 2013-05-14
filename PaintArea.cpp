#include "PaintArea.h"
#include "iostream"

using namespace std;

PaintArea::PaintArea() : QWidget()
{
    setAutoFillBackground(true);
    
    //设置画布大小
    areaWidth  = DEVICE_WIDTH;
    areaHeight = DEVICE_HEIGHT - BUTTONS_HEIGHT;
    setMinimumSize(areaWidth, areaHeight);
    setMaximumSize(areaWidth, areaHeight);
    
    changeFlag = false;
    alphaValue = 0;
}

void PaintArea::setImage(QImage i)
{
    image = i;
    update();
}

void PaintArea::setLastImage(QImage li)
{
    lastImage = li;
    update();
}

void PaintArea::setMatrix(QMatrix m)
{
    matrix = m;
    update();
}

void PaintArea::setLastMatrix(QMatrix lm)
{
    lastMatrix = lm;
    update();
}

void PaintArea::setIsChange(bool b)
{
    changeFlag = b;
    update();
}

void PaintArea::setAlphaValue(int a)
{
    alphaValue = a;
    update();
}

void PaintArea::setAreaSize(int w, int h)
{
    areaWidth  = w;
    areaHeight = h;
    update();
}

void PaintArea::paintEvent(QPaintEvent *)
{   
    //重设画布大小
    setMinimumSize(areaWidth, areaHeight);
    setMaximumSize(areaWidth, areaHeight);
    
    //最佳化转换图片
    imageTra = image.transformed(matrix);
    
    //当前图片信息
    int iw = imageTra.width();
    int ih = imageTra.height();
    int ix = (areaWidth - iw) * 0.5;
    int iy = (areaHeight - ih) * 0.5;
    
    
    
    if (changeFlag)
    {
        shadeChange();
        //turnChange();
    }

    
    //在画布上显示图片
    QPainter painter(this);
    painter.drawPixmap(ix, iy, iw, ih, QPixmap::fromImage(imageTra));
    
}

//翻页特效
void PaintArea::turnChange()
{
    //当前图片信息
    int iw = imageTra.width();
    int ih = imageTra.height();
    
    //处理上一张图片
    QImage lastImageTra;
    lastImageTra = lastImage.transformed(lastMatrix);
    int lw = lastImageTra.width();
    int lh = lastImageTra.height();
    
    //两张图片的最大值和间隔
    int maxW = (lw > iw) ? lw : iw;
    int maxH = (lh > ih) ? lh : ih;
    
    int maxR = (maxW > maxH) ? maxH : maxW;
    
    int r = (1 - 0.01 * alphaValue) * maxR;
    
    //int pageWbegin = 0.01 * alphaValue * maxW;
    //int pageHbegin = 0.01 * alphaValue * maxH;
    //int pageWend = (1 - 0.01 * alphaValue) * maxW;
    //int pageHend = (1 - 0.01 * alphaValue) * maxH;
    
    for (int h = 0; h < maxH; ++h)
    {
        for (int w = 0; w < maxW; ++w) 
        {
            if ((w-maxW/2)*(w-maxW/2) + (h-maxH/2)*(h-maxH/2) < r*r)
            {
                int red   = (int)(qRed  (lastImage.pixel(w, h)));
                int green = (int)(qGreen(lastImage.pixel(w, h)));
                int blue  = (int)(qBlue (lastImage.pixel(w, h)));
                
                imageTra.setPixel(w, h, qRgb(red, green, blue));
            }
        }
    }
}

//渐变特效
void PaintArea::shadeChange()
{
    QColor bkColor = palette().color(QPalette::Window);
    float v = 0.01 * alphaValue;
    
    //当前图片信息
    int iw = imageTra.width();
    int ih = imageTra.height();
    
    //处理上一张图片
    QImage lastImageTra;
    lastImageTra = lastImage.transformed(lastMatrix);
    int lw = lastImageTra.width();
    int lh = lastImageTra.height();
    
    //两张图片的最大值和间隔
    int   maxW = (lw > iw) ? lw : iw;
    int   maxH = (lh > ih) ? lh : ih;
    float difW = (lw < iw) ? (iw-lw)*0.5 : (lw-iw)*0.5;
    float difH = (lh < ih) ? (ih-lh)*0.5 : (lh-ih)*0.5;
    
    //以下分四种情况计算，分别是：
    //1.小图片转换至大图片，或者是同样大小图片转换
    //2.大图片转换至小图片
    //3.宽图片转换至高图片
    //4.高图片转换至宽图片
    
    //1.小图片转换至大图片，或者是同样大小图片转换
    if (lw >= iw && lh >= ih) 
    {
        for (int h = difH; h < maxH - difH; ++h)
        {
            for (int w = difW; w < maxW - difW; ++w) 
            {
                //透明通道，逐渐降低上一张图片的透明值
                if (0 == qAlpha(imageTra.pixel(w-difW, h-difH)))
                {
                    int red   = (int)(qRed  (lastImageTra.pixel(w, h)));
                    int green = (int)(qGreen(lastImageTra.pixel(w, h)));
                    int blue  = (int)(qBlue (lastImageTra.pixel(w, h)));
                    
                    QRgb rgb = QColor(red, green, blue, (int)(255*(1-v))).rgba();
                    imageTra.setPixel(w-difW, h-difH, rgb);
                }
                
                //非透明通道，通过两张图片不同比例加成出现特效
                else 
                {
                    int red   = (int)((qRed  (    imageTra.pixel(w-difW, h-difH))) * v +
                                      (qRed  (lastImageTra.pixel(w,      h)))      * (1-v));
                    int green = (int)((qGreen(    imageTra.pixel(w-difW, h-difH))) * v +
                                      (qGreen(lastImageTra.pixel(w,      h)))      * (1-v));
                    int blue  = (int)((qBlue (    imageTra.pixel(w-difW, h-difH))) * v +
                                      (qBlue (lastImageTra.pixel(w,      h)))      * (1-v));
                    
                    imageTra.setPixel(w-difW, h-difH, qRgb(red, green, blue));
                }
                
            }
        }
    }
    
    //2.大图片转换至小图片
    else if (lw <= iw && lh <= ih) 
    {
        for (int h = 0; h < maxH; ++h) 
        {
            for (int w = 0; w < maxW; ++w) 
            {
                //两张图片重叠部分
                if (h > difH && h < maxH - difH && 
                    w > difW && w < maxW - difW) 
                {
                    if (0 == qAlpha(imageTra.pixel(w, h)))
                    {
                        int red   = (int)(qRed  (lastImageTra.pixel(w-difW, h-difH)));
                        int green = (int)(qGreen(lastImageTra.pixel(w-difW, h-difH)));
                        int blue  = (int)(qBlue (lastImageTra.pixel(w-difW, h-difH)));
                        
                        QRgb rgb = QColor(red, green, blue, (int)(255*(1-v))).rgba();
                        imageTra.setPixel(w, h, rgb);
                    }
                    else 
                    {
                        int red   = (int)((qRed  (    imageTra.pixel(w,      h)))      * v +
                                          (qRed  (lastImageTra.pixel(w-difW, h-difH))) * (1-v));
                        int green = (int)((qGreen(    imageTra.pixel(w,      h)))      * v +
                                          (qGreen(lastImageTra.pixel(w-difW, h-difH))) * (1-v));
                        int blue  = (int)((qBlue (    imageTra.pixel(w,      h)))      * v +
                                          (qBlue (lastImageTra.pixel(w-difW, h-difH))) * (1-v));
                        
                        imageTra.setPixel(w, h, qRgb(red, green, blue));
                    }
                }
                //不重叠部分
                else 
                {
                    if (0 == qAlpha(imageTra.pixel(w, h)))
                    {
                        int red   = (int)(qRed  ( bkColor.rgb()));
                        int green = (int)(qGreen( bkColor.rgb()));
                        int blue  = (int)(qBlue ( bkColor.rgb()));
                        
                        QRgb rgb = QColor(red, green, blue, (int)(255*(1-v))).rgba();
                        imageTra.setPixel(w, h, rgb);
                    }
                    else 
                    {
                        int red   = (int)((qRed  (imageTra.pixel(w, h))) * v +
                                          (qRed  ( bkColor.rgb()))       * (1-v));
                        int green = (int)((qGreen(imageTra.pixel(w, h))) * v +
                                          (qGreen( bkColor.rgb()))       * (1-v));
                        int blue  = (int)((qBlue (imageTra.pixel(w, h))) * v +
                                          (qBlue ( bkColor.rgb()))       * (1-v));
                        
                        imageTra.setPixel(w, h, qRgb(red, green, blue));
                    }
                }
            }
        }
    }
    
    //3.宽图片转换至高图片
    else if (lw > iw && lh < ih) 
    {
        for (int h = 0; h < maxH; ++h) 
        {
            for (int w = difW + 1; w < maxW - difW; ++w) 
            {
                //重叠部分
                if (h > difH && h < maxH - difH) 
                {
                    if (0 == qAlpha(imageTra.pixel(w-difW, h)))
                    {
                        int red   = (int)(qRed  (lastImageTra.pixel(w, h-difH)));
                        int green = (int)(qGreen(lastImageTra.pixel(w, h-difH)));
                        int blue  = (int)(qBlue (lastImageTra.pixel(w, h-difH)));
                        
                        QRgb rgb = QColor(red, green, blue, (int)(255*(1-v))).rgba();
                        imageTra.setPixel(w-difW, h, rgb);
                    }
                    else 
                    {
                        int red   = (int)((qRed  (    imageTra.pixel(w-difW, h))) * v +
                                          (qRed  (lastImageTra.pixel(w, h-difH))) * (1-v));
                        int green = (int)((qGreen(    imageTra.pixel(w-difW, h))) * v +
                                          (qGreen(lastImageTra.pixel(w, h-difH))) * (1-v));
                        int blue  = (int)((qBlue (    imageTra.pixel(w-difW, h))) * v +
                                          (qBlue (lastImageTra.pixel(w, h-difH))) * (1-v));
                        
                        imageTra.setPixel(w-difW, h, qRgb(red, green, blue));
                    }
                }
                //不重叠部分
                else 
                {
                    if (0 == qAlpha(imageTra.pixel(w-difW, h)))
                    {
                        int red   = (int)(qRed  ( bkColor.rgb()));
                        int green = (int)(qGreen( bkColor.rgb()));
                        int blue  = (int)(qBlue ( bkColor.rgb()));
                        
                        QRgb rgb = QColor(red, green, blue, (int)(255*(1-v))).rgba();
                        imageTra.setPixel(w-difW, h, rgb);
                    }
                    else 
                    {
                        int red   = (int)((qRed  (imageTra.pixel(w-difW, h))) * v +
                                          (qRed  ( bkColor.rgb()))       * (1-v));
                        int green = (int)((qGreen(imageTra.pixel(w-difW, h))) * v +
                                          (qGreen( bkColor.rgb()))       * (1-v));
                        int blue  = (int)((qBlue (imageTra.pixel(w-difW, h))) * v +
                                          (qBlue ( bkColor.rgb()))       * (1-v));
                        
                        imageTra.setPixel(w-difW, h, qRgb(red, green, blue));
                    }
                }
            }
        }
    }
    
    //4.高图片转换至宽图片
    else
    {
        for (int h = difH + 1; h < maxH - difH; ++h) 
        {
            for (int w = 0; w < maxW; ++w) 
            {
                //重叠部分
                if (w > difW && w < maxW - difW) 
                {
                    if (0 == qAlpha(imageTra.pixel(w, h-difH)))
                    {
                        int red   = (int)(qRed  (lastImageTra.pixel(w-difW, h)));
                        int green = (int)(qGreen(lastImageTra.pixel(w-difW, h)));
                        int blue  = (int)(qBlue (lastImageTra.pixel(w-difW, h)));
                        
                        QRgb rgb = QColor(red, green, blue, (int)(255*(1-v))).rgba();
                        imageTra.setPixel(w, h-difH, rgb);
                    }
                    else 
                    {
                        int red   = (int)((qRed  (    imageTra.pixel(w, h-difH))) * v +
                                          (qRed  (lastImageTra.pixel(w-difW, h))) * (1-v));
                        int green = (int)((qGreen(    imageTra.pixel(w, h-difH))) * v +
                                          (qGreen(lastImageTra.pixel(w-difW, h))) * (1-v));
                        int blue  = (int)((qBlue (    imageTra.pixel(w, h-difH))) * v +
                                          (qBlue (lastImageTra.pixel(w-difW, h))) * (1-v));
                        
                        imageTra.setPixel(w, h-difH, qRgb(red, green, blue));
                    }
                }
                
                //不重叠部分
                else 
                {
                    if (0 == qAlpha(imageTra.pixel(w, h-difH)))
                    {
                        int red   = (int)(qRed  ( bkColor.rgb()));
                        int green = (int)(qGreen( bkColor.rgb()));
                        int blue  = (int)(qBlue ( bkColor.rgb()));
                        
                        QRgb rgb = QColor(red, green, blue, (int)(255*(1-v))).rgba();
                        imageTra.setPixel(w, h-difH, rgb);
                    }
                    else 
                    {
                        int red   = (int)((qRed  (imageTra.pixel(w, h-difH))) * v +
                                          (qRed  ( bkColor.rgb()))       * (1-v));
                        int green = (int)((qGreen(imageTra.pixel(w, h-difH))) * v +
                                          (qGreen( bkColor.rgb()))       * (1-v));
                        int blue  = (int)((qBlue (imageTra.pixel(w, h-difH))) * v +
                                          (qBlue ( bkColor.rgb()))       * (1-v));
                        
                        imageTra.setPixel(w, h-difH, qRgb(red, green, blue));
                    }
                }
            }
        }
    }
}






























