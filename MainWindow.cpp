#include "MainWindow.h"
#include <QDir>
#include <iostream>

using namespace std;

MainWindow::MainWindow() : QWidget()
{
    setWindowTitle(tr("Digital Photo Frame"));  //设置标题
    
    //设置窗口的最小值和最大值
    setMinimumSize(DEVICE_WIDTH, DEVICE_HEIGHT);
    setMaximumSize(DEVICE_WIDTH, DEVICE_HEIGHT);
    
    //设置窗体的背景色
    QPalette pal = palette();  
    pal.setColor(QPalette::Window, QColor(0xd7,0x54,0x55,0x16));  
    setPalette(pal);
    
    //setAttribute(Qt::WA_TranslucentBackground, true);  //设置窗体全透明，除了控件
    setWindowFlags(Qt::FramelessWindowHint);  //隐藏标题栏
    setWindowOpacity(0.92f);  //设置整个窗体的透明度
    
    createButtons();
    createLayout();
    createConnect();
}

MainWindow::~MainWindow()
{
}

//创建按钮栏
void MainWindow::createButtons()
{
    openFileButton  = new QToolButton;
    zoomInButton    = new QToolButton;
    zoomOutButton   = new QToolButton;
    lastPhotoButton = new QToolButton;
    playPhotoButton = new QToolButton;
    nextPhotoButton = new QToolButton;
    rotate90Button  = new QToolButton;
    rotate270Button = new QToolButton;
    closeButton     = new QToolButton;
    
    //设置按钮图片
    QString path = PATH;
    openFileButton  -> setIcon(QIcon(path + "/images/openfile.png"));
    zoomInButton    -> setIcon(QIcon(path + "/images/zoomin.png"));
    zoomOutButton   -> setIcon(QIcon(path + "/images/zoomout.png"));
    lastPhotoButton -> setIcon(QIcon(path + "/images/last.png"));
    playPhotoButton -> setIcon(QIcon(path + "/images/play.png"));
    nextPhotoButton -> setIcon(QIcon(path + "/images/next.png"));
    rotate90Button  -> setIcon(QIcon(path + "/images/rotate90.png"));
    rotate270Button -> setIcon(QIcon(path + "/images/rotate270.png"));
    closeButton     -> setIcon(QIcon(path + "/images/close.png"));
    
    //设置按钮大小
    openFileButton  -> setIconSize(QSize(32, 32));
    zoomInButton    -> setIconSize(QSize(32, 32));
    zoomOutButton   -> setIconSize(QSize(32, 32));
    lastPhotoButton -> setIconSize(QSize(32, 32));
    playPhotoButton -> setIconSize(QSize(32, 32));
    nextPhotoButton -> setIconSize(QSize(32, 32));
    rotate90Button  -> setIconSize(QSize(32, 32));
    rotate270Button -> setIconSize(QSize(32, 32));
    closeButton     -> setIconSize(QSize(32, 32));
    
    //设置按钮样式
    QString buttonCss = "QToolButton{background-color:rgba(35%,70%,86%,20%);}";
    openFileButton  -> setStyleSheet(buttonCss);
    zoomInButton    -> setStyleSheet(buttonCss);
    zoomOutButton   -> setStyleSheet(buttonCss);
    lastPhotoButton -> setStyleSheet(buttonCss);
    playPhotoButton -> setStyleSheet(buttonCss);
    nextPhotoButton -> setStyleSheet(buttonCss);
    rotate90Button  -> setStyleSheet(buttonCss);
    rotate270Button -> setStyleSheet(buttonCss);
    closeButton     -> setStyleSheet(buttonCss);
    
    //创建画布
    area = new PaintArea();
}

//创建布局
void MainWindow::createLayout()
{   
    //按钮栏布局
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout -> setMargin(0);    //设置边框间隔
    bottomLayout -> setSpacing(20);  //设置控件间隔
    bottomLayout -> addStretch();    //空白占位
    bottomLayout -> addWidget(openFileButton);
    bottomLayout -> addWidget(zoomInButton);
    bottomLayout -> addWidget(zoomOutButton);
    bottomLayout -> addWidget(lastPhotoButton);
    bottomLayout -> addWidget(playPhotoButton);
    bottomLayout -> addWidget(nextPhotoButton);
    bottomLayout -> addWidget(rotate90Button);
    bottomLayout -> addWidget(rotate270Button);
    bottomLayout -> addWidget(closeButton);
    bottomLayout -> addStretch();
    
    //整体布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout -> setMargin(0);
    mainLayout -> setSpacing(5);
    mainLayout -> addWidget(area);
    mainLayout -> addLayout(bottomLayout);
}

