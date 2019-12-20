#include "ExchangeI.h"
#include <QDataStream>
#include <QTcpSocket>
#include <iostream>

extern QString GlobalmessageSend;
extern QString GlobalmessageRead;

/**
 * @brief ExchangeI::ExchangeI
 * override
 */
ExchangeI::ExchangeI() {
}

/**
 * @brief ExchangeI::readMessage
 * override
 */
void ExchangeI::readMessage(QTcpSocket *tcpSocket)
{
    QDataStream in;
    in.setDevice(tcpSocket);
    in.setVersion(QDataStream::Qt_5_10);
    in.startTransaction();
    in >> GlobalmessageRead;
    if (!in.commitTransaction())
        return;
}

/**
 * @brief ExchangeI::sendMessage
 * override
 */
void ExchangeI::sendMessage(QTcpSocket *tcpSocket)
{
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_10);
        out << GlobalmessageSend;
        tcpSocket->write(block);
        tcpSocket->waitForBytesWritten();

}
