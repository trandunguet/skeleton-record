#include "mainwindow.h"
#include "SkelRecorder.h"
#include <QApplication>
#include <QThread>

MainWindow* w;

class WorkerThread : public QThread
{
    Q_OBJECT
private:
    SkelRecorder recorder;
    void run() override
    {
        recorder.init();
        w->setStatus("Ready");
        cv::Mat color, depth;

        forever
        {
            if (!w) continue;
            
            recorder.getNextFrame(color, depth);

            cv::cvtColor(color, color, CV_BGR2RGB);
            cv::cvtColor(depth, depth, CV_BGR2RGB);
            QPixmap pixmapColor = QPixmap::fromImage(QImage(color.data, color.cols, color.rows, QImage::Format_RGB888));
            QPixmap pixmapDepth = QPixmap::fromImage(QImage(depth.data, depth.cols, depth.rows, QImage::Format_RGB888));
            w->setLabels(pixmapColor, pixmapDepth);
        }
    }

public slots:
    void button_onClicked()
    {
        if (recorder.isRecording)
        {
            recorder.stopRecording();
            w->setButton("Record");
            w->setStatus("Ready");
        }
        else
        {
            recorder.startRecording();
            w->setButton("Stop");
            w->setStatus("Recording");
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

    QObject::connect(w->getButton(), SIGNAL(clicked()), &worker, SLOT(button_onClicked()));

    return a.exec();
}

#include "main.moc"
