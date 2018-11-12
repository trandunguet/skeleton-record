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

    void setLabel(QPixmap& pixmap);
    void setButton(QString string);
    QPushButton* getButton();

private:
    QThread thread;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
