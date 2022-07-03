#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "entrance.h"
#include "User/User.h"
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

public slots:
    void on_loginButton_clicked();

    void on_sendButton_clicked();

    void connectToServer(const QString& ip,const QString& port);

    void waitNewMessage();

    void checkNewMessage();
signals:
   void failedConnect();

private:
    Ui::MainWindow *ui;
    Entrance* entranceWindow;
    User user;
    std::thread checkNewMessageThread;
    QTimer* timer;
};
#endif // MAINWINDOW_H
