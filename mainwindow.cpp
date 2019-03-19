#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->verticalLayout_2->addWidget(clabel);
    ui->label->setText("Current Path for image output: " + output_path);
    connect(&statusbar_timer, &QTimer::timeout, this, [this](){ui->statusBar->showMessage("Selection Mode On...");});
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    qDebug() << "Key: " << e->key() << endl;
    switch(e->key()){
    case 83: {
        qDebug() << "Selection Mode on!" << endl;
        if(streaming){
            if(!clabel->selected() && !clabel->paused()){
                clabel->setPlayMode(false);
                clabel->setSelectionMode(true);
                streaming_timer.stop();
                ui->statusBar->showMessage("Selection Mode On...");
            }
        }
    };break;
    case 16777216: {
        qDebug() << "Escape pressed " << endl;
        if(streaming){
            if(clabel->selected() && clabel->paused()){
                clabel->setSelectionMode(false);
                clabel->setPlayMode(true);
                clabel->update();
                streaming_timer.start(timer_timeout);
                statusbar_timer.stop();
                ui->statusBar->showMessage("Playing...");
            }
        }
    };break;
    case 32: {
        qDebug() << "Space pressed! " << endl;
        if(streaming){
            if(clabel->paused() && clabel->selected()){
//                qDebug() << "do somethin to save the corpped images." << endl;
                timestamp = QString::number(std::chrono::milliseconds(std::time(nullptr)).count());
//                CRect rec = clabel->Roi();
//                cv::Rect roi(rec.x, rec.y, rec.width, rec.height);
                QPixmap roi = clabel->Roi();
                QFile file(output_path+"/"+timestamp+".png");
                file.open(QIODevice::WriteOnly);
                roi.save(&file, "PNG");
                ui->statusBar->showMessage("Image Saved!", 500);
                statusbar_timer.start(500);
//                cv::imwrite(output_path.toStdString()+timestamp.toStdString()+".png", frame(roi));
            }else if(!clabel->paused() && !clabel->selected()){
                clabel->setPlayMode(false);
                streaming_timer.stop();
                ui->statusBar->showMessage("Paused!");
            }else if(clabel->paused() && !clabel->selected()){
                clabel->setPlayMode(true);
                streaming_timer.start(timer_timeout);
                ui->statusBar->showMessage("Playing...");
            }else{

            }
        }
    }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_3_clicked()
{
    if(!streaming){
        cap.open(0);
        if(!cap.isOpened()){
            qDebug() << "cannot open webcam..." << endl;
        }else{
            streaming = true;
            ui->statusBar->showMessage("Playing...");
            connect(&streaming_timer, &QTimer::timeout, this, &MainWindow::on_timer);
            timer_timeout = 10;
            streaming_timer.start(timer_timeout);
        }
    }
}

void MainWindow::on_timer()
{
    try {
        cap >> frame;
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
        pxmap = QPixmap::fromImage(QImage(static_cast<unsigned char*>(frame.data), frame.cols, frame.rows, QImage::Format_RGB888));
        clabel->setPixelMap(pxmap);
        clabel->update();
    } catch (cv::Exception) {
        ui->statusBar->showMessage("Done!", 5000);
        on_pushButton_clicked();
    }
}

void MainWindow::on_pushButton_clicked()
{
    streaming = false;
    streaming_timer.stop();
    cap.release();
}

void MainWindow::on_webcam()
{
    on_pushButton_3_clicked();
}

void MainWindow::about()
{
    QMessageBox::about(this, "About", "Image Cropper\nAuthor: Pan\nVersion: 0.5");
}

void MainWindow::on_pushButton_2_clicked()
{
    if(!streaming){
        QString file = QFileDialog::getOpenFileName(this, "Files", QString(),
                                     tr("Video Files (*.mp4 *.avi *.mov *.m4v *.wmv *ogg)"));
        cap.open(file.toStdString());
        if(!cap.isOpened()){
            qDebug() << "cannot open video..." << endl;
        }else{
            streaming = true;
            connect(&streaming_timer, &QTimer::timeout, this, &MainWindow::on_timer);
            timer_timeout = 30;
            streaming_timer.start(timer_timeout);
        }
    }
}

void MainWindow::setPath()
{
    output_path = QFileDialog::getExistingDirectory(this, "set output path", QDir::homePath());
    if(output_path.isEmpty()){
        output_path = ".";
    }else{
        ui->label->setText("Current Path for image output: " + output_path);
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    this->close();
}
