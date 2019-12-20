#ifndef USERRSA_H
#define USERRSA_H
#include "Key.h"
#include <string>
#include <vector>
#include <bits/stdc++.h>
using namespace std;


class UserRSA
{
public:
    // Constructors & destructors

    //UserRSA();
    /**
   * @brief UserRSA
   * Construtor
   * @param publicKey, public key of user
   * @param privateKey, private key of user
   */
    UserRSA(Key publicKey, Key privateKey);

     /**
    * Destructor
    */
    ~UserRSA();

    //Getters and setters
    //void setPublicKey(Key publicKey);
    //void setPrivateKey(Key privateKey);

    /**
   * @brief getPublicKey
   * get the public key of user
   * @return the public key
   */
    Key getPublicKey();

    /**
   * @brief getPrivateKey
   * get the private key of user
   * @return the private key
   */
    Key getPrivateKey();

    //Different functions
    /**
   * @brief encryption
   * using RSA protocole to encrypt string
   * @param key, key used to encrypt
   * @param message, message to encrypt
   * @return string encrypt
   */
    string encryption(Key key, string message);

    /**
   * @brief decipherment
   * using RSA protocole to decipher string
   * @param key, key used to decipher
   * @param v, message to decipher
   * @return string clear
   */
    string decipherment(Key key, string v);

private:
    /**
   * @brief getCharFromString
   * Gets a vector of char from a string
   * @param s, string to convert
   * @return vector of char
   */
    vector<char> getCharFromString(string s);

    /**
   * @brief getStringFromChar
   * Gets a string from a vector of char
   * @param c, vector of char to convert
   * @return string
   */
    string getStringFromChar(vector<char> c);

    /**
   * @brief getIntFromChar
   * Gets an int from a char
   * @param c, char to convert
   * @return int
   */
    int getIntFromChar(char c);

    /**
   * @brief getCharFromInt
   * Gets a char from an int
   * @param s, int to convert
   * @return char
   */
    char getCharFromInt(int s);

    /**
   * @brief getBitsArrayFromInt
   * Gets a bitset from an int
   * @param s, int to convert
   * @return bitset<8>
   */
    bitset<8> getBitsArrayFromInt(int s);

    /**
   * @brief getIntFromBitsArray
   * Gets an int from a bitset
   * @param b bitset<8> to convert
   * @return int
   */
    int getIntFromBitsArray(bitset<8> b);

    /**
   * @brief getBitsArrayFromInt2
   * Gets a bitset from an int
   * @param s int to convert
   * @return bitset<20>
   */
    bitset<20> getBitsArrayFromInt2(int s);

    /**
   * @brief getIntFromBitsArray2
   * Gets an int from a bitset
   * @param b bitset<20> to convert
   * @return int
   */
    int getIntFromBitsArray2(bitset<20> b);

    /**
   * @brief encrypt
   * Calculates the encrypted C from M using RSA
   * @param toEncrypt long long int
   * @param n long long int
   * @param power long long int
   * @return long long int
   */
    long long int encrypt(long long int toEncrypt,long long int n,long long int power);

    /**
   * @brief decrypt
   * Calculates the decrypted D from toDecrypt using RSA
   * @param toDecrypt long long int
   * @param n long long int
   * @param power long long int
   * @return long long int
   */
    long long int decrypt(long long int toDecrypt,long long int n, long long int power);

    /**
   * @brief aPuisBModuloN
   * Calculates a power b modulates mod
   * @param a long long int
   * @param b long long int
   * @param mod long long int
   * @return long long int
   */
    long long int aPuisBModuloN(long long int a, long long int b, long long int mod);

    /**
   * @brief toString
   * Transforms a vector of long long int into a string
   * @param cryptedInt vector of long long int to transform
   * @return string composed of long long int separated by ;
   */
    string toString(vector<long long int>cryptedInt);

    /**
   * @brief toVector
   * Transforms a string into a vector of long long int
   * @param str string to transform
   * @return vector of long long int
   */
    vector<long long int> toVector(string str);

protected:
    /**
   * @brief publicKey
   * public key of user init null
   */

    Key publicKey;

    /**
   * @brief privateKey
   * private key of user init null
   */
    Key privateKey;
};

#endif // USERRSA_H
