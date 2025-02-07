// CMSC 341 - Fall 2024 - Project 4
#include "filesys.h"
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

unsigned int hashCode(const string str) {
   unsigned int val = 0 ;
   const unsigned int thirtyThree = 33 ;  // magic number from textbook
   for (unsigned int i = 0 ; i < str.length(); i++)
      val = val * thirtyThree + str[i] ;
   return val ;
}

class Tester{
    public:
    //Name: Tester - testInsertion()
    //Precondition: None
    //Postcondition: Returns true if insert works correctly for error, colliding and non-colliding keys.
    bool testInsertion();
    //Name: Tester - testFindError()
    //Precondition: None
    //Postcondition: Returns true if getFile() returns empty file for nonexistent files.
    bool testFindError();
    //Name: Tester - testFindNonColliding()
    //Precondition: None
    //Postcondition: Returns true if getFile() finds files with a non-colliding keys.
    bool testFindNonColliding();
    //Name: Tester - testFindColliding()
    //Precondition: None
    //Postcondition: Returns true if getFile() finds files with colliding keys.
    bool testFindColliding();
    //Name: Tester - testRemoveNonColliding()
    //Precondition: None
    //Postcondition: Returns true if files with non-colliding keys are successfully removed.
    bool testRemoveNonColliding();
    //Name: Tester - testRemoveColliding()
    //Precondition: None
    //Postcondition: Returns true if files with colliding keys are successfully removed.
    bool testRemoveColliding();
    //Name: Tester - testRehashInsert()
    //Precondition: None
    //Postcondition: Returns true if rehash triggerred when load factor is greater than 50%.
    bool testRehashInsert();
    //Name: Tester - testRehashInsertCompletion()
    //Precondition: None
    //Postcondition: Returns true if at the end of the rehashing process, all live data is transferred from
    //               old table to new table.
    bool testRehashInsertCompletion();
    //Name: Tester - testRehashRemove()
    //Precondition: None
    //Postcondition: Returns true if rehash triggerred when deleted ratio is greater than 80%.
    bool testRehashRemove();
    //Name: Tester - testRehashRemoveCompletion()
    //Precondition: None
    //Postcondition: Returns true if at the end of the rehashing process, all live data is transferred from
    //               old table to new table.
    bool testRehashRemoveCompletion();
    //Name: Tester - testChangeProbPolicy()
    //Precondition: None
    //Postcondition: Returns true if new probing policy applied during rehashing process.
    bool testChangeProbPolicy();
    //Name: Tester - testUpdateDiskBlock()
    //Precondition: None
    //Postcondition: Returns true if disk blocks are successfully updated.
    bool testUpdateDiskBlock();
    private:
};

const char * FAIL_STATEMENT = "*****TEST FAILED: ";
const char * PASS_STATEMENT = "     TEST PASSED: ";