//创建连接
void MainWindow::createConnect()
{
    playTimer   = new QTimer(this);  //播放计时器，每隔X秒调用slotOncePlayPhoto()
    changeTimer = new QTimer(this);  //特效计时器，每隔X秒调用slotChange()
    
    connect(openFileButton,  SIGNAL(clicked()), this, SLOT(slotOpenFile()));
    connect(zoomInButton,    SIGNAL(clicked()), this, SLOT(slotZoomIn()));
    connect(zoomOutButton,   SIGNAL(clicked()), this, SLOT(slotZoomOut()));
    connect(lastPhotoButton, SIGNAL(clicked()), this, SLOT(slotLastPhoto()));
    connect(playPhotoButton, SIGNAL(clicked()), this, SLOT(slotPlayPhoto()));
    connect(nextPhotoButton, SIGNAL(clicked()), this, SLOT(slotNextPhoto()));
    connect(rotate90Button,  SIGNAL(clicked()), this, SLOT(slotRotate90()));
    connect(rotate270Button, SIGNAL(clicked()), this, SLOT(slotRotate270()));
    connect(closeButton,     SIGNAL(clicked()), this, SLOT(slotClose()));
    connect(playTimer,       SIGNAL(timeout()), this, SLOT(slotOncePlayPhoto()));
    connect(changeTimer,     SIGNAL(timeout()), this, SLOT(slotChange()));
}

//打开照片
void MainWindow::openImage()
{
    //检测imageNo是否溢出
    if (imageNo < 0) {
        imageNo = imagePathList.size() - 1;
    }
    else if (imageNo > imagePathList.size() - 1) {
        imageNo = 0;
    }
    
    //根据编号获取图片路径
    QString s = imagePathList.at(imageNo);
    
    
    //计算出图片最佳化显示的矩阵，并将矩阵和图片传给画布
    if(s != "")
    {
        if(img.load(s))
        {
            float scaleX = (float)(area -> areaWidth) / (float)img.width();
            float scaleY = (float)(area -> areaHeight) / (float)img.height();
            
            scale  = scaleX < scaleY ? scaleX : scaleY;
            rotate = 0.0;
            
            QMatrix mat;
            mat.scale(scale, scale);
            mat.rotate(rotate);
            
            area -> setMatrix(mat);
            area -> setImage(img);
        }
    }
}

//选择照片所在文件夹
void MainWindow::slotOpenFile()
{
    //能打开的图片格式
    QStringList imageTypeList;
    imageTypeList << "bmp" << "jpg" << "jpeg" << "png";
    
    //对话框选择文件路径
    QString imageFilePath = QFileDialog::getOpenFileName(this, "open image file", ".",
                            "Image files (*.bmp *.jpg *.jpeg *.png);;All files (*.*)");
    //如取消，返回
    if (imageFilePath.isEmpty()) {
        return;
    }
    
    //解析出图片所在的路径和照片名
    QFileInfo fi = QFileInfo(imageFilePath);
    QString imagePath = fi.absolutePath();
    QString imageName = fi.fileName();
    
    //将图片所在路径的全部文件导入list
    QDir dir(imagePath);
    QFileInfoList list = dir.entryInfoList();
    
    //初始化
    imageNo = 0;  //给图片编号
    int imageNoTemp = 0;
    imagePathList.clear();  //清空原来的路径list
    
    //找出符合条件的图片文件，并将其输入到imagePathList中。
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        for (int j = 0; j < imageTypeList.size(); ++j) {
            //对比图片后缀名
            if (fileInfo.completeSuffix() == imageTypeList.at(j)) {
                imagePathList << fileInfo.absoluteFilePath();
                //对比当前的图片输入到imagePathList中的顺序，并记录
                if (fileInfo.absoluteFilePath() == imageFilePath) {
                    imageNo = imageNoTemp;
                }
                ++imageNoTemp;
                break;
            }
        }
    }
    
    //根据imageNo编号打开图片
    openImage();
}

