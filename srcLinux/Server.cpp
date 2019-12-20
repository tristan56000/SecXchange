
#include <iostream>
#include <QNetworkConfigurationManager>
#include <QSettings>
#include <QMessageBox>
#include "Server.h"
#include "ThreadKeyboard.h"
#include <QDebug>
#include "Invalid_Parameter.h"
#include <ctime>

extern QString GlobalmessageSend;
extern QString GlobalmessageRead;

/**
 * @brief Server::Server
 * override
 */
Server::Server(QString username, Key publicK, Key privateK,QWidget *parent):
    ExchangeI(),
    QDialog(parent),
    UserRSA(publicK,privateK),
    username(username)
{
    try {
        if(username=="" or username==nullptr or publicK.getType()=="notype" or privateK.getType()=="notype") {
            Invalid_Parameter error = Invalid_Parameter();
            throw(error);
        }
    this->table=PublicKeyTable::getInstance();
     this->privateKey=privateK;
     this->publicKey=publicK;

    std::cout<<" ----------------------------------------------------- " << std::endl;
    std::cout<<" ---             You choose server side            --- "<< std::endl;
    std::cout<<" ----------------------------------------------------- "<< std::endl;
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    // manages the network configurations provided by the system
    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {

        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        config = manager.defaultConfiguration();

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, &QNetworkSession::opened, this, &Server::sessionOpened);

        networkSession->open();
    } else {
        sessionOpened();
    }
}
catch (Invalid_Parameter) {
    std::cout<< "error during the server creation" << std::endl;
    this->exitApplication();
}

}

/**
 * @brief Server::sessionOpened
 * override
 */
void Server::sessionOpened()
{
    if (networkSession) {
        QNetworkConfiguration config = networkSession->configuration();
        QString id = config.identifier();

        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
        settings.endGroup();
    }

    server = new QTcpServer(this);

    // choose the port to open server
    std::string sPort="0";
    while (stoi(sPort)<1024) {
        std::cout << " --> enter the port to open server (>1024) : " <<std::endl;
        getline(std::cin,sPort);
    }

    // find the IP of server
    QHostAddress ip;
    bool find=false;
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost) {
            std::cout << "do you want to choose this ip (y,n) : "<<address.toString().toStdString()<<std::endl;
            std::string choose;
            getline(std::cin,choose);
            if (choose == "y") {
                ip = address;
                find=true;
                break;
            }
        }
    }
    if(!find){
        std::cout << " you don't have IP correct"<<std::endl;
        this->close();
    }

    server->listen(ip,quint16(std::stoi(sPort)));
    connect(server, &QTcpServer::newConnection, this, &Server::connexion);
    std::cout << " --> The server is running on "<< server->serverAddress().toString().toStdString() << std::endl;
    std::cout << " --> Port: "<<server->serverPort() << std::endl;
    std::cout << " --> Wait connection ... " <<std::endl;

}

/**
 * @brief Server::connexion
 * override
 */
void Server::connexion()
{
    tcpSocket = server->nextPendingConnection();
    std::cout << " new connection detected with " << tcpSocket->localAddress().toString().toStdString() <<std::endl;
    bool auth = this->identification();
    if (auth) {
        connect(tcpSocket,&QIODevice::readyRead,this,&Server::read);
        ThreadKeyboard *thread = new ThreadKeyboard();
        connect(thread,&ThreadKeyboard::emitMessage,this,&Server::send);
        connect(thread,&ThreadKeyboard::close,this,&Server::exitApplication);
        connect(tcpSocket,&QTcpSocket::disconnected,this,&Server::exitApplication);
        thread->start();
        std::cout << "-----------------------------------------------------" <<std::endl;
        std::cout << "---                discussion start               ---" <<std::endl;
        std::cout << "-----------------------------------------------------" <<std::endl;
    }
    else {
        std::cout<< " wrong identification" << std::endl;
        this->exitApplication();
    }
}

/**
 * @brief Server::read
 * override
 */
void Server::read()
{
    this->readMessage(tcpSocket);
    QString temp = GlobalmessageRead;
    //std::cout << "==> new message : " << temp.toStdString()<<std::endl;
    temp = QString::fromStdString(decipherment(privateKey,temp.toStdString()));
    std::cout << "==> new message : " << temp.toStdString()<<std::endl;
    if(temp=="!quit") {
        this->exitApplication();
    }
}

/**
 * @brief Server::send
 * override
 */
void Server::send()
{
    QString temp = GlobalmessageSend;
    bool go=false;
    for(int i =0;i<temp.length();i++) {
        if(temp[i]!=' ') {
            if (temp[i]!='\n') {
                if (temp[i]!='\v') {
                    if (temp[i]!='\r') {
                        if (temp[i]!='\t') {
                            go=true;
                        }
                    }
                }
            }
        }
    }
    if (go) {
        temp = QString::fromStdString(this->encryption(publicKeyClient,temp.toStdString()));
        GlobalmessageSend = temp;
        this->sendMessage(tcpSocket);
    }
}
/**
 * @brief Server::exitApplication
 * override
 */
void Server::exitApplication(){
    this->close();
}

/**
 * @brief Server::identification
 * override
 */
bool Server::identification() {

    // generate the alea2
    srand(unsigned(time(0)));
    int nb = rand();
    tcpSocket->waitForReadyRead();

    //reception de idClient+alea1
    QString n;
    QDataStream in;
    in.setDevice(tcpSocket);
    in.setVersion(QDataStream::Qt_5_10);
    in.startTransaction();
    in >> n;
    QStringList tab = n.split(":");
    QString clientName = tab[0];
    //chiffre alea1 avec cléPrivé
    std::string temp = this->encryption(this->privateKey,tab[1].toStdString());
    QString send = QString::fromStdString(temp)+":"+QString::number(nb)+":"+this->username;

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);
    out << send;
    this->tcpSocket->write(block);
    tcpSocket->waitForBytesWritten();
    tcpSocket->waitForReadyRead();
    // reception de nbaleat
    n="";
    QDataStream in1;
    in1.setDevice(tcpSocket);
    in1.setVersion(QDataStream::Qt_5_10);
    in1.startTransaction();
    in1 >> n;
    Key pubKeyClient = this->table->getKey(clientName.toStdString());

    string tmp = this->decipherment(pubKeyClient,n.toStdString());
    int verifnb = std::stoi(tmp);
    if (verifnb==nb) {
        this->publicKeyClient=pubKeyClient;
        return true;
    } else {
        return false;
    }


}
