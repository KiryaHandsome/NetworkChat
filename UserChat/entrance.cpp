#include "entrance.h"
#include "ui_entrance.h"
#include <QMessageBox>

Entrance::Entrance(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Entrance)
{
    ui->setupUi(this);
}

Entrance::~Entrance()
{
    delete ui;
}

void Entrance::on_enterButton_clicked()
{
    QString IPaddress = ui->IPEdit->text();
    QString port = ui->PortEdit->text();
    QStringList ipList = IPaddress.split('.');
    if(ipList.size() != 4 && IPaddress != "localhost") {
        QMessageBox::information(this, "Information", "Incorrect ip-address.");
    }
    else if(IPaddress != "localhost" && ((ipList[0].toInt() == 0 && ipList[0] != "0") || (ipList[1].toInt() == 0 && ipList[1] != "0") ||
            (ipList[2].toInt() == 0 && ipList[2] != "0") || (ipList[3].toInt() == 0 && ipList[3] != "0"))) {
        QMessageBox::information(this, "Information", "Incorrect symbols in ip-address.");
    }
    else if(port.toInt() == 0) {
        QMessageBox::information(this, "Information", "Incorrect port");
    }
    else {
        emit sendLoginInfo(IPaddress, port);
    }
}

void Entrance::failedConnect_slot()
{
    QMessageBox::information(this, "Oops", "Can't connect to this server.");
}

