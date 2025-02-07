// UMBC - CMSC 341 - Fall 2024 - Proj1
#include "snakes.h"
#include <iostream>
using namespace std;

class Tester{
    public:
    //Name: Tester - testAltConstructorNormal()
    //Precondition: None
    //Postcondition: Returns true if default board is created with valid board size
    bool testAltConstructorNormal();
    //Name: Tester - testAltConstructorError()
    //Precondition: None
    //Postcondition: Returns true if invalid board size is converted and default board is created 
    bool testAltConstructorError();
    //Name: Tester - testReStart()
    //Precondition: None
    //Postcondition: Returns true if game is reinitialized without any changes to board
    bool testReStart();
    //Name: Tester - testPlayLadder()
    //Precondition: None
    //Postcondition: Returns true if when player lands on cell with ladder, they're moved to cell at top of ladder
    bool testPlayLadder();
    //Name: Tester - testPlaySnake()
    //Precondition: None
    //Postcondition: Returns true if when player lands on cell with snake, they're moved to cell at bottom of snake
    bool testPlaySnake();
    //Name: Tester - testDefaultBoardNormal()
    //Precondition: None
    //Postcondition: Returns true if function returns true and default board is made when valid size passed and object not empty 
    bool testDefaultBoardNormal();
    //Name: Tester - testDefaultBoardError()
    //Precondition: None
    //Postcondition: Returns true if function returns false when invalid size passed or object already has board
    bool testDefaultBoardError();
    //Name: Tester - testRandomBoardNormal()
    //Precondition: None
    //Postcondition: Returns true if function cleared current board and made new random board with requested values
    bool testRandomBoardNormal();
    //Name: Tester - testRandomBoardError()
    //Precondition: None
    //Postcondition: Returns true if function converted invalid board size to valid size and readjusted out of range number of snakes and ladders to min and max
    bool testRandomBoardError();
    //Name: Tester - testOverloadedAssignmentNormal()
    //Precondition: None
    //Postcondition: Returns true if game has same values but different memory addresses as passed game 
    bool testOverloadedAssignmentNormal();
    //Name: Tester - testOverloadedAssignmentError()
    //Precondition: None
    //Postcondition: Returns true if function does not set equal to empty objects or to itself 
    bool testOverloadedAssignmentEdge();
    private:
    //Name: Tester - testEvenDistribution()
    //Precondition: size is equal to board size, cellArr is size of board
    //Postcondition: Returns true if spaces between all snakes or ladders is same 
    bool testEvenDistribution(string type, int numType, int size, Cell * cellArr[]);
};

const char * FAIL_STATEMENT = "*****TEST FAILED: ";
const char * PASS_STATEMENT = "     TEST PASSED: ";

int main(){
    Tester test;
    if (test.testAltConstructorNormal()){
        cout << PASS_STATEMENT << "Alternate constructor correctly constructed default board." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "Alternate constructor failed to construct default board." << endl;
    }

    if (test.testAltConstructorError()){
        cout << PASS_STATEMENT << "Alternate constructor correctly converted invalid board size." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "Alternate constuctor failed to convert invalid board size." << endl;
    }

    if (test.testReStart()){
        cout << PASS_STATEMENT << "reStart() reintialized the game without changing the board." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "reStart() failed to reinitialize the game." << endl;
    }

    if(test.testPlayLadder()){
        cout << PASS_STATEMENT << "play(...) performed valid move on a ladder." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "play(...) did not perform valid move on a ladder." << endl;
    }

    if (test.testPlaySnake()){
        cout << PASS_STATEMENT << "play(...) performed valid move on a snake." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "play(...) did not perform valid move on a snake." << endl;
    }

    if (test.testDefaultBoardNormal()){
        cout << PASS_STATEMENT << "makeDefaultBoard(...) worked correctly for normal case." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "makeDefaultBoard(...) did not work correctly for normal case." << endl;
    }

    if(test.testDefaultBoardError()){
        cout << PASS_STATEMENT << "makeDefaultBoard(...) worked correctly for error cases." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "makeDefaultBoard(...) did not work correctly for error cases." << endl;
    }

    if (test.testRandomBoardNormal()){
        cout << PASS_STATEMENT << "makeRandomBoard(...) worked correctly for normal cases." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "makeRandomBoard(...) did not work correctly for normal cases." << endl;
    }

    if (test.testRandomBoardError()){
        cout << PASS_STATEMENT << "makeRandomBoard(...) worked correctly for error cases." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "makeRandomBoard(...) did not work correctly for error cases." << endl;
    }

    if(test.testOverloadedAssignmentNormal()){
        cout << PASS_STATEMENT << "Overloaded assignment operator made a deep copy." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "Overloaded assignment operator failed to make a deep copy." << endl;
    }

    if (test.testOverloadedAssignmentEdge()){
        cout << PASS_STATEMENT << "Overloaded assignment operator worked correctly for edge cases." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "Overloaded assignment operator did not work correctly for edge cases." << endl;
    }

    return 0;
}

