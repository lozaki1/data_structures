// CMSC 341 - Fall 2024 - Project 3
#include "mqueue.h"
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

int priorityFn1(const Order &order);
int priorityFn2(const Order &order);

class Tester{
    public:
    //Name: Tester - testInsertNormalMin()
    //Precondition: None
    //Postcondition: Returns true if heap property is preserved in min heap after 300 insertions.
    bool testInsertNormalMin();
    //Name: Tester - testInsertNormalMax()
    //Precondition: None
    //Postcondition: Returns true if heap property is preserved in max heap after 300 insertions.
    bool testInsertNormalMax();
    //Name: Tester - testRemoveNormalMin()
    //Precondition: None
    //Postcondition: Returns true if all removals in min heap happen in correct order.
    bool testRemoveNormalMin();
    //Name: Tester - testRemoveNormalMax()
    //Precondition: None
    //Postcondition: Returns true if all removals in max heap happen in correct order
    bool testRemoveNormalMax();
    //Name: Tester - testLeftistNPL()
    //Precondition: None
    //Postcondition: Returns true if leftist heap preserves leftist property after 300 insertions and
    //               all nodes have correct NPL value.
    bool testLeftistNPL();
    //Name: Tester - testSetPriorityFn()
    //Precondition: None
    //Postcondition: Returns true if after changing the priority function a correct heap is rebuilt 
    //               with the same data (nodes) and the different priority function.
    bool testSetPriorityFn();
    //Name: Tester - testMergeEmptyQueue()
    //Precondition: None
    //Postcondition: Returns true if correctly merges a non empty queue with empty queue.
    bool testMergeEmptyQueue();
    //Name: Tester - testCopyConstructorNormal()
    //Precondition: None
    //Postcondition: Returns true if copy constructor works correctly for normal case.
    bool testCopyConstructorNormal();
    //Name: Tester - testCopyConstructorEdge()
    //Precondition: None
    //Postcondition: Returns true if copy constructor works correctly for edge case.
    bool testCopyConstructorEdge();
    //Name: Tester - testAssignmentOperatorNormal()
    //Precondition: None
    //Postcondition: Returns true if assignment operator works correctly for normal case.
    bool testAssignmentOperatorNormal();
    //Name: Tester - testAssignmentOperatorEdge()
    //Precondition: None
    //Postcondition: Returns true if assignment operator works correctly for edge case.
    bool testAssignmentOperatorEdge();
    //Name: Tester - testDequeueEmptyQueue()
    //Precondition: None
    //Postcondition: Returns true if dequeueing an empty queue throws an out of range error.
    bool testDequeueEmptyQueue();
    //Name: Tester - testMergeDifferentQueues()
    //Precondition: None
    //Postcondition: Returns true if merging two different heap types throws a domain error.
    bool testMergeDifferentQueues();
    private:
    //Name: Tester - testHeapProperty()
    //Precondition: None
    //Postcondition: Returns true if priority of passed node is higher than all its children
    bool testHeapProperty(prifn_t priFn, HEAPTYPE heapType, Node * aNode);
    //Name: Tester - getNPL()
    //Precondition: None
    //Postcondition: Returns actual npl value of passed node.
    int getNPL(Node * aNode); 
    //Name: Tester - testNPLValues()
    //Precondition: None
    //Postcondition: Returns true if all npl values match actual npl values.
    bool testNPLValues(Node * aNode);
    //Name: Tester - checkAddresses()
    //Precondition: None
    //Postcondition: Returns true values of passed nodes are same, but addresses are different
    bool checkAddresses(prifn_t priFn, Node * copy, Node * original);
    //Name: Tester - createNodeArray()
    //Precondition: None
    //Postcondition: Adds all nodes in tree to passed array
    void createNodeArray(Node * arr[], Node * aNode, int size);
    //Name: Tester - testLeftistProperty()
    //Precondition: None
    //Postcondition: Returns true if node's left child has greater or equal npl value to right child
    bool testLeftistProperty(Node * aNode);
};

const char * FAIL_STATEMENT = "*****TEST FAILED: ";
const char * PASS_STATEMENT = "     TEST PASSED: ";