//打开上一张图片
void MainWindow::slotLastPhoto()
{
    if(img.isNull())
	{
		return;
	}
    
    --imageNo;
    openImage();
}

//打开下一张图片
void MainWindow::slotNextPhoto()
{
    if(img.isNull())
	{
		return;
	}
    
    ++imageNo;
    openImage();
}

//是否隐藏按钮栏
void MainWindow::showButtons(bool flag)
{
    openFileButton  -> setVisible(flag);
    zoomInButton    -> setVisible(flag);
    zoomOutButton   -> setVisible(flag);
    lastPhotoButton -> setVisible(flag);
    playPhotoButton -> setVisible(flag);
    nextPhotoButton -> setVisible(flag);
    rotate90Button  -> setVisible(flag);
    rotate270Button -> setVisible(flag);
    closeButton     -> setVisible(flag);
}

//播放图片
void MainWindow::slotPlayPhoto()
{
    if(img.isNull())
	{
		return;
	}
    
    //隐藏按钮栏
    pauseFlag = false;
    showButtons(pauseFlag);
    
    //允许画布设置特效，并将画布全屏
    area -> setIsChange(true);
    area -> setAreaSize(DEVICE_WIDTH, DEVICE_HEIGHT);
    
    //播放一次图片
    slotOncePlayPhoto();
}

//一次播放图片过程
void MainWindow::slotOncePlayPhoto()
{
    if (!pauseFlag) {              //检测是否暂停
        playTimer -> start(5000);  //设置播放间隔
        
        //初始化slotChange()参数，并调用slotChange()
        alpha = 0;
        slotChange();
        
        //把当前图片传给画布，同openImage()
        if (imageNo < 0) {
            imageNo = imagePathList.size() - 1;
        }
        else if (imageNo > imagePathList.size() - 1) {
            imageNo = 0;
        }
        
        QString s = imagePathList.at(imageNo);
        
        if(s != "") 
        {
            if(img.load(s)) 
            {
                QMatrix mat;
                
                float scaleX = (float)(area -> areaWidth)  / (float)img.width();
                float scaleY = (float)(area -> areaHeight) / (float)img.height();
                scale = scaleX < scaleY ? scaleX : scaleY;
                mat.scale(scale, scale);
                area -> setLastMatrix(mat);
                area -> setLastImage(img);
            }
        }
        
        //把下一张图片传给画布
        ++imageNo;
        openImage();
    }
}

//特效过程
void MainWindow::slotChange()
{
    if (alpha < 100) 
    {
        changeTimer -> start(20);
        alpha += 5;
        area -> setAlphaValue(alpha);
    }
}

//暂停播放
void MainWindow::pausePhoto()
{
    area -> setIsChange(false);
    area -> setAreaSize(DEVICE_WIDTH, DEVICE_HEIGHT - BUTTONS_HEIGHT);
    
    pauseFlag = true;
    showButtons(pauseFlag);
}

//放大图片
void MainWindow::slotZoomIn()
{
    if(img.isNull())
	{
		return;
	}
    
    QMatrix mat;
    scale *= 2;
	mat.scale(scale, scale);
    area -> setMatrix(mat);
}

//缩小图片
void MainWindow::slotZoomOut()
{
    if(img.isNull())
	{
		return;
	}
    
    QMatrix mat;
    scale *= 0.5;
	mat.scale(scale, scale);
    area -> setMatrix(mat);
}

//顺时针旋转90度
void MainWindow::slotRotate90()
{
    if(img.isNull())
	{
		return;
	}
    
    QMatrix mat;
    rotate += 90;
	mat.rotate(rotate);
    area -> setMatrix(mat);
}

//逆时针旋转90度
void MainWindow::slotRotate270()
{
    if(img.isNull())
	{
		return;
	}
    
    QMatrix mat;
    rotate -= 90;
	mat.rotate(rotate);
    area -> setMatrix(mat);
}

//关闭程序
void MainWindow::slotClose()
{
    exit(0);
}

//点击窗体则暂停播放
void MainWindow::mousePressEvent(QMouseEvent *)
{
    pausePhoto();
}


















