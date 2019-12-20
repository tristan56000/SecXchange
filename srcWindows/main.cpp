#include <QApplication>
#include <iostream>
#include "Client.h"
#include "Server.h"
#include "Key.h"
#include "PublicKeyTable.h"
#include <QCryptographicHash>
#include <QFile>
#include <stdio.h>
#include <conio.h>


/**
 * @brief GlobalmessageSend
 * global variable use to send messages
 */
QString GlobalmessageSend;

/**
 * @brief GlobalmessageRead
 * global variable use to read messages
 */
QString GlobalmessageRead;

/**
 * @brief readData
 * Gets the private key of the user from the encrypted file "private_RSA_Keys.txt"
 * @return a private key string
 */
QString readData() {
    QString path = "private_RSA_Keys.txt";

    std::ifstream f(path.toStdString(), std::ios::binary | std::ios::in);
    std::string line="";
    f >> line;
    QString liste="";
    liste = QString::fromStdString(line);
    return liste ;

}

/**
 * @brief stringToBinary
 * changes a QString into a binary
 * @param ligne, QString to change
 * @return QString of binary
 */
QString stringToBinary(QString ligne) {
    QString text= "";
    std::string tmp = ligne.toStdString();
   for (int i=0; i<ligne.size();i++) {
       char c = tmp[i];
       for (int i = 7; i >= 0; i--) { // or (int i = 0; i < 8; i++)  if you want reverse bit order in bytes
           int tmp = ((c >> i) & 1);
           text+=QString::number(tmp);
       }
   }
   return text;

}

/**
 * @brief XOR
 * encrypts a text using XOR method
 * @param text, QString to encrypt
 * @param hash, QString key
 * @return QString encrypted
 */
QString XOR( QString text, QString hash) {
    QString convert = "";
    for(int i = 0; i<text.size();i++) {
        int tmp = i%hash.size();
        if(text[i]=="0") {
            if (hash[tmp]=="0") {
                convert+="0";
            } else {
                convert+="1";
            }
        } else {
            if(hash[tmp]=="0") {
                convert+="1";
            } else {
                convert+="0";
            }
        }
    }
    return convert;
}


/**
 * @brief binaryToString
 * changes a binary into a QString
 * @param text, binary to change
 * @return a QString
 */
QString binaryToString( QString text) {
    QString result = "";

    for(int i=0; i<text.length();i=i+8){
        int a =0;
        if (text[i]=="1"){
            a+=pow(2,7);
        }
        if (text[i+1]=="1"){
            a+=pow(2,6);
        }
        if (text[i+2]=="1"){
            a+=pow(2,5);
        }
        if (text[i+3]=="1"){
            a+=pow(2,4);
        }
        if (text[i+4]=="1"){
            a+=pow(2,3);
        }
        if (text[i+5]=="1"){
            a+=pow(2,2);
        }
        if (text[i+6]=="1"){
            a+=pow(2,1);
        }
        if (text[i+7]=="1"){
            a+=pow(2,0);
        }
        char t = a;

        std::string tm1(1,t);
        result+=QString::fromStdString(tm1);
    }
    return result;
}

/**
 * @brief getPrivateKey
 * Gets a private key from a QString
 * @param str QString of key
 * @return a private key
 */
Key getPrivateKey(QString str) {
    Key k=Key(0,0,"noType");

    if(str.contains(':') && str.contains('-')){
        std::string user;
        std::string key;
        std::string line = str.toStdString();
        size_t pos = 0;
        pos = line.find(":");
        user = line.substr(0, pos);
        key = line.substr(pos+1,std::string::npos);
        pos = key.find("-");
        std::string keyPart1 = key.substr(0,pos);
        std::string keyPart2 = key.substr(pos+1,std::string::npos);

        k = Key(std::stoi(keyPart1),std::stoi(keyPart2),"private");
    }
    return k;
}


/**
 * @brief authentification
 * check if user enter a good password
 * @return key of user
 */
Key authentification(){

    string password;

    //struct utsname name;
    //uname(&name);
    //std::cout << name.sysname<<std::endl;

    //static struct termios old, current;

    //tcgetattr(0, &old); /* grab old terminal i/o settings */
    //current = old; /* make new settings same as old settings */
    //current.c_lflag &= ~ICANON; /* disable buffered i/o */
    //current.c_lflag &= ~ECHO; /* set no echo mode */
    //tcsetattr(0, TCSANOW, &current);

    cout << endl << "Enter your password" << endl;

    char c = getch();
    while(c != 13){
        password.push_back(c);
        //cout << password <<endl;
        c = getch();
    }

    //getline(std::cin,password);

    //tcsetattr(0, TCSANOW, &old);


    //std::cout<< password << std::endl;

    QString tmp = QString::fromStdString(password);
    QString pass = QString(QCryptographicHash::hash(tmp.toUtf8(),QCryptographicHash::Sha512).toHex());
    QString test = stringToBinary(pass);
    QString liste = readData();
    QString XORstring = XOR(liste,test);
    QString result = binaryToString(XORstring);

    //std::cout << "key to string : "<< result.toStdString() << std::endl;

    Key k = getPrivateKey(result);
    return k;
}




/**
 * @brief main
 * Launch the application
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::cout<<" ################################################################### "<< std::endl;
    std::cout<<" ###                         SecXchange                         #### "<< std::endl;
    std::cout<<" ################################################################### "<< std::endl;


    //---------------------------------- authentification with pass ----------------------------------
    std::cout << "-----------------------------------------------------" <<std::endl;
    std::cout << "---                AUTHENTIFICATION               ---" <<std::endl;
    std::cout << "-----------------------------------------------------" <<std::endl;
    cout << endl << "Enter your username" << endl;
    std::string username;
    getline(std::cin,username);
    QString nameUser = QString::fromStdString(username);

    Key keyPriv = authentification();
    if(keyPriv==true){
        cout << "\nAccess granted\n";
    }else{
        cout << "\nAccess aborted...\n";
        exit(0);
    }
    // ------------------------------- ------------------------------- -------------------------------

    PublicKeyTable *table = PublicKeyTable::getInstance();
    Key keyPub =  table->getKey(nameUser.toStdString());
    std::cout<< " My Username    : "<<nameUser.toStdString()<<std::endl;
    //std::cout<< " My private Key : "<<keyPriv.getPower()<<std::endl;
    std::cout<< " My public Key  : "<<keyPub.getPower() << " - "<<keyPub.getN()<<std::endl;
    std::cout<<" ### choose your deployment : "<<std::endl;
    std::cout<<" ### > enter 1 to be client" << std::endl;
    std::cout<<" ### > enter 2 to be server"<<std::endl;
    std::string response="";
    getline(std::cin,response);
    if(std::stoi(response)==1) {
        Client client(nameUser,keyPub,keyPriv);
        return a.exec();
    } else {
        Server server(nameUser,keyPub,keyPriv);
        return a.exec();
    }

}