int main(){
    Tester test;
    if (test.testInsertion()){
        cout << PASS_STATEMENT << "Insert worked correctly for error, colliding and non-colliding keys." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "Insert did not work correctly for error, colliding and non-colliding keys." << endl;
    }
    
    if (test.testFindError()){
        cout << PASS_STATEMENT << "getFile() worked correctly for error cases." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "getFile() did not work correctly for error cases." << endl;
    }
    
    if (test.testFindNonColliding()){
        cout << PASS_STATEMENT << "getFile() worked correctly for non-colliding keys." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "getFile() did not work correctly for non-colliding keys." << endl;
    }
    
    if (test.testFindColliding()){
        cout << PASS_STATEMENT << "getFile() worked correctly for colliding keys." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "getFile() did not work correctly for colliding keys." << endl;
    }
    
    if (test.testRemoveNonColliding()){
        cout << PASS_STATEMENT << "remove() worked correctly for non-colliding keys." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "remove() did not work correctly for non-colliding keys." << endl;
    }
    
    if (test.testRemoveColliding()){
        cout << PASS_STATEMENT << "remove() worked correctly for colliding keys." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "remove() did not work correctly for colliding keys." << endl;
    }

    if (test.testRehashInsert()){
        cout << PASS_STATEMENT << "Rehash was correctly triggered during insertion." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "Rehash was not correctly triggered during insertion." << endl;
    }

    if (test.testRehashInsertCompletion()){
        cout << PASS_STATEMENT << "Rehash successfully transferred all data to new table." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "Rehash did not transfer all data to new table." << endl;
    }

    if (test.testRehashRemove()){
        cout << PASS_STATEMENT << "Rehash was correctly triggered during removal." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "Rehash was not triggered during removal." << endl;
    }

    if (test.testRehashRemoveCompletion()){
        cout << PASS_STATEMENT << "Rehash successfully transferred all data to new table." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "Rehash did not transfer all data to new table." << endl;
    }

    if (test.testUpdateDiskBlock()){
        cout << PASS_STATEMENT << "Disk blocks were successfully updated." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "Disk blocks were not updated." << endl;
    }

    if (test.testChangeProbPolicy()){
        cout << PASS_STATEMENT << "Probing policy was successfully updated and applied to rehash." << endl;
    }
    else{
        cout << FAIL_STATEMENT << "Probing policy was not updated or applied to rehash." << endl;
    }

    return 0;
}

bool Tester::testInsertion(){
    // NON-COLLIDING KEYS
    string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    vector<File> dataList;
    Random RndID(DISKMIN,DISKMAX);
    FileSys filesys(MINPRIME, hashCode, DOUBLEHASH);
    for (int i = 0; i < 26; i++){
        string str(1, characters[i]);
        // generating random data
        File dataObj = File(str, RndID.getRandNum(), true);
        // saving data for later use
        dataList.push_back(dataObj);
        // inserting data into the FileSys object
        if (!filesys.insert(dataObj)) cout << "Did not insert " << &dataObj << endl;

        if (!(*(filesys.m_currentTable[i + 65]) == dataObj) || filesys.m_currentSize != i + 1){
            return false;
        }
    }

    // ERROR CASES
    if (filesys.insert(dataList[0]) || filesys.insert(dataList[1])){
        return false;
    }

    // COLLIDING KEYS: Linear probing
    vector<File> dataList2;
    FileSys filesys2(MINPRIME, hashCode, LINEAR);
    for (int i=0;i<30;i++){
        // generating random data
        File dataObj = File("A", RndID.getRandNum(), true);
        // saving data for later use
        dataList2.push_back(dataObj);
        // inserting data into the FileSys object
        if (!filesys2.insert(dataObj)) cout << "Did not insert " << &dataObj << endl;

        if (!(*(filesys2.m_currentTable[i + 65]) == dataObj) || filesys2.m_currentSize != i + 1){
            return false;
        }
    }

    // COLLIDING KEYS: Quadratic probing
    vector<File> dataList3;
    FileSys filesys3(MINPRIME, hashCode, QUADRATIC);
    for (int i=0;i<30;i++){
        // generating random data
        File dataObj = File("A", RndID.getRandNum(), true);
        // saving data for later use
        dataList3.push_back(dataObj);
        // inserting data into the FileSys object
        if (!filesys3.insert(dataObj)) cout << "Did not insert " << &dataObj << endl;

        if (!(*(filesys3.m_currentTable[(65 + (i*i)) % MINPRIME]) == dataObj) || filesys3.m_currentSize != i + 1){
            return false;
        }
    }

    // COLLIDING KEYS: Double hashing
    vector<File> dataList4;
    FileSys filesys4(MINPRIME, hashCode, DOUBLEHASH);
    for (int i = 0; i < 30; i++){
        // generating random data
        File dataObj = File("B", RndID.getRandNum(), true);
        // saving data for later use
        dataList4.push_back(dataObj);
        // inserting data into the FileSys object
        if (!filesys4.insert(dataObj)) cout << "Did not insert " << &dataObj << endl;

        if (!(*(filesys4.m_currentTable[(characters[1] + i * (11-(characters[1] % 11))) % MINPRIME]) == dataObj) ||
            filesys4.m_currentSize != i + 1){
            return false;
        }
    }

    FileSys filesys5(MINPRIME, hashCode, DOUBLEHASH);
    filesys5.insert(File("A", DISKMIN, true));
    if (filesys5.insert(File("A", DISKMIN, true)) || filesys5.m_currentSize == 2){
        return false;
    }
    return true;
}