int main(){
    Tester test;
    if (test.testInsertNormalMin()){
        cout << PASS_STATEMENT << "Heap property preserved in min heap after 300 insertions." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "Heap property not preserved in min heap after 300 insertions." << endl;
    }
    if (test.testInsertNormalMax()){
        cout << PASS_STATEMENT << "Heap property preserved in max heap after 300 insertions." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "Heap property not preserved in max heap after 300 insertions." << endl;
    }
    if (test.testRemoveNormalMin()){
        cout << PASS_STATEMENT << "Heap property preserved in min heap after 150 removals." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "Heap property not preserved in min heap after 150 removals." << endl;
    }
    if (test.testRemoveNormalMax()){
        cout << PASS_STATEMENT << "Heap property preserved in max heap after 150 removals." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "Heap property not preserved in max heap after 150 removals." << endl;
    }
    if (test.testLeftistNPL()){
        cout << PASS_STATEMENT << "All nodes in leftist heap had correct NPL values." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "All nodes in leftist heap did not have correct NPL values." << endl;
    }
    if (test.testSetPriorityFn()){
        cout << PASS_STATEMENT << "Min heap was correctly rebuilt into max heap." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "Min heap was not correctly rebuilt into max heap." << endl;
    }
    if (test.testMergeEmptyQueue()){
        cout << PASS_STATEMENT << "Merge worked correctly for empty queue." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "Merge did not work correctly for empty queue." << endl;
    }
    if (test.testCopyConstructorNormal()){
        cout << PASS_STATEMENT << "Copy constructor normal worked." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "Copy constructor normal did not work." << endl;
    }
    if (test.testCopyConstructorEdge()){
        cout << PASS_STATEMENT << "Copy constructor edge worked." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "Copy constructor edge did not work." << endl;
    }
    if (test.testAssignmentOperatorNormal()){
        cout << PASS_STATEMENT << "Assignment operator normal worked." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "Assignment operator normal did not work." << endl;
    }
    if (test.testAssignmentOperatorEdge()){
        cout << PASS_STATEMENT << "Assignment operator edge worked." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "Assignment operator edge did not work." << endl;
    }
    if (test.testDequeueEmptyQueue()){
        cout << PASS_STATEMENT << "Dequeueing an empty queue threw an out of range error." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "Dequeueing an empty queue did not throw an out of range error." << endl;
    }
    if (test.testMergeDifferentQueues()){
        cout << PASS_STATEMENT << "Merging with different queues threw a domain error." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "Merging with different queues did not throw a domain error." << endl;
    }
    return 0;
}

bool Tester::testInsertNormalMin(){
    // Create random min heap of size 300
    Random nameGen(97,122);
    Random FIFO(MINONE,MAX50); Random processTime(MINONE,MAX12);
    Random dueTime(MINONE,MAX12); Random slackTime(MINONE,MAX12);
    Random material(MINZERO,MAX100); Random importance(MINONE,MAX100);
    Random workers(MAX100,MAX200); Random quantity(MIN1000,MAX10000);
    MQueue queue1(priorityFn2, MINHEAP, SKEW);
    for (int i=0;i<300;i++){
        Order order(nameGen.getRandString(5),
                    FIFO.getRandNum(), processTime.getRandNum(),
                    dueTime.getRandNum(), slackTime.getRandNum(),
                    material.getRandNum(), importance.getRandNum(),
                    workers.getRandNum(), quantity.getRandNum());
        queue1.insertOrder(order);
    }

    // Check that member variables are correct
    if (queue1.getPriorityFn() != priorityFn2 ||
        queue1.m_heapType != MINHEAP ||
        queue1.m_structure != SKEW ||
        queue1.m_size != 300){
        return false;
    }

    // Check that heap property was preserved
    if (!testHeapProperty(queue1.getPriorityFn(), queue1.getHeapType(), queue1.m_heap)){
        return false;
    }
    return true;
}

