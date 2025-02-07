// UMBC - CMSC 341 - Fall 2024 - Proj0
#ifndef SOLITAIRE_H
#define SOLITAIRE_H
#include <iostream>
#include <string>
using namespace std;
class Tester;
// ENGLISH board shape has 33 holes
// DIAMOND board tepe has 41 holes
// We do not support FRENCH board
enum BOARDSHAPE {NONE, ENGLISH, FRENCH, DIAMOND};
const int ENGLISHSIZE = 7; // used to create a 7x7 matrix
const int DIAMONDSIZE = 9; // used to create a 9x9 matrix
const int NUMENGLISHMARBLES = 32; // total number of marbles used on an english board
const int NUMDIAMONDMARBLES = 40; // total number of marbles used on a diamond board
const int OUT = 0;
const int HOLE = 1;
const int MARBLE = 2;
#define REDSTAR "\033[1;31m\u2731\033[0m " // represents a marble (red bold astrisk)
#define BLACKSTAR "\u23E3 " // represents a hole (BENZENE RING WITH CIRCLE)

class Solitaire{
public:
  friend class Tester;
  //Name: Solitaire - Default Constructor
  //Precondition: None
  //Postcondition: Creates an empty Solitaire object with default values
  Solitaire();
  //Name: Solitaire - Constructor
  //Precondition: Requires a valid board shape (diamond, english, french or none)
  //Postcondition: Creates a new Solitaire object set to values of passed board and allocates memory
  Solitaire(BOARDSHAPE board);
  //Name: Solitaire - Copy Constructor
  //Precondition: Solitaire object must already exist
  //Postcondition: Creates a deep copy of existing Solitaire in separate memory address
  Solitaire(const Solitaire & rhs);
  //Name: Solitaire - Destructor
  //Precondition: None
  //Postcondition: Deallocates memory from m_board and sets member variables to 0
  ~Solitaire();
  //Name: Solitaire - clear()
  //Precondition: None
  //Postcondition: Deallocates memory and reinitializes all members
  void clear();
  //Name: Solitaire - newBoard()
  //Precondition: None
  //Postcondition: Reinitializes current object and returns true, if object is empty returns false
  bool newBoard();
  //Name: Solitaire - changeBoard()
  //Precondition: Requires a valid board shape
  //Postcondition: Reallocates memory if shape is different, otherwise only reinitializes object
  void changeBoard(BOARDSHAPE board);
  //Name: Solitaire - play()
  //Precondition: Requires two integer pairs
  //Postcondition: Makes a valid move and returns true, otherwise returns false
  bool play(pair<int,int> origin, pair<int,int> destination);
  //Name: Solitaire - dumpBoard()
  //Precondition: None
  //Postcondition: Prints the current board
  void dumpBoard();       // for debugging purposes
  //Name: Solitaire - reportNumMarbles()
  //Precondition: None
  //Postcondition: Prints the number of marbles left on the board
  int reportNumMarbles(); // for debugging purposes
private:
  int m_numRows;
  int m_numColumns;
  int ** m_board;     // the 2d structure to store game 
  BOARDSHAPE m_shape; // the board shape (either english or diamond)
  int m_numMarbles;   // current number of marbles
    
  /*********************************
   * Private function declarations *
   ********************************/
  
  //Name: Solitaire - initializeDiamond()
  //Precondition: Members set to values for diamond board
  //Postcondition: Initializes m_board to starting diamond board
  void initializeDiamond();
   //Name: Solitaire - initializeEnglish()
  //Precondition: Members set to values for english board
  //Postcondition: Initializes m_board to starting english board
  void initializeEnglish();
   //Name: Solitaire - setVariables()
  //Precondition: Requires a valid board shape
  //Postcondition: Initializes object to passed board and allocates memory
  void setVariables(BOARDSHAPE board);
};
#endif