bool Tester::testAltConstructorNormal(){
    const int BOARDSIZE = 36;
    bool pass = true;

    Snakes game(BOARDSIZE);
    Cell * cellArr[BOARDSIZE];
    Cell * temp = game.m_start;
    int count = 0;
    while (temp != nullptr){
        cellArr[count] = temp;
        temp = temp->getNext();
        count++;
    }

    // Check that both players start on cell 1
    if (game.m_player1 != game.m_start || game.m_player2 != game.m_start){
        pass = false;
    }

    // Check that player 1 goes first
    if (game.m_playerTurn != PLAYER1TURN){
        pass = false;
    }

    // Check that there are no snakes or ladders starting on first cell
    if (cellArr[0]->getNorth() != nullptr || cellArr[0]->getSouth() != nullptr){
        pass = false;
    }

    // Check that there are no snakes or ladders starting on last cell
    if (cellArr[BOARDSIZE - 1]->getNorth() != nullptr || cellArr[BOARDSIZE - 1]->getSouth() != nullptr){
        pass = false;
    }

    // Check that there are no ladders leading to last cell
    for (int j = 0; j < BOARDSIZE; j++){
        if (cellArr[j]->getNorth() != nullptr){
            if (cellArr[j]->getNorth()->getID() == BOARDSIZE){
                pass = false;
            }
        }
    }

    // Check that there are no cells with both a snake and ladder
    for (int i = 0; i < BOARDSIZE; i++){
        if (cellArr[i]->getNorth() != nullptr && cellArr[i]->getSouth() != nullptr){
            pass = false;
        }
    }

    int expectedNumSL = BOARDSIZE / 4;
    int actualNumSL = 0;
    int actualNumSnakes = 0;
    int actualNumLadders = 0;

    for (int i = 0; i < BOARDSIZE; i++){
        if (cellArr[i]->getNorth() != nullptr || cellArr[i]->getSouth() != nullptr){
            actualNumSL++;
            if (cellArr[i]->getNorth() != nullptr){
                actualNumLadders++;
            }
            else{
                actualNumSnakes++;
            }
        }
    }

    // Check that total number of snakes and ladders is (BOARDSIZE / 4) + 1 or (BOARDSIZE / 4) - 1
    if (pow(actualNumSL - expectedNumSL, 2) != 1){
        pass = false;
    }

    // Check that individual number of snakes and ladders combined equals total
    if (actualNumLadders + actualNumSnakes != actualNumSL){
        pass = false;
    }

    // Check that number of snakes is equal to number of ladders, tolerating a difference of 1
    int difference = actualNumLadders - actualNumSnakes;
    if (pow(difference, 2) != 1 && pow(difference, 2) != 0){
        pass = false;
    }

    // Check that ladders are evenly distributed on board
    if (!testEvenDistribution("ladders", actualNumLadders, BOARDSIZE, cellArr)){
        pass = false;
    }

    // Check that snakes are evenly distributed
    if (!testEvenDistribution("snakes", actualNumSnakes, BOARDSIZE, cellArr)){
        pass = false;
    }
    
    return pass;
}

bool Tester::testAltConstructorError(){
    bool pass = true;
    Snakes game1(27);
    Snakes game2(19);
    Snakes game3(10);
    Snakes game4(4);

    int boardSize = 0;
    Cell * temp = game1.m_start;
    while (temp != nullptr){
        boardSize++;
        temp = temp->getNext();
    }

    // Board size should be highest perfect square less than passed size
    // Highest perfect square less than 27 is 25
    if (boardSize != 25){
        pass = false;
    }

    boardSize = 0;
    temp = game2.m_start;
    while (temp != nullptr){
        boardSize++;
        temp = temp->getNext();
    }

    // Highest perfect square less than 19 is 16
    if (boardSize != 16){
        pass = false;
    }

    boardSize = 0;
    temp = game3.m_start;
    while (temp != nullptr){
        boardSize++;
        temp = temp->getNext();
    }

    // Highest perfect square less than 10 is 9
    if (boardSize != 9){
        pass = false;
    }

    boardSize = 0;
    temp = game4.m_start;
    while (temp != nullptr){
        boardSize++;
        temp = temp->getNext();
    }

    // If passed board size is less than 9, should convert to 0 (empty object)
    if (boardSize != 0){
        pass = false;
    }

    return pass;
}

