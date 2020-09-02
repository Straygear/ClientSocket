#include "mainwindow.h"
#include "client.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    client = new CLIENT(__argv);
    ui->setupUi(this);
    connect(ui->lineEdit,SIGNAL(returnPressed()),ui->pushButton_2, SIGNAL(clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete client;
}


void MainWindow::on_pushButton_clicked()
{
        client->initialiseSocket();
        client->connectToServer();
       // client->recieveData();
}

void MainWindow::on_pushButton_2_clicked()
{
    client->sendData(ui->lineEdit->text().toStdString()); //convert Qstring to string to send
    ui->lineEdit->clear();
}

void MainWindow::on_pushButton_3_clicked()
{
    client->disconnectClient();
    if(client->connected()){
        ui->label->setText("Connected");
    }
    else{
        ui->label->setText("Disconnected");
    }
}

void MainWindow::changeEvent(QEvent* event) {
    event->setAccepted(client->recieveData());
    if (event->isAccepted()) {
        QString st = QString::fromStdString(client->getString());  //convert recieved string back to Qstring to display on text browser
        ui->textEdit->insertPlainText(st);
    }
}
