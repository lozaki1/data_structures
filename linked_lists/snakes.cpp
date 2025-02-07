// UMBC - CMSC 341 - Fall 2024 - Proj1
#include "snakes.h"
Snakes::Snakes(){ 
    initializeEmptyObject();
}

Snakes::~Snakes(){
    clear();
}

void Snakes::clear(){
    // Check that memory exists
    if (m_start != nullptr){
        Cell * curr = m_start;        

        // Go through linked list and delete each cell
        while (curr != nullptr){
            m_start = curr;
            curr = curr->m_next;
            m_start->setNorth(nullptr);
            m_start->setSouth(nullptr);
            delete m_start;
        }

        // Set all member variables back to default values
        initializeEmptyObject();
    }
}

const Snakes & Snakes::operator=(const Snakes & rhs){
    // Check that object is not being assigned to itself or empty object
    if (this != &rhs && rhs.m_start != nullptr){
        // Deallocate existing memory
        clear();
        m_start = new Cell(rhs.m_start->getID());
        Cell * curr = m_start;         
        Cell * rhsCurr = rhs.m_start;  
        int listSize = 1;              

        // Allocate new memory and copy values from passed object
        while (rhsCurr->getNext() != nullptr){
            curr->setNext(new Cell(rhsCurr->getNext()->getID()));
            rhsCurr = rhsCurr->getNext();
            curr = curr->getNext();
            listSize++;
        }
        Cell * cellArr[listSize];
        Cell * temp = m_start;          
        for (int i = 0; i < listSize; i++){
            cellArr[i] = temp;
            temp = temp->m_next;
        }
        curr = m_start;
        rhsCurr = rhs.m_start;
        
        // Copy snakes and ladders data
        while (rhsCurr->getNext() != nullptr){
            if (rhsCurr->getNorth() != nullptr){
                curr->m_north = cellArr[rhsCurr->getNorth()->getID() - 1];
            }
            if (rhsCurr->getSouth() != nullptr){
                curr->m_south = cellArr[rhsCurr->getSouth()->getID() - 1];
            }
            curr = curr->m_next;
            rhsCurr = rhsCurr->getNext();
        }

        // Copy player locations and player turn
        m_player1 = cellArr[rhs.m_player1->getID() - 1];
        m_player2 = cellArr[rhs.m_player2->getID() - 1];
        m_playerTurn = rhs.m_playerTurn;
    }
    return *this;
}

int Snakes::rollDice(){
    // Return random number from 1 to 6
    return (rand() % 6) + 1;
}

Snakes::Snakes(int boardSize){
    initializeEmptyObject();
    boardSize = getValidBoardSize(boardSize);
    makeDefaultBoard(boardSize);
}

