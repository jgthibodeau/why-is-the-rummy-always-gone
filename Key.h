/*
 * Key.h
 *
 *  Created on: Oct 11, 2014
 *      Author: ece373
 */

#ifndef KEY_H_
#define KEY_H_
#include <iostream>
using namespace std;

class Key {
private:
	char keyVal;
	string textVal;
public:
	Key(){keyVal=' ';textVal="";};
	Key(char k, string text);

	// Extractors.
	char key() { return keyVal; }
	string text() { return textVal; }
	string toString() { return textVal+": "+keyVal; }

	// Setters
	void setKey(char c){ keyVal = c; }
	void setText(string s){ textVal = s; }
};

#endif /* KEY_H_ */
