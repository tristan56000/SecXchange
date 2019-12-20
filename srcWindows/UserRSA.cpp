#include "UserRSA.h"
#include <cstring>
#include <vector>
#include <bits/stdc++.h>
#include <iostream>
using namespace std;

/**
 * @brief UserRSA::UserRSA
 * override
 */
UserRSA::UserRSA(Key publicKey, Key privateKey) :
    publicKey(publicKey),
    privateKey(privateKey)
{
}

/**
 * Destructs object
 */
UserRSA::~UserRSA() {
    /*delete &publicKey;
    delete &privateKey;*/
    //cout << "dest userRsa" << endl;
}



/**
 * @brief UserRSA::getPublicKey
 * override
 */
Key UserRSA::getPublicKey() {
    return this->publicKey;
}

/**
 * @brief UserRSA::getPrivateKey
 * override
 */
Key UserRSA::getPrivateKey(){
    return this->privateKey;
}

/**
 * @brief UserRSA::getCharFromString
 * override
 */
vector<char> UserRSA::getCharFromString(string s) {
    std::vector<char> data;
    copy( s.begin(), s.end(), std::back_inserter(data));
    return data;
}

/**
 * @brief UserRSA::getStringFromChar
 * override
 */
string UserRSA::getStringFromChar(vector<char> c){
    return string(c.begin(),c.end());
}

/**
 * @brief UserRSA::getIntFromChar
 * override
 */
int UserRSA::getIntFromChar(char c){
    return int(c);
}

/**
 * @brief UserRSA::getCharFromInt
 * override
 */
char UserRSA::getCharFromInt(int s){
    return static_cast<char>(s);
}

/**
 * @brief UserRSA::getBitsArrayFromInt
 * override
 */
bitset<8> UserRSA::getBitsArrayFromInt(int s){
    return bitset<8>(s);
}

/**
 * @brief UserRSA::getIntFromBitsArray
 * override
 */
int UserRSA::getIntFromBitsArray(bitset<8> b){
    return int(b.to_ullong());
}

/**
 * @brief UserRSA::getBitsArrayFromInt2
 * override
 */
bitset<20> UserRSA::getBitsArrayFromInt2(int s){
    return bitset<20>(s);
}

/**
 * @brief UserRSA::getIntFromBitsArray2
 * override
 */
int UserRSA::getIntFromBitsArray2(bitset<20> b){
    return int(b.to_ullong());
}

/**
 * @brief UserRSA::aPuisBModuloN
 * override
 */
long long int UserRSA::aPuisBModuloN(long long int a, long long int b, long long int mod) {
    long long int aReste = a%mod;
    for (long long int i = 0; i < b-1; ++i) {
        aReste = aReste*a;
        aReste = aReste%mod;
    }return aReste;
}

/**
 * @brief UserRSA::encrypt
 * override
 */
long long int UserRSA::encrypt(long long int M, long long int n, long long int power) {
    long long int C = this->aPuisBModuloN(M,power,n);
    return C;
}

/**
 * @brief UserRSA::decrypt
 * override
 */
long long int UserRSA::decrypt(long long int toDecrypt,long long int n, long long int power){
    long long int D = this->aPuisBModuloN(toDecrypt,power,n);
    return D;
}

/**
 * @brief UserRSA::encryption
 * override
 */
string UserRSA::encryption(Key publicKey, string message) {
    vector<int> allInt;
    vector<char> charVector = this->getCharFromString(message);
    vector<char>::iterator iter;
    for(iter = charVector.begin(); iter!=charVector.end(); ++iter){
        int s = getIntFromChar(*iter);
        bitset<8> vInt = getBitsArrayFromInt(s);
        for (int i = 7; i >= 0; --i) {
            allInt.push_back(vInt[i]);
        }
    }int m = (allInt.size()%20);
    if(m!=0){
        vector<int> full(20-m,0);
        allInt.insert(allInt.end(),full.begin(),full.end());
    }
    vector<long long int> cryptedInt;
    int step=20;
    while(static_cast<unsigned int>(step) <= allInt.size()){
        vector<long long int> decoup;
        decoup.insert(decoup.begin(),allInt.begin()+step-20,allInt.begin()+step);
        bitset<20> b;
        size_t i = 0;
        for_each(decoup.begin(), decoup.end(), [&](uint8_t val)
        { b[19-i] = val; ++i; });
        long long int M = this->getIntFromBitsArray2(b);
        //cout << M << "  ";
        long long int C = this->encrypt(M,(long long)publicKey.getN(),(long long)publicKey.getPower());
        //cout << C << endl;
        cryptedInt.push_back(C);
        step = step + 20;
    }
    //cout << cryptedInt.size()<< endl;
    string encrypted = this->toString(cryptedInt);
    //cout << encrypted << endl;
    return encrypted;
}

/**
 * @brief UserRSA::decipherment
 * override
 */
string UserRSA::decipherment(Key key,string encrypted){
    //cout << encrypted << endl;
    vector<long long int> cryptedInt = this->toVector(encrypted);
    //cout << cryptedInt.size()<< endl;
    vector<int> decryptedInt;
    vector<long long int>::iterator it;
    for(it = cryptedInt.begin(); it!=cryptedInt.end(); ++it) {
        //cout << *it << "  ";
        long long int D = this->decrypt(*it, (long long)key.getN(), (long long)key.getPower());
        //cout << D << endl;
        bitset<20> decryptBits = getBitsArrayFromInt2(D);
        for (int i = 19; i >= 0; --i) {
            decryptedInt.push_back(decryptBits[i]);
        }
    }
    int step=8;
    bitset<8> empty(string("00000000"));
    vector<char> character;
    while(static_cast<unsigned int>(step) <= decryptedInt.size()){
        vector<int> decoup;
        decoup.insert(decoup.begin(),decryptedInt.begin()+step-8,decryptedInt.begin()+step);
        bitset<8> b;
        size_t i = 0;
        for_each(decoup.begin(), decoup.end(), [&](uint8_t val)
        { b[7-i] = val; ++i; });
        if(b == empty){break;}
        int s = getIntFromBitsArray(b);
        char c = getCharFromInt(s);
        character.push_back(c);
        step += 8;
    }
    string message = getStringFromChar(character);
    return message;
}

/**
 * @brief UserRSA::toString
 * override
 */
string UserRSA::toString(vector<long long int> cryptedInt) {
    string toRet = "";
    vector<long long int>::iterator it;
    for(it = cryptedInt.begin(); it!=cryptedInt.end(); ++it) {
        toRet = toRet + to_string(*it) + ";";
    }
    return toRet;
}

/**
 * @brief UserRSA::toVector
 * override
 */
vector<long long int> UserRSA::toVector(string str) {
    vector<long long int> cryptedInt;
    size_t pos = 0;
    std::string token;
    while ((pos = str.find(";")) != std::string::npos) {
        token = str.substr(0, pos);
        str.erase(0, pos + 1);char *end;
        long long int temp = strtoull(token.c_str(),&end,10);
        cryptedInt.push_back(temp);
    }
    return cryptedInt;
}
