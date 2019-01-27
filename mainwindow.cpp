#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <EstereoCameraCapturer.h>
#include <QPixmap>
#include <opencv2/imgproc/imgproc.hpp>
#include <QtOpenCVInteroperability.h>
#include <CvHelper.h>

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

    QImage qimage1 = QtOpenCV::cvMatToQImage(cvImg1);
    this->ui->leftImageLabel->setPixmap(QPixmap::fromImage(qimage1));

    QImage qimage2 = QtOpenCV::cvMatToQImage(cvImg2);
    this->ui->rightImageLabel->setPixmap(QPixmap::fromImage(qimage2));

    cv::Mat cvCanny1, cvCanny2;

    CvHelper::Canny(cvImg1, cvCanny1);
    CvHelper::Canny(cvImg2, cvCanny2);

    QImage qimageCanny1 = QtOpenCV::cvMatToQImage(cvCanny1);
    this->ui->leftImageLabel_2->setPixmap(QPixmap::fromImage(qimageCanny1));

    QImage qimageCanny2 = QtOpenCV::cvMatToQImage(cvCanny2);
    this->ui->rightImageLabel_2->setPixmap(QPixmap::fromImage(qimageCanny2));
}
