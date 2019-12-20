#ifndef PUBLICKEYTABLE_H
#define PUBLICKEYTABLE_H

#include <string>
#include <map>
#include "Key.h"
using namespace std;

/**
 * @brief The PublicKeyTable class
 * unique table containing all the public keys registered
 */
class PublicKeyTable{

private:
    /**
     * @brief PublicKeyTable
     * constructor
     */
    PublicKeyTable();

public:
    /**
      * Destructor
      */
    ~PublicKeyTable();

    /**
     * @brief addKey
     *  Adds a key in the table using the given name (if not already in table)
     * @param name, name of the user
     * @param publicKey, public key of the user
     */
    void addKey(string name, Key publicKey);

    /**
     * @brief getKey
     * Gets a key from the table using a given name
     * @param name, name of the user to search
     * @return key from a given name
     */
    Key getKey(string name);

    /**
     * @brief removeKey
     * Removes a key from the table using a given name
     * @param name, name of the user to remove
     */
    void removeKey(string name);

    /**
     * @brief getInstance
     * Gets the instance of the singleton
     * @return a singleton initializated
     */
    static PublicKeyTable *getInstance();

    /**
     * @brief kill
     * kill the singleton
     */
    static void kill ();


private:
    /**
     * @brief readDataFile
     * read date from a file and save the value in the map
     */
    void readDataFile();

    /**
     * @brief table
     * table to save value
     */
    map<string,Key>* table;

    /**
     * @brief singleton
     */
    static PublicKeyTable *singleton;
};

#endif // PUBLICKEYTABLE_H
