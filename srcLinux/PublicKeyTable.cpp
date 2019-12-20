#include <iostream>
#include "PublicKeyTable.h"
#include "Key.h"
#include <fstream>
#include <string>
#include <map>
#include <QDebug>
using namespace std;

PublicKeyTable *PublicKeyTable::singleton = nullptr;

/**
 * @brief PublicKeyTable::PublicKeyTable
 * override
 */
PublicKeyTable::PublicKeyTable(){
    //cout << "Creation" << endl;
    this->table =  new map<string,Key>();
    this->readDataFile();
}

/**
 * @brief PublicKeyTable::~PublicKeyTable
 * override
 */
PublicKeyTable::~PublicKeyTable(){
    //cout << "Destruction" << endl;
    //delete &table;
    //delete table;
}

/**
 * @brief PublicKeyTable::getInstance
 * override
 */
PublicKeyTable * PublicKeyTable::getInstance() {
    if (singleton == nullptr){
        singleton = new PublicKeyTable();
    }
    return singleton;
}

/**
 * @brief PublicKeyTable::kill
 * override
 */
void PublicKeyTable::kill() {
    if(singleton != nullptr){
        delete singleton;
        singleton = nullptr;}
}

/**
 * @brief PublicKeyTable::addKey
 * override
 */
void PublicKeyTable::addKey(string name, Key publicKey) {
    for(auto itr = this->table->begin(); itr != this->table->end(); ++itr) {
        if(itr->first == name){
            cout << "Public key of "+name+" already in table" << endl;
        }
    }
    this->table->insert(std::make_pair(name,publicKey));
}

/**
 * @brief PublicKeyTable::getKey
 * override
 */
Key PublicKeyTable::getKey(string name) {
    for(auto itr = this->table->begin(); itr != this->table->end(); ++itr) {
        if(itr->first == name){
            return itr -> second;
        }
    }
    cout << "User " +name+" not found in table" << endl;
    return Key(0,0,"notype");
}


/**
 * @brief PublicKeyTable::removeKey
 * override
 */
void PublicKeyTable::removeKey(string name) {
    if(this->table->find(name) != this->table->end()){
        this->table->erase(name);
    }else{
        cout << "No key with username :"+name << endl;
    }
}

/**
 * @brief PublicKeyTable::readDataFile
 * override
 */
void PublicKeyTable::readDataFile() {
    string path = "public_RSA_Keys.txt";
    ifstream file(path);
    if (!file) {
        cerr << "Unnacepted file opening "<<endl;
        return ;
    }
    while(!file.eof()) {
        string line;
        file >> line;
        std::string user;
        std::string key;
        size_t pos = 0;
        pos = line.find(":");
        user = line.substr(0, pos);
        key = line.substr(pos+1,std::string::npos);
        pos = key.find("-");
        std::string keyPart1 = key.substr(0,pos);
        std::string keyPart2 = key.substr(pos+1,std::string::npos);

        Key k = Key(std::stoi(keyPart1),std::stoi(keyPart2),"public");
        this->addKey(user,k);
    }
}