bool Tester::testFindError(){
    FileSys filesys(MINPRIME, hashCode, DOUBLEHASH);
    filesys.insert(File("A", DISKMIN, true));
    filesys.insert(File("B", DISKMAX, true));
    // Check that getFile() returns false when file does not exist in table
    if (!(filesys.getFile("A", DISKMAX) == File()) || !(filesys.getFile("B", DISKMIN) == File())){
        return false;
    }
    return true;
}

bool Tester::testFindNonColliding(){
    string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    vector<File> dataList;
    Random RndID(DISKMIN,DISKMAX);
    FileSys filesys(MINPRIME, hashCode, DOUBLEHASH);

    for (int i = 0; i < 26; i++){
        string str(1, characters[i]);
        // generating random data
        File dataObj = File(str, RndID.getRandNum(), true);
        // saving data for later use
        dataList.push_back(dataObj);
        // inserting data into the FileSys object
        if (!filesys.insert(dataObj)) cout << "Did not insert " << &dataObj << endl;
    }

    for (int j = 0; j < 26; j++){
        // Check that getFile() is able to find files with non-colliding keys
        if (!(filesys.getFile(dataList[j].m_name, dataList[j].m_diskBlock) == dataList[j])){
            return false;
        }
    }
    return true;
}

bool Tester::testFindColliding(){
    // TEST 1: Linear probing
    vector<File> dataList;
    FileSys filesys(MINPRIME, hashCode, LINEAR);
    for (int i = 0; i < 49; i++){
        // generating random data
        File dataObj = File("B", DISKMIN + i, true);
        // saving data for later use
        dataList.push_back(dataObj);
        // inserting data into the FileSys object
        if (!filesys.insert(dataObj)) cout << "Did not insert " << &dataObj << endl;
    }

    for (int j = 0; j < 49; j++){
        // Check that getFile() is able to find files with colliding keys using linear probing
        if (!(filesys.getFile(dataList[j].m_name, dataList[j].m_diskBlock) == dataList[j])){
            return false;
        }
    }

    // TEST 2: Quadratic probing
    vector<File> dataList2;
    FileSys filesys2(MINPRIME, hashCode, QUADRATIC);
    for (int i = 0; i < 49; i++){
        // generating random data
        File dataObj = File("A", DISKMIN + i, true);
        // saving data for later use
        dataList2.push_back(dataObj);
        // inserting data into the FileSys object
        if (!filesys2.insert(dataObj)) cout << "Did not insert " << &dataObj << endl;
    }
    for (int j = 0; j < 49; j++){
        // Check that getFile() is able to find files with colliding keys using quadratic probing
        if (!(filesys2.getFile(dataList2[j].m_name, dataList2[j].m_diskBlock) == dataList2[j])){
            return false;
        }
    }

    // TEST 3: Double hashing
    vector<File> dataList3;
    FileSys filesys3(MINPRIME, hashCode, DOUBLEHASH);
    for (int i = 0; i < 49; i++){
        // generating random data
        File dataObj = File("A", DISKMIN + i, true);
        // saving data for later use
        dataList3.push_back(dataObj);
        // inserting data into the FileSys object
        if (!filesys3.insert(dataObj)) cout << "Did not insert " << &dataObj << endl;
    }
    for (int j = 0; j < 49; j++){
        // Check that getFile() is able to find files with colliding keys using double hashing
        if (!(filesys3.getFile(dataList3[j].m_name, dataList3[j].m_diskBlock) == dataList3[j])){
            return false;
        }
    }
    return true;
}

