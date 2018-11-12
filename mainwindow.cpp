#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setLabels(QPixmap& pixmapColor, QPixmap& pixmapDepth)
{
    this->ui->labelColor->setPixmap(pixmapColor);
    this->ui->labelDepth->setPixmap(pixmapDepth);
}

void MainWindow::setButton(QString string)
{
    this->ui->button->setText(string);
}

QPushButton* MainWindow::getButton()
{
    return this->ui->button;
}

void MainWindow::setStatus(QString string)
{
    this->ui->statusBar->showMessage(string);
}