bool Snakes::makeDefaultBoard(int boardSize){
    // Check that board is empty
    if (m_start == nullptr){
        // Check that board size is valid num
        if (boardSize != 0 && boardSize >= 9 && floor(sqrt(boardSize)) == ceil(sqrt(boardSize))){
            Cell * cellArr[boardSize];
            makeEmptyBoard(boardSize, cellArr);

            int numSnakesLadders = (boardSize / 4) - 1;
            int numSnakes = numSnakesLadders / 2;
            int numLadders = numSnakesLadders - numSnakes;

            int currCell = 1;
            int ladderEnd = boardSize;

            // Offset board start and end by 2 (make sure it does not line up with snakes)
            // Add ladder start incrementing by 4 and ladder end decrementing by 4 
            for (int i = 0; i < numLadders; i++){
                if (i == 0){
                    currCell += 2;
                    ladderEnd -= 2;
                    cellArr[currCell - 1]->m_north = cellArr[ladderEnd - 1];
                }
                else{
                    currCell += 4;
                    ladderEnd -= 4;
                    cellArr[currCell - 1]->m_north = cellArr[ladderEnd - 1];
                }
            }
            currCell = boardSize;
            int snakeEnd = 1;

            // Starting at last cell, add snake start decrementing by 4 and starting at first cell, 
            // add snake end incrementing by 4
            for (int j = 0; j < numSnakes; j++){
                currCell -= 4;
                snakeEnd += 4;
                cellArr[currCell - 1]->m_south = cellArr[snakeEnd - 1];
            }

            // Place both players on start cell and set turn to player 1
            m_player1 = m_start;
            m_player2 = m_start;
            m_playerTurn = PLAYER1TURN;

            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}

void Snakes::makeRandomBoard(int boardSize, int numSnakesLadders){
    // Deallocate current board
    clear();
    boardSize = getValidBoardSize(boardSize);
    int const MINSL = 2;
    int const MAXSL = (boardSize / 2) - 2;

    // Check that board size is greater than 0 and board is empty
    if (boardSize != 0 && m_start == nullptr){
        // If num of snakes and ladder is less than min, set it to min
        if (numSnakesLadders < MINSL){
            numSnakesLadders = MINSL;
        } 
        // Else if num of snakes and ladders is greater than max, set it to max
        else if (numSnakesLadders > MAXSL){
            numSnakesLadders = MAXSL;
        }
        Cell * cellArr[boardSize];
        // Make board of specified size
        makeEmptyBoard(boardSize, cellArr);
    
        int numSnakes = numSnakesLadders / 2;
        int numLadders = numSnakesLadders - numSnakes;

        int occupiedCellsArr[numSnakesLadders * 2] = {0};

        // Add snakes and ladders to board
        addSnakesLadders(boardSize, "ladders", numLadders, occupiedCellsArr, numSnakesLadders, cellArr);
        addSnakesLadders(boardSize, "snakes", numSnakes, occupiedCellsArr, numSnakesLadders, cellArr);
        
        // Both players start on cell 1 and player 1 goes first
        m_player1 = m_start;
        m_player2 = m_start;
        m_playerTurn = PLAYER1TURN;
    }
}

bool Snakes::play(int dice){
    const int MINROLL = 1;
    const int MAXROLL = 6;

    bool notLastCell = true;

    // Check that roll is between 1-6
    if (dice >= MINROLL && dice <= MAXROLL){
        Cell * temp = m_start;
        int boardSize = 1;
        bool moveForward = true;
        while (temp->m_next != nullptr){
            boardSize++;
            temp = temp->m_next;
        }

        if (m_playerTurn == 1){
            temp = m_player1;
        }
        else if (m_playerTurn == 2){
            temp = m_player2;
        }

        if (boardSize - temp->m_cellID < dice){
            moveForward = false;
        }

        // If dice roll exceeds required dice value to move to last cell, player does not move
        if (moveForward){
            for (int i = 0; i < dice; i++){
                temp = temp->m_next;
            }
            // If player lands on cell with snake or ladder, they move to top of ladder or end of snake
            if (temp->m_north != nullptr){
                temp = temp->m_north;
            }
            else if (temp->m_south != nullptr){
                temp = temp->m_south;
            }
        }
        // Player turn switches to other player
        if (m_playerTurn == 1){
            m_player1 = temp;
            m_playerTurn = PLAYER2TURN;
        }
        else if (m_playerTurn == 2){
            m_player2 = temp;
            m_playerTurn = PLAYER1TURN;
        }

        // If player lands on last cell (wins), return false
        if (temp->m_next == nullptr){
            notLastCell = false;
        }
    }
    return notLastCell;
}

void Snakes::reStart(){
    // Players go back to first cell and player 1 starts
    m_player1 = m_start;
    m_player2 = m_start;
    m_playerTurn = 1;
}

void Snakes::dumpBoard(){
    if (m_start != nullptr){
        int ladderCode, snakeCode;
        Cell *temp = m_start;
        cout << "START => ";
        while(temp != nullptr){
            cout << temp->m_cellID;

            if (temp == m_player1) cout << " (Player 1)";
            if (temp == m_player2) cout << " (Player 2)";
            if (temp->m_north != nullptr) ladderCode = temp->m_north->m_cellID;
            else ladderCode = -1;
            if (temp->m_south != nullptr) snakeCode = temp->m_south->m_cellID;
            else snakeCode = -1;
            if (ladderCode != -1 || snakeCode != -1){
                cout << " (";
                if (ladderCode != -1) cout << "\033[1;32mLadder to: " << ladderCode << "\033[0m";// green text
                if (snakeCode != -1) cout << "\033[1;31mSnake to: " << snakeCode << "\033[0m";//red text
                cout << ")";
            }
            cout << " => ";
            temp = temp->m_next;
        }
        cout << "END" << endl;
    }
}

int Snakes::getValidBoardSize(int boardSize){
    // If board size is less than 9, return 0 (empty board)
    if (boardSize < 9){
        return 0;
    }
    else{
        // If board size is not perfect square, return highest perfect square less than size
        if (floor(sqrt(boardSize)) != ceil(sqrt(boardSize))){
            return pow(floor(sqrt(boardSize)), 2);
        }
        // Otherwise, return original size
        else{
            return boardSize;
        }
    }
}

void Snakes::makeEmptyBoard(int boardSize, Cell * cellArr[]){
    Cell * newCell = new Cell(1);
    m_start = newCell;
    cellArr[0] = m_start;

    Cell * temp = m_start;

    // Create board with number of cells specified
    for (int i = 2; i < boardSize + 1; i++){
        newCell = new Cell(i);
        temp->m_next = newCell;
        temp = temp->m_next;
        cellArr[i - 1] = newCell;
    }
}

void Snakes::addSnakesLadders(int boardSize, string type, int numType, int occupiedCellsArr[], int numSL, Cell * cellArr[]){
    int startMin = 0, startMax = 0, endMin = 0, endMax = 0, emptyCells = 0, x = 0, numLoops = 0;
    int count = 0;
    bool possibleEndCell = false;
    bool cellOccupied = true;

    if (type == "ladders"){
        startMin = 2;
        startMax = boardSize - 2;
    }
    else if (type == "snakes"){
        startMin = 2;
        startMax = boardSize - 1;
        count = numSL - (numType * 2);
    }
    
    Random randStartCell(startMin, startMax);
    randStartCell.setSeed(time(NULL));
    int startCell = 1;

    // Randomly place specified number of either snakes or ladders to board
    for (int i = 0; i < numType; i++){
        possibleEndCell = false;
        cellOccupied = true;

        // Get random start cell
        while (possibleEndCell == false || cellOccupied == true ||
                cellArr[startCell - 1]->m_north != nullptr ||
                cellArr[startCell - 1]->m_south != nullptr){
            startCell = randStartCell.getRandNum();
            if (type == "ladders"){
                emptyCells = boardSize - startCell - 1;
                x = startCell + 1;
                numLoops = boardSize;
            }
            else if (type == "snakes"){
                emptyCells = startCell - 2;
                x = 2;
                numLoops = startCell;
            }

            // Check that there exists a possible end cell that does not have already have snake or ladder
            for ( ; x < numLoops; x++){
                for (int j = 0; j < numSL * 2; j++){
                    if (occupiedCellsArr[j] == x){
                        emptyCells--;
                    }
                }
            }
            possibleEndCell = false;
            if (emptyCells != 0){
                possibleEndCell = true;
            }

            // Check that potential start cell does not already have snake or ladder
            cellOccupied = false;
            for (int y = 0; y < numSL * 2; y++){
                if (occupiedCellsArr[y] == startCell){
                    cellOccupied = true;
                }
            }
        }
        occupiedCellsArr[count] = startCell;
        count++;

        if (type == "ladders"){
            endMin = startCell + 1;
            endMax = boardSize - 1;
        }
        else if (type == "snakes"){
            endMin = 1;
            endMax = startCell - 1;
        }

        Random randEndCell(endMin, endMax);
        randEndCell.setSeed(time(NULL));
        int endCell = 1;
        cellOccupied = true;

        // Get random end cell
        while (cellOccupied == true ||
                cellArr[endCell - 1]->m_north != nullptr ||
                cellArr[endCell - 1]->m_south != nullptr){
            endCell = randEndCell.getRandNum();
            cellOccupied = false;

            // Check that potential end cell does not already have snake or ladder
            for (int z = 0; z < numSL * 2; z++){
                if (occupiedCellsArr[z] == endCell){
                    cellOccupied = true;
                }
            }
        }

        // Add either snake or ladder to board
        if (type == "ladders")
            cellArr[startCell - 1]->m_north = cellArr[endCell - 1];
        else if (type == "snakes")
            cellArr[startCell - 1]->m_south = cellArr[endCell - 1];
        
        occupiedCellsArr[count] = endCell;
        count++;
    }
}

void Snakes::initializeEmptyObject(){
    m_start = nullptr;
    m_player1 = nullptr;
    m_player2 = nullptr;
    m_playerTurn = 0;
}

bool Snakes::isEmptyObject(){
    // Check if object is empty
    if (m_start == nullptr && m_player1 == nullptr && m_player2 == nullptr && m_playerTurn == 0){
        return true;
    }
    else{
        return false;
    }
}