bool Tester::testRemoveNonColliding(){
    string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    vector<File> dataList;
    Random RndID(DISKMIN,DISKMAX);
    FileSys filesys(MINPRIME, hashCode, DOUBLEHASH);
    for (int i = 0; i < 26; i++){
        string str(1, characters[i]);
        // generating random data
        File dataObj = File(str, RndID.getRandNum(), true);
        // saving data for later use
        dataList.push_back(dataObj);
        // inserting data into the FileSys object
        if (!filesys.insert(dataObj)) cout << "Did not insert " << &dataObj << endl;
    }
    int deleted = 0;

    // Remove less than 80% of files
    // Check that after deleted they're not being used and number of deleted files increases
    for (int j = 0; j < 20; j++){
        if (!filesys.remove(dataList[j]) || !(filesys.getFile(dataList[j].m_name, dataList[j].m_diskBlock) == File()) ||
            filesys.m_currentTable[j + 65]->m_used == true ||
            filesys.m_currNumDeleted != deleted + 1){
            return false;
        }
        deleted++;
    }
    return true;
}

bool Tester::testRemoveColliding(){
    // TEST 1: Linear probing
    vector<File> dataList;
    FileSys filesys(MINPRIME, hashCode, LINEAR);
    for (int i = 0; i < 49; i++){
        // generating random data
        File dataObj = File("A", DISKMIN + i, true);
        // saving data for later use
        dataList.push_back(dataObj);
        // inserting data into the FileSys object
        if (!filesys.insert(dataObj)) cout << "Did not insert " << &dataObj << endl;
    }
    int deleted = 0;

    // Remove less than 80% of files
    for (int j = 0; j < 30; j++){
        // Check that after deleted file is not being used and number of deleted files increases
        if (!filesys.remove(dataList[j]) || !(filesys.getFile(dataList[j].m_name, dataList[j].m_diskBlock) == File()) ||
            filesys.m_currentTable[(j + 65) % MINPRIME]->m_used == true ||
            filesys.m_currNumDeleted != deleted + 1){
            return false;
        }
        deleted++;
    }

    // TEST 2: Quadratic probing
    vector<File> dataList2;
    FileSys filesys2(MINPRIME, hashCode, QUADRATIC);
    for (int i = 0; i < 49; i++){
        // generating random data
        File dataObj = File("A", DISKMIN + i, true);
        // saving data for later use
        dataList2.push_back(dataObj);
        // inserting data into the FileSys object
        if (!filesys2.insert(dataObj)) cout << "Did not insert " << &dataObj << endl;
    }
    deleted = 0;
    for (int j = 0; j < 30; j++){
        if (!filesys2.remove(dataList2[j]) || !(filesys2.getFile(dataList2[j].m_name, dataList2[j].m_diskBlock) == File()) ||
            filesys2.m_currentTable[(65 + (j*j)) % MINPRIME]->m_used == true ||
            filesys2.m_currNumDeleted != deleted + 1){
            return false;
        }
        deleted++;
    }

    // TEST 3: Double hashing
    vector<File> dataList3;
    FileSys filesys3(MINPRIME, hashCode, DOUBLEHASH);
    for (int i = 0; i < 49; i++){
        // generating random data
        File dataObj = File("B", DISKMIN + i, true);
        // saving data for later use
        dataList3.push_back(dataObj);
        // inserting data into the FileSys object
        if (!filesys3.insert(dataObj)) cout << "Did not insert " << &dataObj << endl;
    }
    deleted = 0;
    for (int j = 0; j < 30; j++){
        if (!filesys3.remove(dataList3[j]) || !(filesys3.getFile(dataList3[j].m_name, dataList3[j].m_diskBlock) == File()) ||
            filesys3.m_currentTable[(66 + j * (11-(66 % 11))) % MINPRIME]->m_used == true ||
            filesys3.m_currNumDeleted != deleted + 1){
            return false;
        }
        deleted++;
    }
    return true;
}

