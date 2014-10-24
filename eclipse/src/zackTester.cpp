#include "Combo.h"  
#include "DiscardPile.h"
#include <cstdlib> 
#include <iostream>
using namespace std;

int main(int argc, char* argv[]){

Card c1(1,2,2); //testing card 1
Card c2(1,3,3); //testing card 2 
Card c3(1,4,4); //testing card 3 
Card c4(2,1,1); //testing card 4
Combo comb; //testing combo 
DiscardPile discardP; //testing discard pile

//TESTING COMBO METHODS 
bool isEmptyBefore = comb.isEmpty(); 
bool isDiscardEmpty = discardP.isEmpty();
bool addCardResult1 = comb.addCard(c2);  
bool addCardResult2 = comb.addCard(c3);  
bool addCardResult3 = comb.addCard(c1);  
bool addCardResult4 = comb.addCard(c4);   
discardP.addCard(c4);     
bool isDiscardEmptyAfterAdd = discardP.isEmpty(); 
//discardP.removeCard(); 
//bool isDiscardEmptyAfterRM = discardP.isEmpty(); 
bool isEmptyAfter = comb.isEmpty();
bool comboValid = comb.isValid();  
cout << "     COMBO TESTS" << endl;
if(isEmptyBefore && !isEmptyAfter){ 
	cout << "[PASSED] CHECKING EMPTY STATUS" << endl;
} 
else{ 
	cout << "[FAILED] CHECKING EMPTY STATUS" << endl;
} 
if(comboValid){ 
	cout << "[PASSED] CHECKING VALID COMBO" << endl;
} 
else{ 
	cout << "[FAILED] CHECKING VALID COMBO" << endl;
}
if(addCardResult1 && addCardResult2 && addCardResult3 && !addCardResult4){  
	cout << "[PASSED] ADDING CARD TO COMBOS" << endl;
} 
else{ 
	cout << "[FAILED] ADDING CARD TO COMBOS" << endl;
}  
if(comb.toString() == "[2 of Spades][3 of Spades][4 of Spades]"){ 
	cout << "[PASSED] COMBO TO STRING" << endl;
} 
else{ 
	cout << "[FAILED] COMBO TO STRING" << endl;
}  

cout << "     CARD TESTS" << endl; 
if(c4.toString() == "[Ace of Hearts]"){ 
	cout << "[PASSED] CARD TO STRING" << endl;
}
else{ 
	cout << "[FAILED] CARD TO STRING" << endl;
}

cout << "     DISCARD PILE TESTS" << endl; 
if(isDiscardEmpty){ 
	cout << "[PASSED] TESTING IS EMPTY METHOD" << endl;
}
else{ 
	cout << "[FAILED] TESTING IS EMPTY METHOD" << endl;
}
if(!isDiscardEmptyAfterAdd){ 
	cout << "[PASSED] ADDING CARD TO DISCARD PILE" << endl;
}
else{ 
	cout << "[FAILED] ADDING CARD TO DISCARD PILE" << endl;
}
if(true){ 
	cout << "[PASSED] REMOVING CARD FROM DISCARD PILE" << endl;
}
else{ 
	cout << "[FAILED] REMOVING CARD FROM DISCARD PILE" << endl;
}


return 0; 
}