bool Tester::testReStart(){
    bool pass = true;
    const int BOARDSIZE = 25;
    
    Snakes game(BOARDSIZE);
    // Player 1 gets a 3, player 2 gets a 6
    game.play(3);
    game.play(6);

    Cell * beforeReStartArr[BOARDSIZE];
    Cell * temp = game.m_start;
    for (int i = 0; i < BOARDSIZE; i++){
        beforeReStartArr[i] = temp;
        temp = temp->getNext();
    }

    game.reStart();

    // Check that member variables point to same locations
    if (game.m_player1 != game.m_start || game.m_player2 != game.m_start || game.m_playerTurn != 1){
        pass = false;
    }

    // Check that board is same
    temp = game.m_start;
    int count = 0;
    while (temp != nullptr){
        if (temp != beforeReStartArr[count]){
            pass = false;
        }
        count++;
        temp = temp->getNext();
    }

    // Check that board size is same
    if (count != BOARDSIZE){
        pass = false;
    }
    return pass;
}

bool Tester::testPlayLadder(){
    bool pass = true;
    Snakes game(25);

    // Player 1 gets a 2, moves to cell 3 where there is a ladder to cell 23
    game.play(2);
    
    if (game.m_player1->getID() != 23){
        pass = false;
    }

    // Player 2 gets a 6, moves to cell 7 where there is a ladder to cell 19
    game.play(6);

    if (game.m_player2->getID() != 19){
        pass = false;
    }
    return pass;
}

bool Tester::testPlaySnake(){
    bool pass = true;

    Snakes game(25);
    // Player 1 gets a 6, moves to cell 7
    game.play(6);
    // Player 2 gets a 1, moves to cell 2
    game.play(1);
    // Player 1 gets a 2, moves to cell 9 where there is a snake to cell 5
    game.play(2);

    if (game.m_player1->getID() != 5){
        pass = false;
    }
    return pass;
}

bool Tester::testDefaultBoardNormal(){
    bool pass = true;
    const int BOARDSIZE = 9;
    Snakes game;

    // If object is empty and passed size is valid, function makes default board of that size and returns true
    if (!game.makeDefaultBoard(BOARDSIZE)){
        pass = false;
    }

    Cell * cellArr[BOARDSIZE];
    Cell * temp = game.m_start;
    int count = 0;
    while (temp != nullptr){
        cellArr[count] = temp;
        temp = temp->getNext();
        count++;
    }

    // Check that both players start on cell 1
    if (game.m_player1 != game.m_start || game.m_player2 != game.m_start){
        pass = false;
    }

    // Check that player 1 goes first
    if (game.m_playerTurn != PLAYER1TURN){
        pass = false;
    }

    // Check that there are no snakes or ladders starting on first cell
    if (cellArr[0]->getNorth() != nullptr || cellArr[0]->getSouth() != nullptr){
        pass = false;
    }

    // Check that there are no snakes or ladders starting on last cell
    if (cellArr[BOARDSIZE - 1]->getNorth() != nullptr || cellArr[BOARDSIZE - 1]->getSouth() != nullptr){
        pass = false;
    }

    // Check that there are no ladders leading to last cell
    for (int j = 0; j < BOARDSIZE; j++){
        if (cellArr[j]->getNorth() != nullptr){
            if (cellArr[j]->getNorth()->getID() == BOARDSIZE){
                pass = false;
            }
        }
    }

    // Check that there are no cells with both a snake and ladder
    for (int i = 0; i < BOARDSIZE; i++){
        if (cellArr[i]->getNorth() != nullptr && cellArr[i]->getSouth() != nullptr){
            pass = false;
        }
    }

    int expectedNumSL = BOARDSIZE / 4;
    int actualNumSL = 0;
    int actualNumSnakes = 0;
    int actualNumLadders = 0;

    for (int i = 0; i < BOARDSIZE; i++){
        if (cellArr[i]->getNorth() != nullptr || cellArr[i]->getSouth() != nullptr){
            actualNumSL++;
            if (cellArr[i]->getNorth() != nullptr){
                actualNumLadders++;
            }
            else{
                actualNumSnakes++;
            }
        }
    }

    // Check that total number of snakes and ladders is (BOARDSIZE / 4) + 1 or (BOARDSIZE / 4) - 1
    if (pow(actualNumSL - expectedNumSL, 2) != 1){
        pass = false;
    }

    // Check that individual number of snakes and ladders combined equals total
    if (actualNumLadders + actualNumSnakes != actualNumSL){
        pass = false;
    }

    // Check that number of snakes is equal to number of ladders, tolerating a difference of 1
    int difference = actualNumLadders - actualNumSnakes;
    if (pow(difference, 2) != 1 && pow(difference, 2) != 0){
        pass = false;
    }

    // Check that ladders are evenly distributed on board
    if (!testEvenDistribution("ladders", actualNumLadders, BOARDSIZE, cellArr)){
        pass = false;
    }

    // Check that snakes are evenly distributed
    if (!testEvenDistribution("snakes", actualNumSnakes, BOARDSIZE, cellArr)){
        pass = false;
    }

    return pass;
}

