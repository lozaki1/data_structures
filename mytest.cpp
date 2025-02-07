// UMBC - CMSC 341 - Fall 2024 - Proj0
#include "solitaire.h"
#include <iostream>
using namespace std;

class Tester{
public:
  //Name: Tester - testConstructorNormal()
  //Precondition: None
  //Postcondition: Returns true if memory is created and members set to proper values 
  bool testConstructorNormal();
  //Name: Tester - testConstructorError()
  //Precondition: None
  //Postcondition: Returns true if non-supported board shapes are set to empty objects
  bool testConstructorError();
  //Name: Tester - testNewBoardNormal()
  //Precondition: None
  //Postcondition: Returns true if member variables reinitialized without reallocating memory
  bool testNewBoardNormal();
  //Name: Tester - testNewBoardError()
  //Precondition: None
  //Postcondition: Returns true if passing an empty object returns false and is not modified
  bool testNewBoardError();
  //Name: Tester - testChangeBoardSameShape()
  //Precondition: None
  //Postcondition: Returns true if same shaped board only reinitializes members
  bool testChangeBoardSameShape();
  //Name: Tester - testChangeBoardDiffShape()
  //Precondition: None
  //Postcondition: Returns true if memory is reallocated and member reintialized
  bool testChangeBoardDiffShape();
  //Name: Tester - testPlayNormal()
  //Precondition: None
  //Postcondition: Returns true if correct moves work (2 spaces horizontally or vertically)
  bool testPlayNormal();
  //Name: Tester - testPlayError()
  //Precondition: None
  //Postcondition: Returns true is wrong moves do not work (diagonal, more or less than 2 spaces, out of range, etc.)
  bool testPlayError();
  //Name: Tester - testCopyConstructorNormal()
  //Precondition: None
  //Postcondition: Returns true if copied board has same values with different memory addresses
  bool testCopyConstructorNormal();
  //Name: Tester - testCopyConstructorError()
  //Precondition: None
  //Postcondition: Returns true is empty object values are copied
  bool testCopyConstructorError();
private:
  //Name: Tester - isEmptyObject(Solitaire&)
  //Precondition: Requires a Solitaire object
  //Postcondition: Returns true if passed object is empty
  bool isEmptyObject(const Solitaire & board);
};

const char * FAIL_STATEMENT = "*****TEST FAILED: ";
const char * PASS_STATEMENT = "     TEST PASSED: ";

int main(){
  Tester test;
  if (test.testConstructorNormal()){
    cout << PASS_STATEMENT << "Constructor allocated memory and intialized member variable to proper values." << endl;
  } else{
    cout << FAIL_STATEMENT << "Constructor failed to allocate memory or intialize member variables." << endl;
  }

  if (test.testConstructorError()){
    cout << PASS_STATEMENT << "Constructor set non-supported board shapes to default values." << endl;
  } else{
    cout << FAIL_STATEMENT << "Constructor failed to set non-supported shapes to default values." << endl;
  }

  if (test.testNewBoardNormal()){
    cout << PASS_STATEMENT << "newBoard() reintialized all member variables without reallocating memory." << endl;
  } else{
    cout << FAIL_STATEMENT << "newBoard() failed to reintialize member variable or reallocated memory." << endl;
  }

  if (test.testNewBoardError()){
    cout << PASS_STATEMENT << "newBoard() did not modify empty object and returned false." << endl;
  } else{
    cout << FAIL_STATEMENT << "newBoard() modified empty object or returned true." << endl;
  }
  
  if (test.testChangeBoardSameShape()){
    cout << PASS_STATEMENT << "changeBoard() reinitialized object of same board shape." << endl;
  } else{
    cout << FAIL_STATEMENT << "newBoard() did not reinitialize object of same board shape." << endl;
  }

  if (test.testChangeBoardDiffShape()){
    cout << PASS_STATEMENT << "newBoard() created new object of different board shape." << endl;
  } else{
    cout << FAIL_STATEMENT << "newBoard() did not create new object of different board shape." << endl;
  }

  if (test.testPlayNormal()){
    cout << PASS_STATEMENT << "play() worked for correct moves." << endl;
  } else{
    cout << FAIL_STATEMENT << "play() did not work for correct moves." << endl;
  }

  if (test.testPlayError()){
    cout << PASS_STATEMENT << "play() did not work for wrong moves." << endl;
  } else{
    cout << FAIL_STATEMENT << "play() worked for wrong moves." << endl;
  }

  if (test.testCopyConstructorNormal()){
    cout << PASS_STATEMENT << "Copy constructor created a deep copy of passed object." << endl;
  } else{
    cout << FAIL_STATEMENT << "Copy constructor failed to create a deep copy of passed object." << endl;
  }

  if (test.testCopyConstructorError()){
    cout << PASS_STATEMENT << "Copy constructor copied an empty object." << endl;
  } else{
    cout << FAIL_STATEMENT << "Copy constructor did not copy an empty object." << endl;
  }
  return 0;
}


