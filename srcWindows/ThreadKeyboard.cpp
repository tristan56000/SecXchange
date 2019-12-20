#include "ThreadKeyboard.h"
#include <iostream>
#include <QDataStream>

extern QString GlobalmessageSend;

/**
 * @brief ThreadKeyboard::ThreadKeyboard
 * override
 */
ThreadKeyboard::ThreadKeyboard() {

}

/**
 * @brief ThreadKeyboard::run
 * override
 */
void ThreadKeyboard::run() {
    std::string message="";
    while(message!="!quit") {
        message="";
        getline(std::cin,message);
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_10);
        QString mess = QString::fromStdString(message);
        GlobalmessageSend = mess;
        this->emitMessage();
    }
    this->close();
    this->quit();
}

