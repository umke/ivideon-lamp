#include "connectionform.h"
#include "ui_connectionform.h"
#include "lamp.h"
#include "datatype.h"

#include <QDebug>
#include <QDataStream>
#include <QMessageBox>

ConnectionForm::ConnectionForm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ConnectionForm)
    , m_shpTcpSocket(new QTcpSocket)
    , m_shpCommand(new CommandData)
    , m_shpLamp(new Lamp)
{
    ui->setupUi(this);

    setMinimumSize(430, 600);
    m_shpCommand->type = 0;
    m_shpCommand->length = 0;

    ui->Lamp_vLayout->addWidget(m_shpLamp.data());
    ui->ServerNameLEdit->setText("emb2.extcam.xyz");
    ui->PortLEdit->setText("9991");

    connect(m_shpTcpSocket.data(), SIGNAL(connected()), this, SLOT(slotConnected()));
    connect(m_shpTcpSocket.data(), SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(m_shpTcpSocket.data(), SIGNAL(disconnected()), this, SLOT(slotConnectionClose()));
    connect(ui->ConnectButton, SIGNAL(clicked()), this, SLOT(slotConnectButtonClicked()));
    connect(ui->DisconnectButton, SIGNAL(clicked()), this, SLOT(slotDisconnectButtonClicked()));
    connect(m_shpTcpSocket.data(), SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(slotError(QAbstractSocket::SocketError)));

}

ConnectionForm::~ConnectionForm()
{
    if (m_shpTcpSocket->state() == QAbstractSocket::ConnectedState) {
        m_shpTcpSocket->disconnectFromHost();
    }
    delete ui;
}

void ConnectionForm::slotConnectButtonClicked()
{
    QString adress = "";
    QString port = "";
    if (ui->ServerNameLEdit->text().isEmpty() || ui->PortLEdit->text().isEmpty()) {
        QSharedPointer<QMessageBox> msgBox(new QMessageBox);
        msgBox->setWindowTitle("Фонарь");
        msgBox->setText("Внимание! Неободимо заполнить поля \"Адрес сервера\" и \"Порт\"!");
        msgBox->exec();
    } else {
        m_shpTcpSocket->connectToHost(ui->ServerNameLEdit->text().simplified()
                                      , ui->PortLEdit->text().simplified().toUInt());
    }
}


void ConnectionForm::slotDisconnectButtonClicked()
{
    if (m_shpTcpSocket->state() == QAbstractSocket::ConnectedState) {
        m_shpTcpSocket->disconnectFromHost();
        m_shpLamp->clearState();
    } else {
        QSharedPointer<QMessageBox> msgBox(new QMessageBox);
        msgBox->setWindowTitle("Фонарь");
        msgBox->setText("Внимание! Соединение не установлено!");
        msgBox->exec();
    }

}

void ConnectionForm::slotReadyRead()
{
    QDataStream in(m_shpTcpSocket.data());
    in.setVersion(QDataStream::Qt_5_4);
    for(;;){
        if (m_shpCommand->type == 0) {
            if (m_shpTcpSocket->bytesAvailable() < static_cast<qint64>(sizeof(quint8))) {
                break;
            }
            in >> m_shpCommand->type;
        }
        if (m_shpCommand->length == 0) {
            if (m_shpTcpSocket->bytesAvailable() < static_cast<qint64>(sizeof(quint16))) {
                break;
            }
            in >> m_shpCommand->length;
            if (m_shpCommand->length == 0) {
                m_shpLamp->setCommand(*m_shpCommand.data());
                m_shpCommand->type = 0;
                m_shpCommand->length = 0;
                m_shpCommand->value.clear();
            }
        }

        if (m_shpCommand->length > 0){
            if (m_shpTcpSocket->bytesAvailable() < m_shpCommand->length) {
                break;
            }
            for (size_t i = 0; i < m_shpCommand->length; ++i) {
                quint8 tmp;
                in >> tmp;
                m_shpCommand->value << tmp;
            }
            m_shpLamp->setCommand(*m_shpCommand.data());
            m_shpCommand->type = 0;
            m_shpCommand->length = 0;
            m_shpCommand->value.clear();
        }
    }
}

void ConnectionForm::slotConnected()
{
    qDebug() << "connected!";
    setControlsEnabled(false);
}

void ConnectionForm::slotConnectionClose()
{
    qDebug() << "connection is close!";
    setControlsEnabled(true);
}


void ConnectionForm::slotError(QAbstractSocket::SocketError err)
{
    QString strError = "Ошибка: " + (err == QAbstractSocket::HostNotFoundError ?
                                     "Сервер не найден." :
                                     err == QAbstractSocket::RemoteHostClosedError ?
                                     "Удаленный хост закрыт." :
                                     err == QAbstractSocket::ConnectionRefusedError ?
                                     "В соединении было отказано." :
                                     QString(m_shpTcpSocket->errorString()));

    QSharedPointer<QMessageBox> msgBox(new QMessageBox);
    msgBox->setWindowTitle("Фонарь");
    msgBox->setText(strError);
    msgBox->exec();
}

void ConnectionForm::setControlsEnabled(bool sign)
{
    ui->ServerNameLEdit->setEnabled(sign);
    ui->PortLEdit->setEnabled(sign);
}
