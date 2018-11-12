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
            QPixmap pixmap = QPixmap::fromImage(QImage((unsigned char*) color.data, color.cols, color.rows, QImage::Format_RGB888));
            w->setLabel(pixmap);
        }
    }

public slots:
    void button_onClicked()
    {
        std::cout << "clicked" << std::endl;
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
