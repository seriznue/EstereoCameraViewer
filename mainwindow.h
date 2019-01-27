#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class EstereoCameraCapturer;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    EstereoCameraCapturer *m_estereoCameraCapturer;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showEvent(QShowEvent *event);

public slots:
    void ShowImageCaptured();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