bool Tester::testRehashInsert(){
    string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    vector<File> dataList;
    Random RndID(DISKMIN,DISKMAX);
    Random RndChar(0, 25);
    FileSys filesys(MINPRIME, hashCode, LINEAR);
    bool result = true;

    for (int i = 0; i < 50; i++){
        string str(1, characters[i]);
        // generating random data
        File dataObj = File(str, RndID.getRandNum(), true);
        // saving data for later use
        dataList.push_back(dataObj);
        // inserting data into the FileSys object
        if (!filesys.insert(dataObj)) cout << "Did not insert " << &dataObj << endl;
    }
    for (vector<File>::iterator it = dataList.begin(); it != dataList.end(); it++){
        result = result && (*it == filesys.getFile((*it).getName(), (*it).getDiskBlock()));
    }

    // Edge case: Check that when load factor is less than 50% rehashing does not occur
    if (!result || filesys.m_currentCap != MINPRIME || filesys.m_currentSize != 50 ||
        filesys.m_oldTable != nullptr || filesys.m_transferIndex != 0){
        return false;
    }

    // Insert one more file, bringing load factor to over 50%
    File dataObj = File("y", RndID.getRandNum(), true);
    filesys.insert(dataObj);
    dataList.push_back(dataObj);

    for (vector<File>::iterator it = dataList.begin(); it != dataList.end(); it++){
        result = result && (*it == filesys.getFile((*it).getName(), (*it).getDiskBlock()));
    }

    // Check that rehash was triggered and all files still exist in old table
    if (!result || filesys.m_currentCap != 211 || filesys.m_currentSize != 21 ||
        filesys.m_oldTable == nullptr || filesys.m_transferIndex == 0){
        return false;
    }
    return true;
}

bool Tester::testRehashInsertCompletion(){
    string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    vector<File> dataList;
    Random RndID(DISKMIN,DISKMAX);
    Random RndChar(0, 25);
    FileSys filesys(MINPRIME, hashCode, LINEAR);
    bool result = true;

    for (int i = 0; i < 55; i++){
        string str(1, characters[i]);
        // generating random data
        File dataObj = File(str, RndID.getRandNum(), true);
        // saving data for later use
        dataList.push_back(dataObj);
        // inserting data into the FileSys object
        if (!filesys.insert(dataObj)) cout << "Did not insert " << &dataObj << endl;
    }

    for (vector<File>::iterator it = dataList.begin(); it != dataList.end(); it++){
        result = result && (*it == filesys.getFile((*it).getName(), (*it).getDiskBlock()));
    }

    // Check that after rehashing 5 times all files are transferred and old table is deleted
    if (!result || filesys.m_currentCap != 211 || filesys.m_currentSize != 55 ||
        filesys.m_oldTable != nullptr || filesys.m_transferIndex != 0){
        return false;
    }

    return true;
}

bool Tester::testRehashRemove(){
    string characters = "abcdefghijklmnopqrstuvwxyz";
    vector<File> dataList;
    Random RndID(DISKMIN,DISKMAX);
    Random RndChar(0, 25);
    FileSys filesys(MINPRIME, hashCode, LINEAR);

    for (int i = 0; i < 10; i++){
        string str(1, characters[i]);
        // generating random data
        File dataObj = File(str, RndID.getRandNum(), true);
        // saving data for later use
        dataList.push_back(dataObj);
        // inserting data into the FileSys object
        if (!filesys.insert(dataObj)) cout << "Did not insert " << &dataObj << endl;
    }

    // Edge case: Check that when deleted ratio is less than 80% rehashing does not occur
    for (int x = 0; x < 7; x++){
        // Check that after deleted file is not being used and number of deleted files increases
        if (!filesys.remove(dataList[x]) || filesys.m_currentTable[(x + 97) % MINPRIME]->getUsed() != false ||
            filesys.m_currNumDeleted != x + 1 || filesys.m_oldTable != nullptr || 
            filesys.m_transferIndex != 0) {
            return false;
        }
    }
    // Remove another file, bringing deleted ratio over 80%
    filesys.remove(dataList[7]);

    // Check that rehashing was triggered
    if (filesys.m_currentCap != MINPRIME || filesys.m_currentSize != 2 ||
        filesys.m_oldTable == nullptr || filesys.m_transferIndex == 0){
        return false;
    }
    return true;
}

