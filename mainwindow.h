#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <QTimer>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include "clabel.h"
#include <chrono>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    cv::VideoCapture cap;
    cv::Mat frame;
    QPixmap pxmap;
    QTimer streaming_timer, statusbar_timer;
    bool streaming = false;
    CLabel* clabel = new CLabel(this);
    int timer_timeout;
    QString timestamp;
    QString output_path = ".";
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent* e);
    ~MainWindow();

private slots:
    void on_pushButton_3_clicked();
    void on_timer();
    void on_pushButton_clicked();
    void on_webcam();
    void about();

    void on_pushButton_2_clicked();
    void setPath();
    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