bool Tester::testDefaultBoardError(){
    bool pass = true;
    const int BOARDSIZE = 25;

    // Error Case #1: Non-empty object
    Snakes defaultGame(BOARDSIZE);

    Cell * cellArr[BOARDSIZE];
    Cell * temp = defaultGame.m_start;
    int count = 0;
    while (temp != nullptr){
        cellArr[count] = temp;
        count++;
        temp = temp->getNext();
    }
    
    // Should return false is object already has a board
    if (defaultGame.makeDefaultBoard(BOARDSIZE)){
        pass = false;
        cout << "1" << endl;
    }

    // Check that board was not modified
    temp = defaultGame.m_start;
    count = 0;
    while (temp != nullptr){
        if (temp != cellArr[count]){
            pass = false;
            cout << "2" << endl;
        }
        count++;
        temp = temp->getNext();
    }

    // Error Case #2: Invalid board size
    Snakes game1;
    Snakes game2;
    Snakes game3;

    // If size is not a perfect square, function should return false and not modify object
    if (game1.makeDefaultBoard(10) || !game1.isEmptyObject()){
        pass = false;
        cout << "3" << endl;
    }

    // If size is less than 9 and imperfect square, should return false and not modify object
    if (game2.makeDefaultBoard(8) || !game2.isEmptyObject()){
        pass = false;
        cout << "4" << endl;
    }

    // If size is less than 9, even if perfect square, should return false and not modify object
    if (game2.makeDefaultBoard(4) || !game3.isEmptyObject()){
        pass = false;
        cout << "5" << endl;
    }

    return pass;
}