bool Tester::testRehashRemoveCompletion(){
    string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    vector<File> dataList;
    Random RndID(DISKMIN,DISKMAX);
    Random RndChar(0, 25);
    FileSys filesys(211, hashCode, LINEAR);

    for (int i = 0; i < 62; i++){
        string str(1, characters[i]);
        // generating random data
        File dataObj = File(str, RndID.getRandNum(), true);
        // saving data for later use
        dataList.push_back(dataObj);
        // inserting data into the FileSys object
        if (!filesys.insert(dataObj)) cout << "Did not insert " << &dataObj << endl;
    }

    // Remove 26 files with uppercase keys
    for (int x=0;x<26;x++){
        if (!filesys.remove(dataList[x]) || filesys.m_currentTable[(x + 65) % 211]->getUsed() != false) {
            return false;
        }
    }

    // Remove 23 files with lowercase keys
    for (int y=0;y<23;y++){
        if (!filesys.remove(dataList[y + 26]) || filesys.m_currentTable[(y + 97) % 211]->getUsed() != false) {
            return false;
        }
    }

    // Remove 5 files with number keys (should trigger rehashing)
    for (int z = 0; z < 5; z++){
        if (!filesys.remove(dataList[z + 49])){
            return false;
        }
    }

    // Check that after rehashing 5 times all files are transferred and old table is deleted
    if (filesys.m_currentCap != MINPRIME || filesys.m_currentSize != 10 ||
        filesys.m_oldTable != nullptr || filesys.m_transferIndex != 0){
        return false;
    }
    return true;
}

bool Tester::testChangeProbPolicy(){
    string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    vector<File> dataList;
    Random RndID(DISKMIN,DISKMAX);
    FileSys filesys(MINPRIME, hashCode, LINEAR);

    for (int i = 0; i < 50; i++){
        string str(1, characters[i]);
        // generating random data
        File dataObj = File(str, RndID.getRandNum(), true);
        // saving data for later use
        dataList.push_back(dataObj);
        // inserting data into the FileSys object
        if (!filesys.insert(dataObj)) cout << "Did not insert " << &dataObj << endl;
    }

    // Change probing policy from linear to quadratic
    filesys.changeProbPolicy(QUADRATIC);

    // Insert 2 more files, triggering rehashing 
    File file("w", 490612, true);
    File file2("w", 490613, true);
    filesys.insert(file);
    filesys.insert(file2);

    // Check that when transferred from old table to new table, new probing policy was used
    if (!(*filesys.m_currentTable[123] == file) || !(*filesys.m_currentTable[128] == file2)){
        return false;
    }
    return true;
}

bool Tester::testUpdateDiskBlock(){
    string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    vector<File> dataList;
    Random RndID(DISKMIN,DISKMAX);
    FileSys filesys(MINPRIME, hashCode, DOUBLEHASH);

    // Insert 26 files
    for (int i = 0; i < 26; i++){
        string str(1, characters[i]);
        // generating random data
        File dataObj = File(str, RndID.getRandNum(), true);
        // saving data for later use
        dataList.push_back(dataObj);
        // inserting data into the FileSys object
        if (!filesys.insert(dataObj)) cout << "Did not insert " << &dataObj << endl;
    }

    // Update disk block of all files  
    for (int j = 0; j < 26; j++){
        filesys.updateDiskBlock(dataList[j], DISKMIN);
        // Check that disk block was updated
        if (filesys.m_currentTable[j + 65]->getDiskBlock() != DISKMIN){
            return false;
        }
    }
    return true;
}