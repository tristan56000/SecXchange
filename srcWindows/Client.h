#ifndef CLIENT_H
#define CLIENT_H

#include <QNetworkSession>
#include <QTcpSocket>
#include "ExchangeI.h"
#include "UserRSA.h"
#include "Key.h"
#include <QDialog>
#include "PublicKeyTable.h"

/**
 * @brief The Client class
 * Class client permit the discussion, a client should connect
 * on a server to start discussion
 */
class Client : public ExchangeI, public QDialog, public UserRSA
{
public:
    /**
     * constructor
     * @brief Client
     * @param parent
     * @throws Invalid_Parameter error
     */
    Client(QString username, Key publicKey, Key privateKey, QWidget *parent= nullptr);

private slots:

    /**
     * @brief sessionOpened
     * initialize network
     */
    void sessionOpened();

    /**
     * @brief send
     * send a message
     */
    void send();

    /**
     * @brief read
     * read a message
     */
    void read();

    /**
     * @brief exitApplication
     * to close application
     */
    void exitApplication();

    /**
     * @brief connexionAccept
     * if connection is accepted, print some text in terminal
     */
    void connexionAccept();

private:

    /**
     * @brief getParam
     * get the param at the user for the connection
     */
    void getParam();

    /**
     * @brief identification
     * establish authentification between two points
     */
     bool identification();

     /**
      * @brief connexion
      *  ask connection with a server
      * @param ip, ip of the server
      * @param port, port of the server
      */
     void connexion(QString ip, std::string port);

    /**
     * @brief networkSession
     * to config the network
     */
    QNetworkSession *networkSession = nullptr;

    /**
     * @brief tcpSocket
     * socket use for the transmission
     */
    QTcpSocket *tcpSocket;


    /**
     * @brief publicKeyServer
     * value to save the server public key, init null
     */
    Key publicKeyServer = Key(0,0,"notype");

    /**
     * @brief username
     * value to save the name enter by the user
     */
    QString username;

    /**
     * @brief table
     * DB of public keys and username
     */
    PublicKeyTable *table;

};
#endif // CLIENT_H