bool Tester::testRandomBoardNormal(){
    bool pass = true;
    const int BOARDSIZE = 25;
    int numSnakesLadders = 7;

    Snakes game1;
    game1.makeRandomBoard(BOARDSIZE, numSnakesLadders);

    Cell * temp = game1.m_start;
    Cell * cellArr[BOARDSIZE];
    int count = 0;
    while (temp != nullptr){
        cellArr[count] = temp;
        temp = temp->getNext();
        count++;
    }

    // Check that both players start on cell 1
    if (game1.m_player1 != game1.m_start || game1.m_player2 != game1.m_start){
        pass = false;
    }

    // Check that player 1 goes first
    if (game1.m_playerTurn != PLAYER1TURN){
        pass = false;
    }

    // Check that there are no snakes or ladders starting on first cell
    if (cellArr[0]->getNorth() != nullptr || cellArr[0]->getSouth() != nullptr){
        pass = false;
    }

    // Check that there are no snakes or ladders starting on last cell
    if (cellArr[BOARDSIZE - 1]->getNorth() != nullptr || cellArr[BOARDSIZE - 1]->getSouth() != nullptr){
        pass = false;
    }

    // Check that there are no ladders leading to last cell
    for (int j = 0; j < BOARDSIZE; j++){
        if (cellArr[j]->getNorth() != nullptr){
            if (cellArr[j]->getNorth()->getID() == BOARDSIZE){
                pass = false;
            }
        }
    }

    // Check that there are no cells with both a snake and ladder
    for (int i = 0; i < BOARDSIZE; i++){
        if (cellArr[i]->getNorth() != nullptr && cellArr[i]->getSouth() != nullptr){
            pass = false;
        }
    }

    int numSnakes = 0;
    int numLadders = 0;
    for (int x = 0; x < BOARDSIZE; x++){
        if (cellArr[x]->getNorth() != nullptr){
            numLadders++;
        }
        else if (cellArr[x]->getSouth() != nullptr){
            numSnakes++;
        }
    }

    // Check that individual number of snakes and ladders combined equals total
    if (numLadders + numSnakes != numSnakesLadders){
        pass = false;
    }

    // Check that number of snakes is equal to number of ladders, tolerating a difference of 1
    int difference = numLadders - numSnakes;
    if (pow(difference, 2) != 1 && pow(difference, 2) != 0){
        pass = false;
    }

    // Check that ladders are NOT evenly distributed on board
    if (testEvenDistribution("ladders", numLadders, BOARDSIZE, cellArr)){
        pass = false;
    }

    // Check that snakes are NOT evenly distributed
    if (testEvenDistribution("snakes", numSnakes, BOARDSIZE, cellArr)){
        pass = false;
    }

    const int MINSNAKESLADDERS = 2;
    const int MAXSNAKESLADDERS = (BOARDSIZE / 2) - 2;
    Snakes game2;
    Snakes game3;
    game2.makeRandomBoard(BOARDSIZE, MINSNAKESLADDERS);
    game3.makeRandomBoard(BOARDSIZE, MAXSNAKESLADDERS);

    // Edge Case #1: Test that min number of snakes and ladders works
    temp = game2.m_start;
    int numSL = 0;
    while (temp != nullptr){
        if (temp->getNorth() != nullptr || temp->getSouth() != nullptr){
            numSL++;
        }
        temp = temp->getNext();
    }
    if (numSL != MINSNAKESLADDERS){
        pass = false;
    }

    // Edge Case #2: Test that max number of snakes and ladders works
    temp = game3.m_start;
    numSL = 0;
    while (temp != nullptr){
        if (temp->getNorth() != nullptr || temp->getSouth() != nullptr){
            numSL++;
        }
        temp = temp->getNext();
    }
    if (numSL != MAXSNAKESLADDERS){
        pass = false;
    }

    return pass;
}

bool Tester::testRandomBoardError(){
    bool pass = true;
    const int VALIDBOARDSIZE = 25;
    const int NONPERFECTSQUARE = 27;
    const int MINBOARDSIZE = 9;
    const int MINNUMSL = 2;
    const int MAXNUMSL = (VALIDBOARDSIZE / 2) - 2;

    Snakes game1;
    Snakes game2;
    Snakes game3;
    Snakes game4;
    game1.makeRandomBoard(NONPERFECTSQUARE, MINNUMSL);
    game2.makeRandomBoard(MINBOARDSIZE - 5, MINNUMSL);
    game3.makeRandomBoard(VALIDBOARDSIZE, MINNUMSL - 1);
    game4.makeRandomBoard(VALIDBOARDSIZE, MAXNUMSL + 1);

    // Error case #1: Passed board size does not have integer square root
    Cell * temp = game1.m_start;
    int size = 0;
    while (temp != nullptr){
        size++;
        temp = temp->getNext();
    }
    if (size != pow(floor(sqrt(double(NONPERFECTSQUARE))), 2)){
        pass = false;
    }

    // Error case #2: Passed board size is less than 9 (smaller than 3x3 board)
    if (game2.m_start != nullptr){
        pass = false;
    }

    // Error case #3: Passed num of snakes and ladder is less than min
    temp = game3.m_start;
    int numSL = 0;
    while (temp != nullptr){
        if (temp->getNorth() != nullptr || temp->getSouth() != nullptr){
            numSL++;
        }
        temp = temp->getNext();
    }

    // If num of snakes and ladders is less than min, should be set to min
    if (numSL != MINNUMSL){
        pass = false;
    }

    // Error case #4: Passed num of snakes and ladders is more than max
    temp = game4.m_start;
    numSL = 0;
    while (temp != nullptr){
        if (temp->getNorth() != nullptr || temp->getSouth() != nullptr){
            numSL++;
        }
        temp = temp->getNext();
    }

    // If num of snakes and ladders is more than max, should be set to max
    if (numSL != MAXNUMSL){
        pass = false;
    }

    return pass;
}

