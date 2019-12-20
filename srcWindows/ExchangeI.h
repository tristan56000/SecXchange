#ifndef EXCHANGEI_H
#define EXCHANGEI_H
#include <QTcpSocket>

/**
 * @brief The ExchangeI class
 * Class who permits to read and send messages
 */
class ExchangeI
{
public:
    /**
     * @brief ExchangeI
     * CONSTRUCTOR
     */
    ExchangeI();

    /**
     * @brief readMessage
     * read a message in a tcpSocket
     * @param tcpSocket
     */
    void readMessage(QTcpSocket *tcpSocket);

    /**
     * @brief sendMessage
     * write a message in a tcpsocket
     * @param tcpSocket
     */
    void sendMessage(QTcpSocket *tcpSocket);


};

#endif // EXCHANGEI_H
