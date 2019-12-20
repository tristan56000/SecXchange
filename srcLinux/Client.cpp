#include "Client.h"
#include <QHostInfo>
#include <iostream>
#include <QNetworkConfigurationManager>
#include <QSettings>
#include "ThreadKeyboard.h"
#include "ExchangeI.h"
#include "Key.h"
#include "Invalid_Parameter.h"
#include <ctime>

extern QString GlobalmessageSend;
extern QString GlobalmessageRead;

/**
 * @brief Client::Client
 * override
 */
Client::Client(QString username,Key publicK, Key privateK, QWidget *parent) :
    ExchangeI(),
    QDialog(parent),
    UserRSA(publicK, privateK),
    username(username)

{

    try {
        if(username=="" or username==nullptr or publicK.getType()=="notype" or privateK.getType()=="notype") {
            Invalid_Parameter error = Invalid_Parameter();
            throw(error);
        }
    this->table=PublicKeyTable::getInstance();
    this->publicKey=publicK;
    this->privateKey=privateK;

        std::cout<< " My Username    : "<<username.toStdString()<<std::endl;
        //std::cout<< " My private Key : "<<privateKey.getPower()<<std::endl;
        std::cout<< " My public Key  : "<<publicKey.getPower()<<std::endl;



    std::cout<<" ----------------------------------------------------- " << std::endl;
    std::cout<<" ---             You choose client side            --- "<< std::endl;
    std::cout<<" ----------------------------------------------------- "<< std::endl;
    // find out name of this machine
    QString name = QHostInfo::localHostName();
    if (!name.isEmpty()) {
        //hostCombo->addItem(name);
        std::cout<< name.toStdString() << std::endl;
        QString domain = QHostInfo::localDomainName();
        if (!domain.isEmpty()) {
            //hostCombo->addItem(name + QChar('.') + domain);
            std::cout<< name.toStdString()<<"."<<domain.toStdString() << std::endl;
        }

    }
    if (name != QLatin1String("localhost")) {
        //hostCombo->addItem(QString("localhost"));
        std::cout<< " --- localhost" << std::endl;
    }
    // find out IP addresses of this machine
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // add non-localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (!ipAddressesList.at(i).isLoopback()) {
            //hostCombo->addItem(ipAddressesList.at(i).toString());
            std::cout<<" --- "<<ipAddressesList.at(i).toString().toStdString()<< std::endl;
        }
    }



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
        connect(networkSession, &QNetworkSession::opened, this, &Client::sessionOpened);

        networkSession->open();
    }
    this->getParam();
    }
    catch (Invalid_Parameter) {
        std::cout<< "Error during the client creation" << std::endl;
        this->exitApplication();
    }
}

/**
 * @brief Client::getParam
 * override
 */
void Client::getParam() {
    std::cout<<std::endl<< "--- Enter Address of Server : " << std::endl;
    std::string sIP;
    getline(std::cin,sIP);
    QString ip = QString::fromStdString(sIP);
    std::string sPort="0";
    while(std::stoi(sPort)<1024){
        std::cout<< " --- Enter Port (>1024) : " << std::endl;
        getline(std::cin,sPort);
    }
    this->connexion(ip,sPort);
}

/**
 * @brief Client::connexion
 * override
 */
void Client::connexion(QString ip, std::string sPort)
{
    this->tcpSocket=new QTcpSocket(this);
    connect(tcpSocket, &QTcpSocket::connected,this,&Client::connexionAccept);
    tcpSocket->connectToHost(ip,quint16(std::stoi(sPort)));

}

/**
 * @brief Client::sessionOpened
 * override
 */
void Client::sessionOpened()
{
    // Save the used configuration
    QNetworkConfiguration config = networkSession->configuration();
    QString id = config.identifier();

    QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
    settings.endGroup();
}

/**
 * @brief Client::read
 * override
 */
void Client::read()
{
    this->readMessage(tcpSocket);
    QString temp = GlobalmessageRead;
    //std::cout << "==> new message : "<< temp.toStdString()<<std::endl;
    temp = QString::fromStdString(decipherment(privateKey,temp.toStdString()));
    std::cout << "==> new message : " << temp.toStdString()<<std::endl;
    if(temp=="!quit") {
        this->exitApplication();
    }
}

/**
 * @brief Client::send
 * override
 */
void Client::send()
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
    if(go) {
        temp = QString::fromStdString(this->encryption(publicKeyServer,temp.toStdString()));
        GlobalmessageSend = temp;
        this->sendMessage(tcpSocket);
    }
}

/**
 * @brief Client::exitApplication
 * override
 */
void Client::exitApplication()
{
    this->close();
}

/**
 * @brief Client::connexionAccept
 * override
 */
void Client::connexionAccept() {
    bool auth = this->identification();
    if (auth) {
        connect(tcpSocket, &QIODevice::readyRead, this, &Client::read);
        ThreadKeyboard *thread = new ThreadKeyboard();
        connect(thread,&ThreadKeyboard::emitMessage,this,&Client::send);
        connect(thread,&ThreadKeyboard::close,this,&Client::exitApplication);
        connect(tcpSocket,&QTcpSocket::disconnected,this,&Client::exitApplication);
        thread->start();
        std::cout << "-----------------------------------------------------" <<std::endl;
        std::cout << "---                Discussion start               ---" <<std::endl;
        std::cout << "-----------------------------------------------------" <<std::endl;
    } else {
        std::cout<< " Wrong identification" << std::endl;
        exit(0);
    }

}

/**
 * @brief Client::identification
 * override
 */
bool Client::identification() {
    // envoie alea1 avec identité au serveur
    srand(unsigned(time(0)));
    int nb = rand();
    QString n = this->username+":"+QString::number(nb);
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);
    out << n;
    this->tcpSocket->write(block);
    tcpSocket->waitForBytesWritten();

    tcpSocket->waitForReadyRead();
    // reception de alea1 chiffré avec clé privée du serveur
    // + alea2 + identité serveur
    n="";
    QDataStream in;
    in.setDevice(tcpSocket);
    in.setVersion(QDataStream::Qt_5_10);
    in.startTransaction();
    in >> n;
    // dechiffre alea1;
    QStringList tab = n.split(":");
    Key pubKeyServer = this->table->getKey(tab[2].toStdString());
    int verifnb = stoi(this->decipherment(pubKeyServer,tab[0].toStdString()));
    if(verifnb!=nb) {
        return false;
    } else {
        // on chiffre avec notre clé publique et on envoie l'alea2 au serveur.
        std::string encodeAleat2 = this->encryption(this->privateKey,tab[1].toStdString());
        QString temp = QString::fromStdString(encodeAleat2);
        QByteArray block1;
        QDataStream out(&block1, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_10);
        out << temp;
        this->tcpSocket->write(block1);
        tcpSocket->waitForBytesWritten();
        this->publicKeyServer=pubKeyServer;
        return true;
    }
}
