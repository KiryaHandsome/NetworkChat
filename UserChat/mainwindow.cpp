#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <mutex>


std::string newMessage;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), user()
{
    ui->setupUi(this);

    ui->sendButton->setEnabled(false);

    entranceWindow = new Entrance();
    connect(entranceWindow, SIGNAL(sendLoginInfo(const QString&,const QString&)),
            SLOT(connectToServer(const QString&,const QString&)));

    entranceWindow->show();

}

MainWindow::~MainWindow()
{
    delete timer;
    delete ui;
}


void MainWindow::on_loginButton_clicked()
{
    QString nickname = ui->nicknameEdit->text();
    if(nickname != "" && nickname != " ") {
        user.sendNickname(nickname.toStdString().c_str());

        ui->sendButton->setEnabled(true);
        ui->loginButton->setEnabled(false);

        timer = new QTimer();
        connect(timer, SIGNAL(timeout()), SLOT(checkNewMessage()));
        timer->start(50);

        checkNewMessageThread = std::thread(&MainWindow::waitNewMessage, this);
        checkNewMessageThread.detach();
    }
    else {
        QMessageBox::information(this, "Info", "Incorrect nickname");
    }
}


void MainWindow::on_sendButton_clicked()
{
    QString message = ui->messageEdit->text();
    ui->messageEdit->clear();
    user.sendMessageToServer(message.toStdString().c_str());
}


void MainWindow::connectToServer(const QString& ip,const QString& port)
{
    //try connect to server
    if(user.startup(ip.toStdString(), port.toStdString())) {
        //remove enter window
        delete entranceWindow;
        this->show();
    }
    else {
        emit failedConnect();
    }
}


void MainWindow::waitNewMessage()
{
    while(true) {
        std::string s = user.receiveMessageFromServer();
        if(s != "") {
            size_t pos = s.find_first_of('\n');
            s.erase(pos, s.size() - pos);
            newMessage = s;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

}

void MainWindow::checkNewMessage()
{
    if(newMessage != "") {
        ui->chatBrowser->setText(ui->chatBrowser->toPlainText() + QString::fromStdString(newMessage) + '\n');
        newMessage = "";
    }
}