bool Tester::testHeapProperty(prifn_t priFn, HEAPTYPE heapType, Node * aNode){
    bool rightChildGood = false;
    bool leftChildGood = false;
    prifn_t priority = priFn;
    // If node is null, return true
    if (aNode == nullptr){
        return true;
    }
    else{
        if (aNode->m_left != nullptr && aNode->m_right != nullptr) {
            if (heapType == MINHEAP){
                leftChildGood = priority(aNode->m_order) <= priority(aNode->m_left->m_order);
                rightChildGood = priority(aNode->m_order) <= priority(aNode->m_right->m_order);
            }
            else{
                leftChildGood = priority(aNode->m_order) >= priority(aNode->m_left->m_order);
                rightChildGood = priority(aNode->m_order) >= priority(aNode->m_right->m_order);
            }
            // If node's priority is higher than both left and right child's and same is true
            // for all their children, return true. Otherwise, return false.
            if (leftChildGood && rightChildGood &&
                testHeapProperty(priFn, heapType, aNode->m_left) &&
                testHeapProperty(priFn, heapType, aNode->m_right)){
                return true;
            }
            else{
                return false;
            }
        }
        else if (aNode->m_right != nullptr){
            if (heapType == MINHEAP){
                rightChildGood = priority(aNode->m_order) <= priority(aNode->m_right->m_order);
            }
            else{
                rightChildGood = priority(aNode->m_order) >= priority(aNode->m_right->m_order);
            }
            
            // If node's priority is higher than right child's and same is true for all right child's
            // children, return true. Otherwise, return false.
            if (rightChildGood &&
                testHeapProperty(priFn, heapType, aNode->m_left) &&
                testHeapProperty(priFn, heapType, aNode->m_right)){
                return true;
            }
            else{
                return false;
            }
        }
        else if (aNode->m_left != nullptr){
            if (heapType == MINHEAP){
                leftChildGood = priority(aNode->m_order) <= priority(aNode->m_left->m_order);
            }
            else{
                leftChildGood = priority(aNode->m_order) >= priority(aNode->m_left->m_order);
            }

            // If node's priority is higher than left child's and same is true for all left child's
            // children, return true. Otherwise, return false.
            if (leftChildGood &&
                testHeapProperty(priFn, heapType, aNode->m_left) &&
                testHeapProperty(priFn, heapType, aNode->m_right)){
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

bool Tester::testInsertNormalMax(){
    // Create random max heap of size 300
    Random nameGen(97,122);
    Random FIFO(MINONE,MAX50); Random processTime(MINONE,MAX12);
    Random dueTime(MINONE,MAX12); Random slackTime(MINONE,MAX12);
    Random material(MINZERO,MAX100); Random importance(MINONE,MAX100);
    Random workers(MAX100,MAX200); Random quantity(MIN1000,MAX10000);
    MQueue queue1(priorityFn1, MAXHEAP, SKEW);
    for (int i=0;i<300;i++){
        Order order(nameGen.getRandString(5),
                    FIFO.getRandNum(), processTime.getRandNum(),
                    dueTime.getRandNum(), slackTime.getRandNum(),
                    material.getRandNum(), importance.getRandNum(),
                    workers.getRandNum(), quantity.getRandNum());
        queue1.insertOrder(order);
    }

    if (!testHeapProperty(queue1.getPriorityFn(), queue1.getHeapType(), queue1.m_heap)){
        return false;
    }
    return true;
}

bool Tester::testRemoveNormalMin(){
    // Create random min heap of size 300
    Random nameGen(97,122);
    Random FIFO(MINONE,MAX50); Random processTime(MINONE,MAX12);
    Random dueTime(MINONE,MAX12); Random slackTime(MINONE,MAX12);
    Random material(MINZERO,MAX100); Random importance(MINONE,MAX100);
    Random workers(MAX100,MAX200); Random quantity(MIN1000,MAX10000);
    MQueue queue1(priorityFn2, MINHEAP, SKEW);
    int size = 300;
    prifn_t priority = queue1.getPriorityFn();
    for (int i=0;i<300;i++){
        Order order(nameGen.getRandString(5),
                    FIFO.getRandNum(), processTime.getRandNum(),
                    dueTime.getRandNum(), slackTime.getRandNum(),
                    material.getRandNum(), importance.getRandNum(),
                    workers.getRandNum(), quantity.getRandNum());
        queue1.insertOrder(order);
    }

    if (!testHeapProperty(queue1.getPriorityFn(), queue1.getHeapType(), queue1.m_heap)){
        return false;
    }

    // Dequeue 150 times
    for (int j = 0; j < 150; j++){
        int highestPriority = priority(queue1.m_heap->m_order);
        Order nextOrder = queue1.getNextOrder();

        // Check that function returned highest priority order
        if (highestPriority != priority(nextOrder)){
            return false;
        }

        // Check that size is one less
        if (queue1.m_size != size - j - 1){
            return false;
        }

        // Check that heap property was preserved after removal
        if (!testHeapProperty(queue1.getPriorityFn(), queue1.getHeapType(), queue1.m_heap)){
            return false;
        }
    }

    return true;
}

bool Tester::testRemoveNormalMax(){
    // Create random max heap of size 300
    Random nameGen(97,122);
    Random FIFO(MINONE,MAX50); Random processTime(MINONE,MAX12);
    Random dueTime(MINONE,MAX12); Random slackTime(MINONE,MAX12);
    Random material(MINZERO,MAX100); Random importance(MINONE,MAX100);
    Random workers(MAX100,MAX200); Random quantity(MIN1000,MAX10000);
    int size = 300;
    MQueue queue1(priorityFn1, MAXHEAP, SKEW);
    prifn_t priority = queue1.getPriorityFn();
    for (int i=0;i<300;i++){
        Order order(nameGen.getRandString(5),
                    FIFO.getRandNum(), processTime.getRandNum(),
                    dueTime.getRandNum(), slackTime.getRandNum(),
                    material.getRandNum(), importance.getRandNum(),
                    workers.getRandNum(), quantity.getRandNum());
        queue1.insertOrder(order);
    }

    if (!testHeapProperty(queue1.getPriorityFn(), queue1.getHeapType(), queue1.m_heap)){
        return false;
    }

    // Dequeue 150 times
    for (int j = 0; j < 150; j++){
        int highestPriority = priority(queue1.m_heap->m_order);
        Order nextOrder = queue1.getNextOrder();
        
        // Check that function returned highest priority node
        if (highestPriority != priority(nextOrder)){
            return false;
        }

        // Check that size is one less
        if (queue1.m_size != size - j - 1){
            return false;
        }

        // Check that heap property preserved after removal
        if (!testHeapProperty(queue1.getPriorityFn(), queue1.getHeapType(), queue1.m_heap)){
            return false;
        }
    }
    return true;
}
bool Tester::testLeftistNPL(){
    // Create random leftist min heap of size 300
    Random nameGen(97,122);
    Random FIFO(MINONE,MAX50); Random processTime(MINONE,MAX12);
    Random dueTime(MINONE,MAX12); Random slackTime(MINONE,MAX12);
    Random material(MINZERO,MAX100); Random importance(MINONE,MAX100);
    Random workers(MAX100,MAX200); Random quantity(MIN1000,MAX10000);
    MQueue queue1(priorityFn2, MINHEAP, LEFTIST);
    for (int i=0;i<300;i++){
        Order order(nameGen.getRandString(5),
                    FIFO.getRandNum(), processTime.getRandNum(),
                    dueTime.getRandNum(), slackTime.getRandNum(),
                    material.getRandNum(), importance.getRandNum(),
                    workers.getRandNum(), quantity.getRandNum());
        queue1.insertOrder(order);
    }

    // Check that all npl values are correct
    if (!testNPLValues(queue1.m_heap)){
        return false;
    }

    // Check that leftist property preserved
    if (!testLeftistProperty(queue1.m_heap)){
        cout << "shit" << endl;
        return false;
    }

    return true;
}

bool Tester::testNPLValues(Node * aNode){
    // If node is null, return true
    if (aNode == nullptr){
        return true;
    }

    // If node's npl is equal to actual npl and same is true for all children, return true
    if (aNode->m_npl == getNPL(aNode) &&
        testNPLValues(aNode->m_left) &&
        testNPLValues(aNode->m_right)){
        return true;
    }
    else{
        return false;
    }
}

int Tester::getNPL(Node * aNode){
    // If node is null, return -1
    if (aNode == nullptr){
        return -1;
    }

    // Get npl of left and right child
    int leftNPL = getNPL(aNode->m_left);
    int rightNPL = getNPL(aNode->m_right);

    // If left npl is less, return left npl plus 1
    // Otherwise, return right npl plus 1
    if (leftNPL <= rightNPL){
        return leftNPL + 1;
    }
    else{
        return rightNPL + 1;
    }
}

bool Tester::testLeftistProperty(Node * aNode){
    // If node is null, return true
    if (aNode == nullptr){
        return true;
    }

    if (aNode->m_left != nullptr && aNode->m_right != nullptr){
        // If left child npl is great than or equal to right child and same is true for all children,
        // return true. Otherwise, return false.
        if (aNode->m_left->m_npl >= aNode->m_right->m_npl &&
            testLeftistProperty(aNode->m_left) &&
            testLeftistProperty(aNode->m_right)){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        // If node had only one child, check that it's left child (left child should have greater npl)
        // If leftist property applies to all left child's children, return true, otherwise false
        if (aNode->m_right == nullptr && testLeftistProperty(aNode->m_left) &&
            testLeftistProperty(aNode->m_right)){
            return true;
        }
        else{
            return false;
        }
    }
}
bool Tester::testSetPriorityFn(){
    // Create random leftist max heap of size 100
    Random nameGen(97,122);
    Random FIFO(MINONE,MAX50); Random processTime(MINONE,MAX12);
    Random dueTime(MINONE,MAX12); Random slackTime(MINONE,MAX12);
    Random material(MINZERO,MAX100); Random importance(MINONE,MAX100);
    Random workers(MAX100,MAX200); Random quantity(MIN1000,MAX10000);
    MQueue queue1(priorityFn1, MAXHEAP, LEFTIST);
    for (int i = 0; i < 100; i++){
        Order order(nameGen.getRandString(5),
                    FIFO.getRandNum(), processTime.getRandNum(),
                    dueTime.getRandNum(), slackTime.getRandNum(),
                    material.getRandNum(), importance.getRandNum(),
                    workers.getRandNum(), quantity.getRandNum());
        queue1.insertOrder(order);
    }

    if (!testHeapProperty(queue1.getPriorityFn(), queue1.getHeapType(), queue1.m_heap)){
        return false;
    }
    
    // Put all nodes in array
    Node * arr1[queue1.m_size];
    int size1 = queue1.m_size;
    for (int k = 0; k < 100; k++){
        arr1[k] = nullptr;
    }
    createNodeArray(arr1, queue1.m_heap, queue1.m_size);

    // Change from max heap to min heap
    queue1.setPriorityFn(priorityFn2, MINHEAP);

    // Create array of rebuilt heap
    Node * arr2[queue1.m_size];
    int size2 = queue1.m_size;
    for (int p = 0; p < 100; p++){
        arr2[p] = nullptr;
    }
    createNodeArray(arr2, queue1.m_heap, queue1.m_size);

    // Check that memory was not reallocated (addresses still exist, just in different order)
    for (int x = 0; x < 100; x++){
        bool sameAddress = false;
        for (int y = 0; y < 100; y++){
            if (arr2[y] == arr1[x]){
                sameAddress = true;
            }
        }
        if (!sameAddress){
            return false;
        }
    }

    // Check that size is same
    if (size1 != size2){
        return false;
    }

    // Check that heap property still preserved
    if (!testHeapProperty(queue1.getPriorityFn(), queue1.getHeapType(), queue1.m_heap)){
        return false;
    }

    // Check that all npl values are correct
    if (!testNPLValues(queue1.m_heap)){
        return false;
    }

    // Check that leftist property preserved
    if (!testLeftistProperty(queue1.m_heap)){
        return false;
    }

    // Repeat but this time change from min heap to max heap
    MQueue queue2(priorityFn2, MINHEAP, LEFTIST);
    for (int j = 0; j < 10; j++){
        Order order(nameGen.getRandString(5),
                    FIFO.getRandNum(), processTime.getRandNum(),
                    dueTime.getRandNum(), slackTime.getRandNum(),
                    material.getRandNum(), importance.getRandNum(),
                    workers.getRandNum(), quantity.getRandNum());
        queue2.insertOrder(order);
    }

    if (!testHeapProperty(queue2.getPriorityFn(), queue2.getHeapType(), queue2.m_heap)){
        return false;
    }

    Node * arr3[queue2.m_size];
    for (int z = 0; z < 10; z++){
        arr3[z] = nullptr;
    }

    createNodeArray(arr3, queue2.m_heap, queue2.m_size);

    queue2.setPriorityFn(priorityFn1, MAXHEAP);

    Node * arr4[queue2.m_size];
    for (int s = 0; s < 10; s++){
        arr4[s] = nullptr;
    }
    createNodeArray(arr4, queue2.m_heap, queue2.m_size);

    for (int a = 0; a < 10; a++){
        bool sameAddress = false;
        for (int b = 0; b < 10; b++){
            if (arr4[b] == arr3[a]){
                sameAddress = true;
            }
        }
        if (!sameAddress){
            return false;
        }
    }

    if (!testHeapProperty(queue2.getPriorityFn(), queue2.getHeapType(), queue2.m_heap)){
        return false;
    }

    if (!testNPLValues(queue2.m_heap)){
        return false;
    }

    if (!testLeftistProperty(queue2.m_heap)){
        return false;
    }

    return true;
}

void Tester::createNodeArray(Node * arr[], Node * aNode, int size){
    if (aNode != nullptr){
        bool indexFound = false;
        for (int i = 0; i < size; i++){
            // Insert node into next available index
            if (arr[i] == nullptr && !indexFound){
                arr[i] = aNode;
                indexFound = true;
            }
        }
        createNodeArray(arr, aNode->m_left, size);
        createNodeArray(arr, aNode->m_right, size);
    }
}

bool Tester::testMergeEmptyQueue(){
    // Create empty skew max heap
    MQueue queue2(priorityFn1, MAXHEAP, SKEW);
    // Create random skew max heap of size 8
    MQueue queue1(priorityFn1, MAXHEAP, SKEW);
    Random nameGen(97,122);
    Random FIFO(MINONE,MAX50); Random processTime(MINONE,MAX12);
    Random dueTime(MINONE,MAX12); Random slackTime(MINONE,MAX12);
    Random material(MINZERO,MAX100); Random importance(MINONE,MAX100);
    Random workers(MAX100,MAX200); Random quantity(MIN1000,MAX10000);
    for (int i = 0; i < 8; i++){
        Order order(nameGen.getRandString(5),
                    FIFO.getRandNum(), processTime.getRandNum(),
                    dueTime.getRandNum(), slackTime.getRandNum(),
                    material.getRandNum(), importance.getRandNum(),
                    workers.getRandNum(), quantity.getRandNum());
        queue1.insertOrder(order);
    }

    // Create array of non empty queue
    Node * arr1[queue1.m_size];
    for (int x = 0; x < 8; x++){
        arr1[x] = nullptr;
    }
    createNodeArray(arr1, queue1.m_heap, queue1.m_size);

    // Merge non empty queue with empty queue
    queue2.mergeWithQueue(queue1);

    // Create array of merged queue
    Node * arr2[queue2.m_size];
    for (int y = 0; y < 8; y++){
        arr2[y] = nullptr;
    }
    createNodeArray(arr2, queue2.m_heap, queue2.m_size);

    // Check that memory was not reallocated (addresses still exist)
    for (int a = 0; a < 8; a++){
        bool sameAddress = false;
        for (int b = 0; b < 8; b++){
            if (arr2[b] == arr1[a]){
                sameAddress = true;
            }
        }
        if (!sameAddress){
            return false;
        }
    }

    // Check that size was updated
    if (queue2.m_size != 8){
        return false;
    }

    if (queue1.m_heap != nullptr || queue1.m_size != 0){
        return false;
    }

    return true;
}

bool Tester::testCopyConstructorNormal(){
    // Create random leftist max heap of size 10
    Random nameGen(97,122);
    Random FIFO(MINONE,MAX50); Random processTime(MINONE,MAX12);
    Random dueTime(MINONE,MAX12); Random slackTime(MINONE,MAX12);
    Random material(MINZERO,MAX100); Random importance(MINONE,MAX100);
    Random workers(MAX100,MAX200); Random quantity(MIN1000,MAX10000);
    MQueue queue1(priorityFn1, MAXHEAP, LEFTIST);
    for (int i = 0; i < 10; i++){
        Order order(nameGen.getRandString(5),
                    FIFO.getRandNum(), processTime.getRandNum(),
                    dueTime.getRandNum(), slackTime.getRandNum(),
                    material.getRandNum(), importance.getRandNum(),
                    workers.getRandNum(), quantity.getRandNum());
        queue1.insertOrder(order);
    }

    // Initialize new object using previous object
    MQueue queue2(queue1);

    // Check that new tree has same values but different addresses
    if (!checkAddresses(queue1.getPriorityFn(), queue1.m_heap, queue2.m_heap)){
        return false;
    }

    return true;
}

bool Tester::checkAddresses(prifn_t priFn, Node * copy, Node * original){
    prifn_t priority = priFn;
    // If both nodes are null, return true.
    // If one node is null and the other is not, return false.
    if (copy == nullptr && original == nullptr){
        return true;
    }
    else if ((copy == nullptr && original != nullptr) ||
            (copy != nullptr && original == nullptr)){
        return false;
    }
    else{
        // If addresses are different but priority is same and same is true for all children, return true
        // Otherwise, return false
        if (copy != original && priority(copy->m_order) == priority(original->m_order) &&
            checkAddresses(priFn, copy->m_left, original->m_left) &&
            checkAddresses(priFn, copy->m_right, original->m_right)){
            return true;
        }
        else{
            return false;
        }
    }
}

bool Tester::testCopyConstructorEdge(){
    // Create empty skew max heap
    MQueue emptyQueue(priorityFn1, MAXHEAP, SKEW);
    // Initialize new object using empty queue
    MQueue queue1(emptyQueue);

    // Check that heap is empty and priority function, heap type and structure are same
    if (queue1.m_heap != nullptr || queue1.m_size != 0 ||
        queue1.m_priorFunc != emptyQueue.m_priorFunc ||
        queue1.m_heapType != emptyQueue.m_heapType ||
        queue1.m_structure != emptyQueue.m_structure){
        return false;
    }

    return true;
}

bool Tester::testAssignmentOperatorNormal(){
    // Create random leftist max heap of size 10
    Random nameGen(97,122);
    Random FIFO(MINONE,MAX50); Random processTime(MINONE,MAX12);
    Random dueTime(MINONE,MAX12); Random slackTime(MINONE,MAX12);
    Random material(MINZERO,MAX100); Random importance(MINONE,MAX100);
    Random workers(MAX100,MAX200); Random quantity(MIN1000,MAX10000);
    MQueue queue1(priorityFn1, MAXHEAP, LEFTIST);
    MQueue queue2(priorityFn2, MINHEAP, SKEW);
    for (int i=0;i<10;i++){
        Order order(nameGen.getRandString(5),
                    FIFO.getRandNum(), processTime.getRandNum(),
                    dueTime.getRandNum(), slackTime.getRandNum(),
                    material.getRandNum(), importance.getRandNum(),
                    workers.getRandNum(), quantity.getRandNum());
        queue1.insertOrder(order);
    }

    // Create random skew min heap of size 10
    for (int i=0;i<10;i++){
        Order order(nameGen.getRandString(5),
                    FIFO.getRandNum(), processTime.getRandNum(),
                    dueTime.getRandNum(), slackTime.getRandNum(),
                    material.getRandNum(), importance.getRandNum(),
                    workers.getRandNum(), quantity.getRandNum());
        queue2.insertOrder(order);
    }

    // Set queues equal to each other
    queue2 = queue1;

    // Check that member variables are same, tree values are same, but addresses different
    if (queue2.m_heapType != queue1.m_heapType ||
        queue2.m_priorFunc != queue1.m_priorFunc ||
        queue2.m_structure != queue1.m_structure ||
        queue2.m_size != queue1.m_size ||
        !checkAddresses(queue2.getPriorityFn(), queue2.m_heap, queue1.m_heap)){
        return false;
    }
    return true;
}

bool Tester::testAssignmentOperatorEdge(){
    // Error Case 1: Set empty object equal to empty object
    MQueue emptyQueue1(priorityFn1, MAXHEAP, SKEW);
    MQueue emptyQueue2(priorityFn2, MINHEAP, LEFTIST);
    
    emptyQueue1 = emptyQueue2;

    // Should still be empty
    if (emptyQueue1.m_heap != nullptr || emptyQueue1.m_size != 0){
        return false;
    }

    // Error Case 2: Set empty object equal to non-empty object
    MQueue nonEmptyQueue(priorityFn1, MAXHEAP, SKEW);
    Order order("abc", 10, 10, 10, 10, 10, 10, 150, 5000);
    nonEmptyQueue.insertOrder(order);
    emptyQueue1 = nonEmptyQueue;

    // Object should not be empty, values should be same and addresses different
    if (emptyQueue1.m_heap == nullptr || 
        emptyQueue1.m_heapType != nonEmptyQueue.m_heapType ||
        emptyQueue1.m_priorFunc != nonEmptyQueue.m_priorFunc ||
        emptyQueue1.m_structure != nonEmptyQueue.m_structure ||
        emptyQueue1.m_size != nonEmptyQueue.m_size ||
        !checkAddresses(emptyQueue1.getPriorityFn(), emptyQueue1.m_heap, nonEmptyQueue.m_heap)){
        return false;
    }

    // Error Case 3: Set non empty object equal to empty object
    nonEmptyQueue = emptyQueue2;
    // Object should be empty
    if (nonEmptyQueue.m_heap != nullptr || nonEmptyQueue.m_size != 0){
        return false;
    }

    return true;
}
bool Tester::testDequeueEmptyQueue(){
    // Create empty skew min heap
    MQueue queue2(priorityFn2, MINHEAP, SKEW);
    
    // Try dequeueing from empty queue
    // If function throws out of range error, return true, otherwise false
    try{
        Order remove = queue2.getNextOrder();
    } catch (const std::out_of_range& e) {
        cerr << "Error: " << e.what() << endl; 
        return true;
    }
    return false;
}

bool Tester::testMergeDifferentQueues(){
    // Create two skew heaps of different types
    MQueue queue1(priorityFn1, MAXHEAP, SKEW);
    queue1.insertOrder(Order("xyz", 10, 10, 10, 10, 10, 10, 150, 5000));
    MQueue queue2(priorityFn2, MINHEAP, SKEW);
    queue2.insertOrder(Order("abc", 5, 5, 5, 5, 5, 5, 120, 2000));
    
    // Try merging queues
    // If function throws domain error, return true, otherwise false
    try{
        queue1.mergeWithQueue(queue2);
    } catch (const std::domain_error& e){
        cerr << "Error: " << e.what() << endl;
        return true;
    }
    return false;
}

/* Priority functions */
int priorityFn1(const Order & order) {
    //this function works with a MAXHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [1101-10400]
    int minValue = MIN1000 + MAX100 + MINONE + MINZERO;
    int maxValue = MAX100 + MAX100 + MAX200 + MAX10000;
    //the larger value means the higher priority
    int priority = order.getMaterial() + order.getImportance() + 
                    order.getWorkForce() + order.getQuantity();
    if (priority >= minValue && priority <= maxValue)
        return priority;
    else
        return 0; // this is an invalid order object
}

int priorityFn2(const Order & order) {
    //this function works with a MINHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [4-86]
    int minValue = MINONE + MINONE + MINONE + MINONE;
    int maxValue = MAX12 + MAX12 + MAX12 + MAX50;
    //the smaller value means the higher priority
    int priority = order.getFIFO() + order.getProcessTime() + 
                order.getDueTime() + order.getSlackTime();
    if (priority >= minValue && priority <= maxValue)
        return priority;
    else
        return 0; // this is an invalid order object
}