bool Tester::testOverloadedAssignmentNormal(){
    bool pass = true;

    Snakes game1(25);
    game1.play(1);
    game1.play(4);
    game1.play(2);
    Snakes game2(9);
    game2 = game1;

    // Check that current cells player 1 is on have different addresses, but same values
    if (game2.m_player1 != game1.m_player1){
        if (game2.m_player1->getID() != game1.m_player1->getID()){
            pass = false;
        }
    }
    else{
        pass = false;
    }

    // Check that current cells player 2 is on have different addresses, but same values
    if (game2.m_player2 != game1.m_player2){
        if (game2.m_player2->getID() != game1.m_player2->getID()){
            pass = false;
        }
    }
    else{
        pass = false;
    }

    // Check that player turn is same
    if (game2.m_playerTurn != game1.m_playerTurn){
        pass = false;
    }

    Cell * original = game1.m_start;
    Cell * copy = game2.m_start; 
    for (int i = 0; i < 25; i++){
        // Cell addresses should be different
        if (original != copy){
            // Cell values should be same
            if (original->getID() == copy->getID()){
                // Both cells should either have a ladder or not have a ladder
                if (original->getNorth() != nullptr && copy->getNorth() != nullptr){
                    // Addresses of ladder destinations should be different
                    if (original->getNorth() != copy->getNorth()){
                        // Cell number of ladder destinations should be same
                        if (original->getNorth()->getID() != copy->getNorth()->getID()){
                            pass = false;
                        }
                    }
                    else{
                        pass = false;
                    }
                }
                else if (original->getNorth() == nullptr && copy->getNorth() == nullptr){

                }
                else{
                    pass = false;
                }

                // Both cells should either have a snake or not have a snake
                if (original->getSouth() != nullptr && copy->getSouth() != nullptr){
                    // Addresses of snake destinations should be different
                    if (original->getSouth() != copy->getSouth()){
                        // Cell number of snake destinations should be same
                        if (original->getSouth()->getID() != copy->getSouth()->getID()){
                            pass = false;
                        }
                    }
                }
                else if (original->getSouth() == nullptr && copy->getSouth() == nullptr){

                }
                else{
                    pass = false;
                }
            }
            else{
                pass = false;
            }
        }
        else{
            pass = false;
        }
    }

    return pass;
}

bool Tester::testOverloadedAssignmentEdge(){
    bool pass = true;
    Snakes emptyObject;
    Snakes game(9);

    // Edge Case #1: Copying an empty object
    game = emptyObject;

    // Should not be empty object (did nothing)
    if (game.m_start == nullptr || game.m_player1 == nullptr || game.m_player2 == nullptr || game.m_playerTurn == 0){
        pass = false;
    }

    // Edge Case #2: Copying an object to itself
    Cell * beforeAssignmentArr[9];
    Cell * temp = game.m_start;
    for (int i = 0; i < 9; i++){
        beforeAssignmentArr[i] = temp;
        temp = temp->getNext();
    }
    game = game;

    Cell * afterAssignmentArr[9];
    temp = game.m_start;
    for (int i = 0; i < 9; i++){
        afterAssignmentArr[i] = temp;
        temp = temp->getNext();
    }
    // Addresses should be same as before (did nothing)
    for (int i = 0; i < 9; i++){
        if (beforeAssignmentArr[i] != afterAssignmentArr[i]){
            pass = false;
        }
    }
    
    return pass;
}

bool Tester::testEvenDistribution(string type, int numType, int size, Cell * cellArr[]){
    int count = 0;
    int spacing = 0;
    bool pass = true;
    Cell * temp;

    int spaceArr[numType - 1];
    for (int k = 0; k < size; k++){
        if (type == "ladders"){
            temp = cellArr[k]->getNorth();
        }
        else if (type == "snakes"){
            temp = cellArr[k]->getSouth();
        }

        if (temp != nullptr && count == 0){
            spacing = 0;
            count++;
        }
        else if (temp != nullptr && count > 0){
            spaceArr[count - 1] = spacing;
            spacing = 0;
            count++;
        }
        spacing++;
        
    }

    // Check that space between each snake or ladder is same
    for (int x = 0; x < numType - 1; x++){
        if (spaceArr[x] != 4){
            pass = false;
        }
    }
    return pass;
}