bool Tester::testConstructorNormal(){
  Solitaire diamondBoard(DIAMOND);
  Solitaire englishBoard(ENGLISH);

  // check that dimentions, shape and number of marbles match diamond board
  if (diamondBoard.m_numRows == DIAMONDSIZE &&
      diamondBoard.m_numColumns == DIAMONDSIZE &&
      diamondBoard.m_shape == DIAMOND &&
      diamondBoard.m_numMarbles == NUMDIAMONDMARBLES &&
      diamondBoard.m_board != nullptr){
    for (int i = 0; i < DIAMONDSIZE; i++){
      if (diamondBoard.m_board[i] == nullptr)
        return false;
    }
  } else{
    return false;
  }

  // check that member values are correct for english board
  if (englishBoard.m_numRows == ENGLISHSIZE &&
      englishBoard.m_numColumns == ENGLISHSIZE &&
      englishBoard.m_shape == ENGLISH &&
      englishBoard.m_numMarbles == NUMENGLISHMARBLES &&
      englishBoard.m_board != nullptr){
    for (int i = 0; i < ENGLISHSIZE; i++){
      if (englishBoard.m_board[i] == nullptr)
        return false;
    }
  } else{
    return false;
  }
  return true;
}


bool Tester::testConstructorError(){
  Solitaire nonSupportedShape1(FRENCH);
  Solitaire nonSupportedShape2(NONE);

  // check that objects are empty
  if (!isEmptyObject(nonSupportedShape1) || !isEmptyObject(nonSupportedShape2))
    return false;
  
  return true;
}


bool Tester::isEmptyObject(const Solitaire & board){
  if (board.m_numRows == 0 &&
      board.m_numColumns == 0 &&
      board.m_numMarbles == 0 &&
      board.m_board == nullptr){
    return true;
  } else{
    return false;
  }
}


bool Tester::testNewBoardNormal(){
  Solitaire diamondBoard(DIAMOND);

  // make a move from (6,4) to (4, 4)
  diamondBoard.play(pair<int,int>(6,4), pair<int,int>(4,4)); 

  int * ptrArray[diamondBoard.m_numRows];

  // copy current memory addresses of m_board to ptrArray
  for (int i = 0; i < diamondBoard.m_numRows; i++){
    ptrArray[i] = diamondBoard.m_board[i];
  }

  // newBoard() should return true since diamondBoard is not an empty object
  if (!diamondBoard.newBoard()){
    return false;
  }

  // check that board reset to beginning (previous move was undone) 
  if (diamondBoard.m_board[4][4] == HOLE && diamondBoard.m_board[5][4] == MARBLE &&
      diamondBoard.m_board[6][4] == MARBLE){
  } else{
    return false;
  }

  for (int i = 0; i < diamondBoard.m_numRows; i++){
    if (diamondBoard.m_board[i] != ptrArray[i]){ // check that memory addresses are same (memory was not reallocated)
      return false;
    }
  }
  return true;
}


bool Tester::testNewBoardError(){
  Solitaire emptyBoard;

  // check that object is empty
  if (isEmptyObject(emptyBoard)){
    // newBoard() should return false for empty object
    if (!emptyBoard.newBoard()){
      // object should still be empty
      if (isEmptyObject(emptyBoard)){
	return true;
      } else{
	return false;
      }
    } else{
      return false;
    }
  }else{
    return false;
  }
}


bool Tester::testChangeBoardSameShape(){
  // if board shape is same, changeBoard() calls newBoard() to reinitialize object
  if (testNewBoardNormal())
    return true;
  else
    return false;
}


bool Tester::testChangeBoardDiffShape(){
  Solitaire board1(DIAMOND);

  // change from diamond board to english board
  board1.changeBoard(ENGLISH);

  // check that members updated to english values
  if (board1.m_numRows == ENGLISHSIZE &&
      board1.m_numColumns == ENGLISHSIZE &&
      board1.m_shape == ENGLISH &&
      board1.m_numMarbles == NUMENGLISHMARBLES &&
      board1.m_board != nullptr){
    for (int i = 0; i < ENGLISHSIZE; i++){
      if (board1.m_board[i] == nullptr)
        return false;
    }
  } else{
    return false;
  }

  // change english board to french board (not supported)
  board1.changeBoard(FRENCH);

  // object should now be empty
  if (!isEmptyObject(board1))
    return false;

  // change empty board to diamond board
  Solitaire board2;
  board2.changeBoard(DIAMOND);

  // check that members updated to diamond values
  if (board2.m_numRows == DIAMONDSIZE &&
      board2.m_numColumns == DIAMONDSIZE &&
      board2.m_shape == DIAMOND &&
      board2.m_numMarbles == NUMDIAMONDMARBLES &&
      board2.m_board != nullptr){
    for (int i = 0; i < DIAMONDSIZE; i++){
      if (board2.m_board[i] == nullptr)
        return false;
    }
  } else{
    return false;
  }
  
  return true;
}


