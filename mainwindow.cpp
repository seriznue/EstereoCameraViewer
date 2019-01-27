#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <EstereoCameraCapturer.h>
#include <QPixmap>
#include <opencv2/imgproc/imgproc.hpp>
#include <asmOpenCV.h>

static const int CAM_WIDTH = 640;
static const int CAM_HEIGHT = 680;

static EstereoCameraCapturer *estereoCameraCapturerPtr1 = 0;
static QTimer *timer1 = 0;

void StartEstereoCameraCapturer()
{
    if (estereoCameraCapturerPtr1) {
        estereoCameraCapturerPtr1->Start();
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_estereoCameraCapturer(0)
{
    ui->setupUi(this);

    timer1 = new QTimer;
    timer1->start(33);

    m_estereoCameraCapturer = new EstereoCameraCapturer(CAM_WIDTH, CAM_HEIGHT, timer1);

    estereoCameraCapturerPtr1 = m_estereoCameraCapturer;
    std::thread t1(StartEstereoCameraCapturer);
    t1.join();

    connect(estereoCameraCapturerPtr1, SIGNAL(imageCaptured()), this, SLOT(ShowImageCaptured()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
}

void MainWindow::ShowImageCaptured()
{
    cv::Mat cvImg1, cvImg2;

    if(!estereoCameraCapturerPtr1)
        return;

    estereoCameraCapturerPtr1->GetCapture(cvImg1, cvImg2);

    QImage qimage1 = ASM::cvMatToQImage(cvImg1);
    this->ui->leftImageLabel->setPixmap(QPixmap::fromImage(qimage1));

    QImage qimage2 = ASM::cvMatToQImage(cvImg2);
    this->ui->rightImageLabel->setPixmap(QPixmap::fromImage(qimage2));

    /*

    cvImg1 = ASM::QImageToCvMat(*qImg1, true);
    cv::Mat imgGrayscale, imgBlurred, imgCanny;
    cv::cvtColor(cvImg1, imgGrayscale, CV_BGR2GRAY);                   // convert to grayscale
    cv::GaussianBlur(imgGrayscale,imgBlurred,cv::Size(5, 5),1.8);           // Blur Effect
    double lowTh = 10 , highTh = 100;
    cv::Canny(imgBlurred,imgCanny,lowTh,highTh);       // Canny Edge Image
    QImage qImgCammy = ASM::cvMatToQImage(imgCanny);
    this->ui->imageLabel->setPixmap(QPixmap::fromImage(qImgCammy));

    //qImg1->save("/home/seriznue/kaka2.png");
    if(qImg1) {
        this->ui->imageLabel->setPixmap(QPixmap::fromImage(qImgCammy));
        delete qImg1;
        qImg1 = 0;
    }
    */
}
