// UMBC - CMSC 341 - Fall 2024 - Proj0
#include "solitaire.h"
using namespace std;
class Tester{
    public:
    bool testCopyConstructorNormal(const Solitaire& solitaire);
};
int main(){
    cout << "Create a diamond board:" << endl;
    Solitaire pegSolitaire(ENGLISH);
    pegSolitaire.dumpBoard();
    cout << "Make a move from (6,4) to (4,4):" << endl;
    pegSolitaire.play(pair<int,int>(3,1), pair<int,int>(3,3));
    pegSolitaire.dumpBoard();
    cout << "Make a move from (6,6) to (6,4):" << endl;
    pegSolitaire.play(pair<int,int>(1,2), pair<int,int>(3,2));
    pegSolitaire.dumpBoard();
    pegSolitaire.reportNumMarbles();
    Tester aTester;
    cout << endl;
    cout << "Testing copy constructor normal case:" << endl;
    if (aTester.testCopyConstructorNormal(pegSolitaire))
        cout << "\tCopy constructor test normal case passed!" << endl;
    else
        cout << "\tCopy constructor test normal case failed!" << endl;
    return 0;
}

bool Tester::testCopyConstructorNormal(const Solitaire& solitaire){
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
