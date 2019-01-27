#include "EstereoCameraCapturer.h"
#include <QImage>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#include <QDebug>

using namespace cv;

EstereoCameraCapturer::EstereoCameraCapturer(int index, int width, int height, QTimer *timer) :
    QObject(),
    m_cameraTimer(0),
    m_isCameraRunning (false),
    m_index(index),
    m_width(width),
    m_height(height),
    m_qImage1(0),
    m_qImage2(0)
{
    if(timer)
        m_cameraTimer = timer;

}

void EstereoCameraCapturer::Start()
{
    if(!m_isCameraRunning)
    {
        // open camera stream
        m_capture1.open(0, CAP_ANY); // default: 0

        if(!m_capture1.isOpened())
            return;

        m_capture2.open(1, CAP_ANY); // default: 0

        if(!m_capture2.isOpened())
            return;

        // set the acquired frame size to the size of its container
        m_capture1.set(cv::CAP_PROP_FRAME_WIDTH, m_width);
        m_capture1.set(CAP_PROP_FRAME_HEIGHT, m_height);

        m_capture2.set(cv::CAP_PROP_FRAME_WIDTH, m_width);
        m_capture2.set(CAP_PROP_FRAME_HEIGHT, m_height);

        m_isCameraRunning = true;

        // start timer for acquiring the video
        //m_cameraTimer->start(33); // 33 ms = 30 fps
        // at the timeout() event, execute the cameraTimerTimeout() method
        connect(m_cameraTimer, SIGNAL(timeout()), this, SLOT(CameraTimerTimeout()));
    }

}

void EstereoCameraCapturer::Stop()
{
    if(!m_capture1.isOpened() || !m_capture2.isOpened())
        return;

    // stop timer
    m_cameraTimer->stop();
    // release camera stream
    m_capture1.release();
    m_capture2.release();

    m_isCameraRunning = false;

    disconnect(m_cameraTimer, SIGNAL(timeout()), this, SLOT(CameraTimerTimeout()));
}

void EstereoCameraCapturer::CameraTimerTimeout()
{
    if(!m_isCameraRunning)
        return;

    if(!m_capture1.isOpened())
        return;

    if(!m_capture2.isOpened())
        return;

    m_capture1 >> m_image1;
    m_capture2 >> m_image2;

    if(m_qImage1) {
        delete m_qImage1;
        m_qImage1 = 0;
    }

    if(m_qImage2) {
        delete m_qImage2;
        m_qImage2 = 0;
    }

    m_qImage1 = new QImage((const unsigned char*)(m_image1.data), m_image1.cols, m_image1.rows, m_image1.step, QImage::Format_RGB888);
    m_qImage2 = new QImage((const unsigned char*)(m_image2.data), m_image2.cols, m_image2.rows, m_image2.step, QImage::Format_RGB888);

    emit imageCaptured();


}

void EstereoCameraCapturer::GetCapture(QImage **qImage1, QImage **qImage2) const {
    disconnect(m_cameraTimer, SIGNAL(timeout()), this, SLOT(CameraTimerTimeout()));

    if (m_qImage1) {
        *qImage1 = new QImage(*m_qImage1);
    }

    if (m_qImage2) {
        *qImage2 = new QImage(*m_qImage2);
    }

    connect(m_cameraTimer, SIGNAL(timeout()), this, SLOT(CameraTimerTimeout()));
}

void EstereoCameraCapturer::GetCapture(cv::Mat **cvImage1, cv::Mat **cvImage2) const {
    disconnect(m_cameraTimer, SIGNAL(timeout()), this, SLOT(CameraTimerTimeout()));

    if (m_qImage1) {
        *cvImage1 = new cv::Mat(m_image1.clone());
    }

    if (m_qImage2) {
        *cvImage2 = new cv::Mat(m_image2.clone());
    }

    connect(m_cameraTimer, SIGNAL(timeout()), this, SLOT(CameraTimerTimeout()));
}

EstereoCameraCapturer::~EstereoCameraCapturer()
{
    if(m_qImage1) {
        delete m_qImage1;
        m_qImage1 = 0;
    }

    if(m_qImage2) {
        delete m_qImage2;
        m_qImage2 = 0;
    }

    if(m_cameraTimer) {
        delete m_cameraTimer;
        m_cameraTimer = 0;
    }
}
