#ifndef CONNECTIONFORM_H
#define CONNECTIONFORM_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QSharedPointer>

QT_BEGIN_NAMESPACE
namespace Ui { class ConnectionForm; }
QT_END_NAMESPACE

class Lamp;

struct CommandData;

class ConnectionForm : public QMainWindow
{
    Q_OBJECT

public:
    ConnectionForm(QWidget *parent = nullptr);
    ~ConnectionForm();
private:
    void setControlsEnabled(bool);

private:
    Ui::ConnectionForm *ui;

    QSharedPointer<QTcpSocket> m_shpTcpSocket;
    QSharedPointer<CommandData> m_shpCommand;
    QSharedPointer<Lamp> m_shpLamp;
private slots:
    void slotConnectButtonClicked();
    void slotDisconnectButtonClicked();
    void slotReadyRead();
    void slotConnected();
    void slotConnectionClose();
    void slotError(QAbstractSocket::SocketError);

};
#endif // CONNECTIONFORM_H
