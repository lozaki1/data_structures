// UMBC - CMSC 341 - Fall 2024 - Proj0
#include "solitaire.h"

Solitaire::Solitaire(){
  // set members to default values
  m_numRows = 0;
  m_numColumns = 0;
  m_board = nullptr;
  m_shape = NONE;
  m_numMarbles = 0;
}


Solitaire::Solitaire(BOARDSHAPE board){
  // initialize members and board to correct shape, or default if none
  if (board == DIAMOND){
    setVariables(DIAMOND);
    initializeDiamond();
  } else if (board == ENGLISH){
    setVariables(ENGLISH);
    initializeEnglish();
  } else if (board == FRENCH){
    setVariables(FRENCH);
  } else{
    setVariables(NONE);
  }
}


Solitaire::Solitaire(const Solitaire & rhs){
  m_numRows = rhs.m_numRows;
  m_numColumns = rhs.m_numColumns;
  m_shape = rhs.m_shape;
  m_numMarbles = rhs.m_numMarbles;

  // check that passed object is not empty
  if (rhs.m_board != nullptr){
    // allocate memory and copy values
    m_board = new int*[m_numRows];

    for (int i = 0; i < m_numRows; i++){
      m_board[i] = new int[m_numColumns];
      for (int j = 0; j < m_numColumns; j++){
	m_board[i][j] = rhs.m_board[i][j];
      }
    }
  } else{
    m_board = nullptr;
  }
}


Solitaire::~Solitaire(){
  // if object is already empty, do nothing, otherwise deallocate memory
  if (m_board != nullptr){
    clear();
  }
}


void Solitaire::clear(){
  if (m_board != nullptr){
    for (int i = 0; i < m_numRows; i++){
      if (m_board[i] != nullptr){
	delete [] m_board[i];
	m_board[i] = nullptr;
      }
    }
    delete [] m_board;
  }
  setVariables(NONE);
}


bool Solitaire::play(pair<int,int> origin, pair<int,int> destination){
  int x1 = origin.first;          // row number of origin
  int y1 = origin.second;         // column number of origin
  int x2 = destination.first;     // row number of destination
  int y2 = destination.second;    // column number of destination

  int arr[4] = {x1, x2, y1, y2};
  
  int yDiff = y1 - y2;
  int xDiff = x1 - x2;

  // check that object is not empty
  if (m_board != nullptr){
    // check that coordinates are not out of bounds
    for (int i = 0; i < 4; i++){
      if (arr[i] < 0 || arr[i] >= m_numRows){
	return false;
      }
    }
    // check that origin is a marble and destination is a hole
    if (m_board[x1][y1] == MARBLE && m_board[x2][y2] == HOLE){
      // check that distance between spaces is either 2 up, 2 down, 2 left or 2 right
      if ((x1 == x2 && (yDiff  == 2 || yDiff  == -2)) ||
	(y1 == y2 && (xDiff == 2 || xDiff == -2))){
	// check that middle space contains a marble
	if (m_board[(x1 + x2) / 2][(y1 + y2) / 2] == MARBLE){
	  // set origin and middle to holes and destination to marble
	  m_board[(x1 + x2) / 2][(y1 + y2) / 2] = HOLE;
	  m_board[x1][y1] = HOLE;
	  m_board[x2][y2] = MARBLE;
	  return true;
	} else{
	  return false;
	}
      } else{
	return false;
      }
    } else{
      return false;
    }
  } else{
    return false;
  }
}


void Solitaire::changeBoard(BOARDSHAPE board){
  // if asking shape is different from current shape, create new object
  if (board != m_shape){
    clear();
    setVariables(board);
  }
  // reinitialize members
  newBoard();
}


bool Solitaire::newBoard(){
  // check if empty object
  if (m_board != nullptr){
    // initialize board to appropriate shape
    if (m_shape == DIAMOND){
      initializeDiamond();
    } else if (m_shape == ENGLISH){
      initializeEnglish();
    }
    return true;
  } else{
    return false;
  }
}


void Solitaire::dumpBoard(){
    // this is to facilitate debugging 
    cout << "  ";
    for (int k=0;k<m_numColumns;k++){
        cout << k << " ";
    }
    cout << endl;
    for (int i=1;i <= m_numRows;i++){
        cout << i-1 << " ";
        for (int j=1;j<=m_numColumns;j++){
            if (m_board[i-1][j-1] == OUT)
                cout << "  ";
            else if (m_board[i-1][j-1] == HOLE)
                cout << BLACKSTAR;
            else
                cout << REDSTAR;
        }
        cout << endl;
    }
    cout << endl;
}


int Solitaire::reportNumMarbles(){
    // this is to facilitate debugging 
    cout << "You have " << m_numMarbles << " marbles to remove!" << endl;
    return m_numMarbles;
}


void Solitaire::initializeDiamond(){
  int mid = m_numRows / 2;    
  int start = mid;
  int end = start + 1;

  // add marbles in diamond shape
  for (int i = 0; i < m_numRows; i++){
    for (int k = start; k < end; k++){
      m_board[i][k] = MARBLE;
    }
    
    if (i < mid){
      start--;
      end++;
    } else{
      start++;
      end--;
    }
  }
  // leave middle space empty
  m_board[mid][mid] = HOLE;
}


void Solitaire::initializeEnglish(){
  int mid = m_numRows / 2;
  int start = mid - 1;
  int end = m_numRows - start;

  // add marbles in english shape
  for (int i = 0; i < m_numRows; i++){
    for (int j = start; j < end; j++){
      m_board[i][j] = MARBLE;
      m_board[j][i] = MARBLE;
    }
  }
  // leave middle space empty
  m_board[mid][mid] = HOLE;
}


void Solitaire::setVariables(BOARDSHAPE board){
  int size = 0;        // dimension of matrix
  m_numRows = 0;
  m_numColumns = 0;
  m_board = nullptr;
  m_shape = board;
  m_numMarbles = 0;
  
  if (board == DIAMOND || board == ENGLISH){
    if (board == DIAMOND){
      size = DIAMONDSIZE;
      m_numMarbles = NUMDIAMONDMARBLES;
    } else{
      size = ENGLISHSIZE;
      m_numMarbles = NUMENGLISHMARBLES;
    }

    m_numRows = size;
    m_numColumns = size;
    m_shape = board;

    // allocate memory
    m_board = new int*[size];
    for (int x = 0; x < size; x++){
      m_board[x] = new int[size];
    }

    // initialize all board squares to OUT
    for (int i = 0; i < size; i++){
      for (int j = 0; j < size; j++){
	m_board[i][j] = OUT;
      }
    }
  }
}