bool Tester::testPlayNormal(){
  Solitaire diamondBoard(DIAMOND);
  Solitaire englishBoard(ENGLISH);

  // check that valid horizontal moves work
  if (!diamondBoard.play(pair<int,int>(6,4), pair<int,int>(4,4))){
    return false;
  }
  if (!diamondBoard.play(pair<int,int>(6,6), pair<int,int>(6,4))){
    return false;
  }

  // check that valid vertical moves work
  if (!englishBoard.play(pair<int,int>(3,1), pair<int,int>(3,3))){
    return false;
  }
  if (!englishBoard.play(pair<int,int>(1,2), pair<int,int>(3,2))){
    return false;
  }
  
  return true;
}


bool Tester::testPlayError(){
  Solitaire diamondBoard(DIAMOND);
  
  // check out of bounds moves
  if (diamondBoard.play(pair<int,int>(4,9), pair<int,int>(4,7))){
    return false;
  }
  if (diamondBoard.play(pair<int,int>(-1,4), pair<int,int>(1,4))){
    return false;
  }
  if (diamondBoard.play(pair<int,int>(9,4), pair<int,int>(7,4))){
    return false;
  }
  if (diamondBoard.play(pair<int,int>(4,-1), pair<int,int>(4,1))){
    return false;
  }

  // check moves that are greater than or less than 2 spaces away
  if (diamondBoard.play(pair<int,int>(4,1), pair<int,int>(4,4))){
    return false;
  }
  if (diamondBoard.play(pair<int,int>(1,4), pair<int,int>(4,4))){
    return false;
  }
  if (diamondBoard.play(pair<int,int>(3,4), pair<int,int>(4,4))){
    return false;
  }

  Solitaire emptyBoard;
  // check moves made on empty board
  if (emptyBoard.play(pair<int,int>(3,1), pair<int,int>(3,3))){
    return false;
  }

  // check diagonal moves
  if (diamondBoard.play(pair<int,int>(2,2), pair<int,int>(4,4))){
    return false;
  }
  
  // check moves where destination is not hole
  if (diamondBoard.play(pair<int,int>(2,3), pair<int,int>(2,5))){
    return false;
  }
  if (diamondBoard.play(pair<int,int>(4,1), pair<int,int>(2,1))){
    return false;
  }

  diamondBoard.play(pair<int,int>(4,2), pair<int,int>(4,4));
  diamondBoard.play(pair<int,int>(4,0), pair<int,int>(4,2));

  // check moves where origin is not marble
  if (diamondBoard.play(pair<int,int>(4,1), pair<int,int>(4,3))){
    return false;
  }
  if (diamondBoard.play(pair<int,int>(1,6), pair<int,int>(1,4))){
    return false;
  }

  // check moves where space between origin and destination is not marble
  if (diamondBoard.play(pair<int,int>(4,2), pair<int,int>(4,0))){
    return false;
  }

  return true;
}


bool Tester::testCopyConstructorNormal(){
  Solitaire solitaire(DIAMOND);
  Solitaire copy(solitaire);
  if (solitaire.m_numColumns == copy.m_numColumns &&
      solitaire.m_numRows == copy.m_numRows &&
      solitaire.m_shape == copy.m_shape &&
      solitaire.m_numMarbles == copy.m_numMarbles &&
      solitaire.m_board != copy.m_board){
    for (int i=0;i<solitaire.m_numRows;i++){
      for (int j=0;j<solitaire.m_numColumns;j++)
	{
	  if ((solitaire.m_board[i][j] != copy.m_board[i][j]) ||   //check the value
	      (solitaire.m_board[i] == copy.m_board[i])){          //check the ith pointer
	    return false;
	  }
	}
    }
    return true;// everything is correct
  }
  else return false;//everthing else
}


bool Tester::testCopyConstructorError(){
  // create and copy empty Solitaire object
  Solitaire empty;
  Solitaire copy(empty);

  // check that copy is also empty object
  if (isEmptyObject(copy)){
    return true;
  } else{
    return false;
  }
}
