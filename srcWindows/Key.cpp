#include <iostream>
#include "Key.h"
#include <string>
using namespace std;

/**
 * @brief Key::Key
 * override
 */
Key::Key(int n, int power, string type)
{
    this->n = n;
    this->power = power;
    this->type = type;
}

/**
 * @brief Key::~Key
 * override
 */
Key::~Key() {
    /*delete &n;
    delete &power;
    delete &type;*/
    //cout << "dest key" << this->power <<endl;
}

/**
 * @brief Key::setN
 * override
 */
void Key::setN(int n){
    this->n = n;
}

/**
 * @brief Key::setPower
 * override
 */
void Key::setPower(int power) {
    this->power = power;
}

/**
 * @brief Key::setType
 * override
 */
void Key::setType(string type){
    this->type = type;
}

/**
 * @brief Key::getN
 * override
 */
int Key::getN() const{
    return this->n;
}

/**
 * @brief Key::getPower
 * override
 */
int Key::getPower() const{
    return this->power;
}

/**
 * @brief Key::getType
 * override
 */
string Key::getType() const{
    return this->type;
}

/**
 * @brief Key::operator==
 * override
 */
bool Key::operator==(bool pass) {
    bool t=false;
    if(this->type!="noType") {
        t=true;
    }
    if(pass == t) {
        return true;
    } else {
        return false;
    }
   }


