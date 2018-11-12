#include "mainwindow.h"
#include "SkelRecorder.h"
#include <QApplication>
#include <QThread>

MainWindow* w;

class WorkerThread : public QThread
{
    Q_OBJECT
    void run() override
    {
        SkelRecorder recorder;
        recorder.init();

        cv::Mat color, depth;

        forever
        {
            if (!w) continue;
            
            recorder.getNextFrame(color, depth);
            QPixmap pixmap = QPixmap::fromImage(QImage((unsigned char*) color.data, color.cols, color.rows, QImage::Format_RGB888));
            w->setLabel(pixmap);
        }
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    w = new MainWindow();
    w->show();

    WorkerThread worker;
    worker.start();

    return a.exec();
}

#include "main.moc"
