#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <EstereoCameraCapturer.h>
#include <QPixmap>
#include <opencv2/imgproc/imgproc.hpp>
#include <asmOpenCV.h>

static const int LEFT_CAM_INDEX = 0;
static const int RIGHT_CAM_INDEX = 1;
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

    m_estereoCameraCapturer = new EstereoCameraCapturer(LEFT_CAM_INDEX,  CAM_WIDTH, CAM_HEIGHT, timer1);

    estereoCameraCapturerPtr1 = m_estereoCameraCapturer;
    std::thread t1(StartEstereoCameraCapturer);
    t1.join();

    connect(estereoCameraCapturerPtr1, SIGNAL(imageCaptured()), this, SLOT(ShowImageCaptured1()));
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
    QImage *qImg1 = 0;
    QImage *qImg2 = 0;
    //cv::Mat cvImg1, cvImg2;

    while (!qImg1 && !qImg2 && estereoCameraCapturerPtr1) {
        estereoCameraCapturerPtr1->GetCapture(&qImg1, &qImg2);
    }


    if(qImg1) {
        this->ui->leftImageLabel->setPixmap(QPixmap::fromImage(*qImg1));
        delete qImg1;
        qImg1 = 0;
    }

    if(qImg2) {
        this->ui->rightImageLabel->setPixmap(QPixmap::fromImage(*qImg2));
        delete qImg2;
        qImg2 = 0;
    }

    return;

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

//void MainWindow::ShowImageCaptured()
//{
//    QImage *qImg1 = 0;
//    cv::Mat *cvImg1 = 0;

//    while (!cvImg1) {
//        //cameraCapturerPtr1->GetCapture(&qImg1);
//        cameraCapturerPtr1->GetCapture(&cvImg1);
//    }

//    qImg1 = new QImage((const unsigned char*)(cvImg1->data), cvImg1->cols, cvImg1->rows, cvImg1->step, QImage::Format_RGB888);

//    if(qImg1) {
//        this->ui->imageLabel->setPixmap(QPixmap::fromImage(*qImg1));
//        delete qImg1;
//        qImg1 = 0;
//    }

//    if(cvImg1) {
//        delete cvImg1;
//        cvImg1 = 0;
//    }
//}

//void MainWindow::ShowImageCaptured()
//{
//    QImage *qImg1 = 0;
//    cv::Mat *cvImg1 = 0;

//    while (!cvImg1) {
//        //cameraCapturerPtr1->GetCapture(&qImg1);
//        cameraCapturerPtr1->GetCapture(&cvImg1);
//    }

//    cv::Mat imgOri, imgGrayscale, imgBlurred, imgCanny;
//    imgOri = cvImg1->clone();
//    cv::cvtColor(imgOri, imgGrayscale, CV_BGR2GRAY);                   // convert to grayscale
//    cv::GaussianBlur(imgGrayscale,imgBlurred,cv::Size(5, 5),1.8);           // Blur Effect
//    double lowTh = 10 , highTh = 100;
//    cv::Canny(imgBlurred,imgCanny,lowTh,highTh);       // Canny Edge Image
//    qImg1 = new QImage((const unsigned char*)(imgCanny.data), imgCanny.cols, imgCanny.rows, imgCanny.step, QImage::Format_RGB888);

//    //qImg1 = new QImage((const unsigned char*)(cvImg1->data), cvImg1->cols, cvImg1->rows, cvImg1->step, QImage::Format_RGB888);

//    if(qImg1) {
//        this->ui->imageLabel->setPixmap(QPixmap::fromImage(*qImg1));
//        delete qImg1;
//        qImg1 = 0;
//    }

//    if(cvImg1) {
//        delete cvImg1;
//        cvImg1 = 0;
//    }
//}
