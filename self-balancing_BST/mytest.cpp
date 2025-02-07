// UMBC - CMSC 341 - Fall 2024 - Proj2
#include "swarm.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>
using namespace std;
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};
class Random {
public:
    Random(){}
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else if (type == UNIFORMREAL) { //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
        else { //the case of SHUFFLE to generate every number only once
            m_generator = std::mt19937(m_device());
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }
    void init(int min, int max){
        m_min = min;
        m_max = max;
        m_type = UNIFORMINT;
        m_generator = std::mt19937(10);// 10 is the fixed seed value
        m_unidist = std::uniform_int_distribution<>(min,max);
    }
    void getShuffle(vector<int> & array){
        // this function provides a list of all values between min and max
        // in a random order, this function guarantees the uniqueness
        // of every value in the list
        // the user program creates the vector param and passes here
        // here we populate the vector using m_min and m_max
        for (int i = m_min; i<=m_max; i++){
            array.push_back(i);
        }
        shuffle(array.begin(),array.end(),m_generator);
    }

    void getShuffle(int array[]){
        // this function provides a list of all values between min and max
        // in a random order, this function guarantees the uniqueness
        // of every value in the list
        // the param array must be of the size (m_max-m_min+1)
        // the user program creates the array and pass it here
        vector<int> temp;
        for (int i = m_min; i<=m_max; i++){
            temp.push_back(i);
        }
        std::shuffle(temp.begin(), temp.end(), m_generator);
        vector<int>::iterator it;
        int i = 0;
        for (it=temp.begin(); it != temp.end(); it++){
            array[i] = *it;
            i++;
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }

    string getRandString(int size){
        // the parameter size specifies the length of string we ask for
        // to use ASCII char the number range in constructor must be set to 97 - 122
        // and the Random type must be UNIFORMINT (it is default in constructor)
        string output = "";
        for (int i=0;i<size;i++){
            output = output + (char)getRandNum();
        }
        return output;
    }
    
    int getMin(){return m_min;}
    int getMax(){return m_max;}
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};

class Tester{
    public:
    //Name: Tester - testInsertAVL()
    //Precondition: None
    //Postcondition: Returns true if AVL tree is balanced, preserves BST property, and height values
    //               are correct after a decent number of insertions.
    bool testInsertAVL();
    //Name: Tester - testInsertSplay()
    //Precondition: None
    //Postcondition: Returns true if Splay tree splays inserted node and preserves BST property after
    //               each insertion.
    bool testInsertSplay();
    //Name: Tester - testRemoveNormalBST()
    //Precondition: None
    //Postcondition: Returns true if removing a node from a BST tree results in a tree without node.
    bool testRemoveNormalBST();
    //Name: Tester - testRemoveEdgeBST()
    //Precondition: None
    //Postcondition: Returns true if removing a node from a BST tree with one node results in an empty tree.
    bool testRemoveEdgeBST();
    //Name: Tester - testRemoveAVL()
    //Precondition: None
    //Postcondition: Returns true if AVL tree is balanced, preserves BST property, and height values are
    //               correct after a decent number of removals.
    bool testRemoveAVL();
    //Name: Tester - testOverloadedAssignmentNormal()
    //Precondition: None
    //Postcondition: Returns true if swarm has same values but different memory addresses as passed swarm 
    bool testAssignmentOperatorNormal();
    //Name: Tester - testOverloadedAssignmentError()
    //Precondition: None
    //Postcondition: Returns true if function does not allocate memory when set equal to empty objects or 
    //               to itself
    bool testAssignmentOperatorError();
    private:
    //Name: Tester - testBSTProperty()
    //Precondition: None
    //Postcondition: Returns true if every node's value is greater than all nodes in its left subtree and
    //               less than all values in its right subtree
    bool testBSTProperty(Robot * aBot);
    //Name: Tester - testOverloadedAssignmentError()
    //Precondition: None
    //Postcondition: Returns true if passed tree is balanced (height differ by at most 1)
    bool testBalance(Robot * aBot);
    //Name: Tester - testHeightValues()
    //Precondition: None
    //Postcondition: Returns true if height values of all nodes in tree are correct
    bool testHeightValues(Robot * aBot);
    //Name: Tester - getActualHeight()
    //Precondition: None
    //Postcondition: Calculates and returns height of passed node
    int getActualHeight(Robot * aBot);
    //Name: Tester - findID()
    //Precondition: None
    //Postcondition: Returns true if passed id is in tree, otherwise returns false
    bool findID(int id, Robot * aBot);
    //Name: Tester - checkDeepCopy()
    //Precondition: None
    //Postcondition: Returns true if copy swarm is a deep copy of original swarm
    bool checkDeepCopy(Robot * original, Robot * copy);
};

const char * FAIL_STATEMENT = "*****TEST FAILED: ";
const char * PASS_STATEMENT = "     TEST PASSED: ";

int main(){
    Tester test;
    if (test.testInsertAVL()){
        cout << PASS_STATEMENT << "AVL tree is balanced after multiple insertions." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "AVL tree is not balanced after multiple insertions." << endl;
    }

    if (test.testInsertSplay()){
        cout << PASS_STATEMENT << "Splay tree splayed inserted node." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "Splay tree did not splay inserted node." << endl;
    }

    if (test.testRemoveAVL()){
        cout << PASS_STATEMENT << "AVL tree is balanced after multiple removals." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "AVL tree is not balanced after multiple removals." << endl;
    }

    if (test.testRemoveNormalBST()){
        cout << PASS_STATEMENT << "Remove function worked correctly for BST normal case." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "Remove function did not work correctly for BST normal case." << endl;
    }
    
    if (test.testRemoveEdgeBST()){
        cout << PASS_STATEMENT << "Remove function worked correctly for BST edge case." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "Remove function did not work correctly for BST edge case." << endl;
    }

    if (test.testAssignmentOperatorNormal()){
        cout << PASS_STATEMENT << "Assignment operator worked correctly for normal case." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "Assignment operator did not work correctly for normal case." << endl;
    }

    if (test.testAssignmentOperatorError()){
        cout << PASS_STATEMENT << "Assignment operator worked correctly for error case." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "Assignment operator did not work correctly for error case." << endl;
    }

    return 0;
}

bool Tester::testInsertAVL(){
    Random idGen(MINID,MAXID);
    Random typeGen(0,4);

    // Create random AVL tree of size 300
    Swarm team(AVL);
    int teamSize = 300;
    int ID = 0;
    ROBOTTYPE roboType = DEFAULT_TYPE;
    for(int i = 0; i < teamSize; i++){
        ID = idGen.getRandNum();
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(ID,roboType);
        team.insert(robot);
    }

    // Check that height of each node is correct
    if (!testHeightValues(team.m_root)){
        return false;
    }
    // Check that each node's value is greater than all nodes in its left subtree and
    // less than all values in its right subtree
    if (!testBSTProperty(team.m_root)){
        return false;
    }
    // Check that tree is balanced
    if (!testBalance(team.m_root)){
        return false;
    }
    return true;
}

bool Tester::testBSTProperty(Robot * aBot){
    // If node is null, return true
    if (aBot == nullptr){
        return true;
    }
    else{
        // Node has both left and right child
        if (aBot->m_left != nullptr && aBot->m_right != nullptr){
            // If node's value is less than right child's value, greater than left child's,
            // and same is true for all their children, return true. Otherwise, return false.
            if (aBot->m_id > aBot->m_left->m_id &&
                aBot->m_id < aBot->m_right->m_id &&
                testBSTProperty(aBot->m_left) &&
                testBSTProperty(aBot->m_right)){
                return true;
            }
            else{
                return false;
            }
        }
        // Node only has right child
        else if (aBot->m_right != nullptr){
            // If node's value is less than right child's value, and same is true for all right
            // child's children, return true. Otherwise, return false.
            if (aBot->m_id < aBot->m_right->m_id &&
                testBSTProperty(aBot->m_right)){
                return true;
            }
            else{
                return false;
            }
        }
        // Node only has left child
        else if (aBot->m_left != nullptr){
            // If node's value is greater than left child's value, and same is true for all left
            // child's children, return true. Otherwise, return false.
            if (aBot->m_id > aBot->m_left->m_id &&
                testBSTProperty(aBot->m_left)){
                return true;
            }
            else{
                return false;
            }
        }
        else{
            return true;
        }
    }
}

bool Tester::testBalance(Robot * aBot){
    // If node is null, return true
    if (aBot == nullptr){
        return true;
    }

    // Calculate children's heights
    int leftHeight = -1;
    int rightHeight = -1;
    if (aBot->m_left != nullptr){
        leftHeight = aBot->m_left->m_height;
    }
    if (aBot->m_right != nullptr){
        rightHeight = aBot->m_right->m_height;
    }

    // Check that height difference is at most 1
    if (leftHeight - rightHeight == -1 ||
        leftHeight - rightHeight == 1 ||
        leftHeight - rightHeight == 0){
        // If same is true for all children, return true. Otherwise, return false.
        if (testBalance(aBot->m_left) && testBalance(aBot->m_right)){
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

bool Tester::testHeightValues(Robot * aBot){
    // If node is null, return true
    if (aBot == nullptr){
        return true;
    }
    // If node's m_height matches calculated height and same is true for all their children,
    // return true, otherwise return false.
    if (aBot->m_height == getActualHeight(aBot) && 
        testHeightValues(aBot->m_left) &&
        testHeightValues(aBot->m_right)){
        return true;
    }
    else{
        return false;
    }
}

int Tester::getActualHeight(Robot * aBot){
    // If node is null, height is -1
    if (aBot == nullptr){
        return -1;
    }

    // Calculate left child's height and right child's height
    int leftHeight = getActualHeight(aBot->m_left);
    int rightHeight = getActualHeight(aBot->m_right);

    // Set nodes height to whichever child's height is greater plus 1
    if (leftHeight > rightHeight || leftHeight == rightHeight){
        return leftHeight + 1;
    }
    else{
        return rightHeight + 1;
    }
}

bool Tester::testInsertSplay(){
    Random idGen(MINID,MAXID);
    Random typeGen(0,4);
    
    // Create random splay tree of size 300
    Swarm team(SPLAY);
    int teamSize = 300;
    int ID = 0;
    ROBOTTYPE roboType = DEFAULT_TYPE;

    for(int i = 0; i < teamSize; i++){
        ID = idGen.getRandNum();
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(ID,roboType);
        team.insert(robot);

        // Check that after each insertion, inserted node is splayed and BST property preserved
        if (team.m_root->getID() != ID){
            return false;
        }
        if (!testBSTProperty(team.m_root)){
            return false;
        }
    }

    // Check that each node's height value is correct
    if (!testHeightValues(team.m_root)){
        return false;
    }

    return true;
}

bool Tester::testRemoveNormalBST(){
    // Case 1: Node to remove has no children
    Swarm team1(BST);
    for(int i = 10000; i < 40000; i = i + 10000){
        Robot robot(i, BIRD);
        team1.insert(robot);
    }

    // Before removing, 30000 should be in tree
    if (!findID(30000, team1.m_root)){
        return false;
    }

    team1.remove(30000);
    
    // After removing, 30000 should not be in tree
    if (findID(30000, team1.m_root)){
        return false;
    }

    // Check height values are updated
    if (!testHeightValues(team1.m_root)){
        return false;
    }

    // Check BST property preserved
    if (!testBSTProperty(team1.m_root)){
        return false;
    }

    // Case 2: Node to remove has 1 child
    Swarm team2(BST);
    for(int i = 10000; i < 40000; i = i + 10000){
        Robot robot(i, BIRD);
        team2.insert(robot);
    }

    // Prior to removing 20000, 20000 should be in tree
    if (!findID(20000, team2.m_root)){
        return false;
    }

    team2.remove(20000);

    // After removing 20000, should not be in tree
    if (findID(20000, team2.m_root)){
        return false;
    }

    // Check that 30000 replaced 20000
    if (team2.m_root->getRight()->getID() != 30000){
        return false;
    }

    if (!testHeightValues(team2.m_root)){
        return false;
    }

    if (!testBSTProperty(team2.m_root)){
        return false;
    }

    // Case 3: Node to remove has 2 children
    Swarm team3(BST);
    for(int i = 10000; i < 60000; i = i + 20000){
        Robot robot(i, BIRD);
        team3.insert(robot);
    }
    Robot robot(20000, BIRD);
    team3.insert(robot);

    // Before removing, 30000 should be in tree
    if (!findID(30000, team3.m_root)){
        return false;
    }

    team3.remove(30000);

    // After removing, 30000 should not be in tree
    if (findID(30000, team3.m_root)){
        return false;
    }
    
    // Check that removed node's right child's min (50000) replaced it
    if (team3.m_root->getRight()->getID() != 50000){
        return false;
    }

    // Check that removed node's left child is same and node that used to be 50000 is removed
    if (team3.m_root->getRight()->getRight() != nullptr ||
        team3.m_root->getRight()->getLeft()->getID() != 20000){
        return false;
    }

    if (!testHeightValues(team3.m_root)){
        return false;
    }

    if (!testBSTProperty(team3.m_root)){
        return false;
    }

    return true;
}

bool Tester::findID(int id, Robot * aBot){
    // If node is null, return false
    if (aBot == nullptr){
        return false;
    }
    // If current node's id matches or one of its children's matches, return true
    // Otherwise, return false
    if (aBot->getID() == id || findID(id, aBot->m_left) || findID(id, aBot->m_right)){
        return true;
    }
    else{
        return false;
    }
}

bool Tester::testRemoveEdgeBST(){
    // Create BST tree with one node
    Swarm team(BST);
    Robot robot(10000, BIRD);
    team.insert(robot);

    // Check that node is m_root
    if (team.m_root->getID() != 10000){
        return false;
    }

    // Check that node is only node and height is 0
    if (team.m_root->getHeight() != 0 ||
        team.m_root->getLeft() != nullptr ||
        team.m_root->getRight() != nullptr){
        return false;
    }

    team.remove(10000);

    // Check that after removing, tree becomes empty
    if (team.m_root != nullptr){
        return false;
    }

    return true;
}

bool Tester::testRemoveAVL(){
    // Create random AVL tree of size 300
    Random idGen(MINID,MAXID);
    Random typeGen(0,4);

    Swarm team(AVL);
    int teamSize = 300;
    int ID = 0;
    ROBOTTYPE roboType = DEFAULT_TYPE;

    Random indexGen(0, teamSize);
    int IDArray[teamSize] = {0};
    int indexArray[teamSize / 2] = {0};
    int index = 0;

    for(int i = 0; i < teamSize; i++){
        ID = idGen.getRandNum();
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(ID,roboType);
        team.insert(robot);
        IDArray[i] = ID;
    }

    // Randomly remove half of the nodes
    for (int j = 0; j < (teamSize / 2); j++){
        bool diffIndex = false;
        while (!diffIndex){
            bool match = false;
            index = indexGen.getRandNum();
            for (int y = 0; y < j; y++){
                if (index == indexArray[y]){
                    match = true;
                }
            }
            if (!match){
                diffIndex = true;
            }
        }
        team.remove(IDArray[index]);
    }

    // Check that after removing, height values are correct
    if (!testHeightValues(team.m_root)){
        return false;
    }
    // Check that BST property is preserved
    if (!testBSTProperty(team.m_root)){
        return false;
    }
    // Check that tree is still balanced
    if (!testBalance(team.m_root)){
        return false;
    }

    return true;
}

bool Tester::testAssignmentOperatorNormal(){
    // Create random AVL tree of size 100
    Random idGen(MINID,MAXID);
    Random typeGen(0,4);

    Swarm team1(AVL);
    int teamSize = 100;
    int ID = 0;
    ROBOTTYPE roboType = DEFAULT_TYPE;
    for(int i = 0; i < teamSize; i++){
        ID = idGen.getRandNum();
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(ID,roboType);
        team1.insert(robot);
    }

    // Create a random splay tree of size 50
    Swarm team2(SPLAY);
    for(int i = 0; i < 5; i++){
        Robot robot(i * 10000, BIRD);
        team2.insert(robot);
    }

    // Set second tree equal to first tree
    team2 = team1;

    // Check that assignment operator made a deep copy
    if (!checkDeepCopy(team1.m_root, team2.m_root)){
        return false;
    }

    return true;
}

bool Tester::checkDeepCopy(Robot * original, Robot * copy){
    // If both objects are empty, return true
    // If one is empty and the other is not, return false
    if (original == nullptr && copy == nullptr){
        return true;
    }
    else if ((original == nullptr && copy != nullptr) ||
            (original != nullptr && copy == nullptr)){
        return false;
    }

    // If values are same and addresses are different, return true, otherwise false
    if (original != copy && original->getID() == copy->getID() &&
        original->getHeight() == copy->getHeight() &&
        original->getType() == copy->getType() &&
        original->getState() == copy->getState() &&
        checkDeepCopy(original->m_left, copy->m_left) &&
        checkDeepCopy(original->m_right, copy->m_right)){
        return true;
    }
    else{
        return false;
    }
}

bool Tester::testAssignmentOperatorError(){
    // Error Case 1: Set empty object equal to empty object
    Swarm emptyTeam1;
    Swarm emptyTeam2;

    emptyTeam1 = emptyTeam2;
    
    // Should still be empty
    if (emptyTeam1.m_root != nullptr || emptyTeam1.m_type != NONE){
        return false;
    }

    // Error Case 2: Set empty object equal to non-empty object
    Swarm team1(BST);
    Robot robot1(10000, BIRD);
    team1.insert(robot1);
    emptyTeam1 = team1;

    // Object should not be empty, values should be same and addresses different
    if (emptyTeam1.m_root == nullptr || emptyTeam1.m_root->getID() != team1.m_root->getID() ||
        emptyTeam1.m_root == team1.m_root || emptyTeam1.m_type != team1.m_type){
        return false;
    }

    // Error Case 3: Set non empty object equal to empty object
    team1 = emptyTeam2;

    // Object should be empty
    if (team1.m_root != nullptr || team1.m_type != NONE){
        return false;
    }

    return true;
}