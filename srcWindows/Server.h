#ifndef SERVER_H
#define SERVER_H
#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkSession>
#include "ExchangeI.h"
#include "UserRSA.h"
#include "PublicKeyTable.h"

/**
 * @brief The Server class
 * class who need a new connection from a client to start the discussion
 */
class Server : public ExchangeI, public QDialog, public UserRSA
{

public:
    /**
     * @brief Server
     * CONSTRUCTOR
     * @param parent
     */
    Server(QString username, Key publicKey, Key privateKey, QWidget *parent = nullptr);

private slots:
    /**
     * @brief sessionOpened
     * initialise network
     */
    void sessionOpened();

    /**
     * @brief connexion
     * initialise and get the socket when a new connection is detect
     */
    void connexion();

    /**
     * @brief send
     * send message
     */
    void send();

    /**
     * @brief read
     * read message
     */
    void read();

    /**
     * @brief exitApplication
     * close application
     */
    void exitApplication();

private:
    /**
     * @brief identification
     * establish authentification between two points
     */
    bool identification();

    /**
     * @brief server
     * QTcpServer to listen a new connection
     */
    QTcpServer *server = nullptr;

    /**
     * @brief tcpSocket
     * QTcpSocket to exchange with client
     */
    QTcpSocket *tcpSocket;

    /**
     * @brief networkSession
     * to configure network
     */
    QNetworkSession *networkSession = nullptr;


    /**
     * @brief publicKeyClient
     * value to save the client public key, init null
     */
    Key publicKeyClient = Key(0,0,"notype");

    /**
     * @brief username
     * value to save the name enter by the user
     */
    QString username;

    /**
     * @brief table
     * BDD of public key and username
     */
    PublicKeyTable *table;
};

#endif // SERVER_H
