// UMBC - CMSC 341 - Fall 2024 - Proj2
#ifndef SWARM_H
#define SWARM_H
#include <iostream>
using namespace std;
class Grader;//this class is for grading purposes, no need to do anything
class Tester;//this is your tester class, you add your test functions in this class
class Swarm;
enum STATE {ALIVE, DEAD};   // possible states for a robot
enum ROBOTTYPE {BIRD, DRONE, REPTILE, SUB, QUADRUPED};
enum TREETYPE {NONE, BST, AVL, SPLAY};
const int MINID = 10000;    // min robot ID
const int MAXID = 99999;    // max robot ID
#define DEFAULT_HEIGHT 0
#define DEFAULT_ID 0
#define DEFAULT_TYPE DRONE
#define DEFAULT_STATE ALIVE

class Robot{
    public:
    friend class Swarm;
    friend class Grader;
    friend class Tester;
    Robot(int id, ROBOTTYPE type = DEFAULT_TYPE, STATE state = DEFAULT_STATE)
        :m_id(id),m_type(type), m_state(state) {
            m_left = nullptr;
            m_right = nullptr;
            m_height = DEFAULT_HEIGHT;
        }
    Robot(){
        m_id = DEFAULT_ID;
        m_type = DEFAULT_TYPE;
        m_state = DEFAULT_STATE;
        m_left = nullptr;
        m_right = nullptr;
        m_height = DEFAULT_HEIGHT;
    }
    int getID() const {return m_id;}
    STATE getState() const {return m_state;}
    string getStateStr() const {
        string text = "";
        switch (m_state)
        {
        case DEAD:text = "DEAD";break;
        case ALIVE:text = "ALIVE";break;
        default:text = "UNKNOWN";break;
        }
        return text;
    }
    ROBOTTYPE getType() const {return m_type;}
    string getTypeStr() const {
        string text = "";
        switch (m_type)
        {
        case BIRD:text = "BIRD";break;
        case DRONE:text = "DRONE";break;
        case REPTILE:text = "REPTILE";break;
        case SUB:text = "SUB";break;
        case QUADRUPED:text = "QUADRUPED";break;
        default:text = "UNKNOWN";break;
        }
        return text
        ;
    }
    int getHeight() const {return m_height;}
    Robot* getLeft() const {return m_left;}
    Robot* getRight() const {return m_right;}
    void setID(const int id){m_id=id;}
    void setState(STATE state){m_state=state;}
    void setType(ROBOTTYPE type){m_type=type;}
    void setHeight(int height){m_height=height;}
    void setLeft(Robot* left){m_left=left;}
    void setRight(Robot* right){m_right=right;}
    private:
    int m_id;
    ROBOTTYPE m_type;
    STATE m_state;
    Robot* m_left;  //the pointer to the left child in the BST
    Robot* m_right; //the pointer to the right child in the BST
    int m_height;   //the height of this node in the BST
};
class Swarm{
    public:
    friend class Grader;
    friend class Tester;
    //Name: Swarm - Default Constructor
    //Precondition: None
    //Postcondition: Creates an empty Swarm object with default values
    Swarm();
    //Name: Swarm - Alternate Constructor
    //Precondition: None
    //Postcondition: Creates a default Swarm object of passed type
    Swarm(TREETYPE type);
    //Name: Swarm - Destructor
    //Precondition: None
    //Postcondition: Deallocates memory from all nodes in tree and sets member variables to default values
    ~Swarm();
    //Name: Swarm - Overloaded Assignment Operator
    //Precondition: None
    //Postcondition: Creates a deep copy of passed Swarm object and sets current object equal to it
    const Swarm & operator=(const Swarm & rhs);
    //Name: Swarm - clear()
    //Precondition: None
    //Postcondition: Deallocates memory from all nodes in tree
    void clear();
    //Name: Swarm - getType()
    //Precondition: None
    //Postcondition: Returns tree type
    TREETYPE getType() const;
    //Name: Swarm - setType()
    //Precondition: None
    //Postcondition: Sets tree type to passed type. If type is changed to AVL, should reconstruct tree 
    //               as AVL tree.
    void setType(TREETYPE type);
    //Name: Swarm - insert()
    //Precondition: None
    //Postcondition: If id is valid and unique, should allocate memory and initialize new node, add it 
    //               to correct spot on tree, update heights and rebalance if necessary.
    void insert(const Robot& robot);
    //Name: Swarm - remove()
    //Precondition: Tree is not empty and node with passed id exists in tree
    //Postcondition: Locates and removes correct node on tree, updates heights and rebalances if necessary.
    //               If splay tree, does nothing.
    void remove(int id);
    //Name: Swarm - dumpTree()
    //Precondition: None
    //Postcondition: Prints tree
    void dumpTree() const;
    private:
    Robot* m_root;  // the root of the BST
    TREETYPE m_type;// the type of tree

    // ***************************************************
    // Any private helper functions must be delared here!
    // ***************************************************

    //Name: Swarm - checkDuplicate()
    //Precondition: None
    //Postcondition: Returns true if passed id exists in tree, otherwise returns false.
    bool checkDuplicate(int id, Robot * aBot);
    //Name: Swarm - updateHeights()
    //Precondition: None
    //Postcondition: Recalculates heights of all nodes in tree
    void updateHeights();
    //Name: Swarm - findHeight()
    //Precondition: None
    //Postcondition: Calculates and returns height of passed node
    int findHeight(Robot * aBot);
    //Name: Swarm - reBalanceTree()
    //Precondition: None
    //Postcondition: Checks that each node in tree is balanced, if imbalanced performs proper rotations
    void reBalanceTree(Robot * aBot);
    //Name: Swarm - leftRotation()
    //Precondition: None
    //Postcondition: Performs left rotation about passed node
    Robot * leftRotation(Robot * aBot);
    //Name: Swarm - rightRotation()
    //Precondition: None
    //Postcondition: Performs right rotation about passed node
    Robot * rightRotation(Robot * aBot);
    //Name: Swarm - getHeight()
    //Precondition: None
    //Postcondition: If passed node exists, returns m_height. Otherwise, returns -1.
    int getHeight(Robot * aBot);
    //Name: Swarm - remove()
    //Precondition: None
    //Postcondition: Returns tree type
    Robot * remove(int id, Robot * aBot);//helped for recursive traversal
    //Name: Swarm - findMin()
    //Precondition: None
    //Postcondition: Returns smallest node in passed node's subtree
    Robot * findMin(Robot * aBot);
    //Name: Swarm - splay()
    //Precondition: Type of tree is splay tree
    //Postcondition: Node with passed id is brought to m_root
    Robot * splay(Robot * aBot, int id);
    //Name: Swarm - testBalance()
    //Precondition: None
    //Postcondition: Returns true is tree is balanced, otherwise returns false
    bool testBalance(Robot * aBot);

    void checkBalance(Robot * aBot);//helper for recursive traversal for reBalanceTree()
    Robot * insert1(int id, ROBOTTYPE type, STATE state, Robot * robot);//helper for recursive traversal for insert()
    Robot * clear(Robot * aBot);//helper for recursive traversal for clear()
    void preOrderCopy(Robot * aBot);//helper for pre-order traversal for assignment operator
    void dump(Robot* aBot) const;//helper for recursive traversal for dumpTree()
};
#endif