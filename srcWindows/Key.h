#ifndef KEY_H
#define KEY_H

#include <string>
using namespace std;

/**
 * @brief The Key class
 * class representing a RSA key used by RSA users
 * can be public, private or notype
 */
class Key
{
public:
    // Constructors & destructors
    /**
   * @brief Key
   * Constructs objects from class Key
   * @param n, modulo
   * @param power, power
   * @param type, public, private, notype
   */
  Key(int n, int power, string type);

  /**
  * Destructor
  */
  ~Key();


  /**
   * @brief Password
   * checks if the key is private
   * @param b
   */
  /**
 *
 * @param b
 * @return
 */

  /**
   * @brief operator==
   * checks if the key is usable
   * @param b boolean to check with
   * @return boolean
   */
  bool operator == (bool b);

  //Getters and setters
  /**
   * @brief setN
   * sets the n
   * @param n, int to set
   */
  void setN(int n);

  /**
   * @brief setPower
   * set the power
   * @param power, int to set
   */
  void setPower(int power);

  /**
   * @brief setType
   * set the type
   * @param type, string to set
   */
  void setType(string type);

  /**
   * @brief getN
   * gets the n
   * @return the value of n
   */
  int getN() const;

  /**
   * @brief getPower
   * gets the power
   * @return the value of power
   */
  int getPower() const;

  /**
   * @brief getType
   * gets the type
   * @return the value of type
   */
  string getType() const;

private:
  /**
   * @brief n
   * modulo of the key
   */
  int n;

  /**
   * @brief power
   * power of the key
   */
  int power;

  /**
   * @brief type
   * type of the key
   */
  string type;
};

#endif // KEY_H
