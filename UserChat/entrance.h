#ifndef ENTRANCE_H
#define ENTRANCE_H

#include <QWidget>

namespace Ui {
class Entrance;
}

class Entrance : public QWidget
{
    Q_OBJECT

public:
    explicit Entrance(QWidget *parent = nullptr);
    ~Entrance();

public slots:
    void on_enterButton_clicked();

    void failedConnect_slot();

signals:
    void sendLoginInfo(const QString& ipAddress, const QString& port);

private:
    Ui::Entrance *ui;
};

#endif // ENTRANCE_H
