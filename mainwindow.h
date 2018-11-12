#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QThread>
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setLabels(QPixmap& pixmapColor, QPixmap& pixmapDepth);
    void setButton(QString string);
    void setStatus(QString string);
    QPushButton* getButton();

private:
    QThread thread;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
