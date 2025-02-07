// UMBC - CMSC 341 - Fall 2024 - Proj2
#include "swarm.h"
Swarm::Swarm(){
    m_root = nullptr;
    m_type = NONE;
}

Swarm::Swarm(TREETYPE type){
    m_root = nullptr;
    m_type = type;
}

Swarm::~Swarm(){
    clear();
}

void Swarm::clear(){
    m_root = clear(m_root);
    m_type = NONE;
}

Robot * Swarm::clear(Robot * aBot){
    // Check that memory exists
    if (aBot != nullptr){
        // Clear node's left and right child before clearing node
        aBot->m_left = clear(aBot->m_left);
        aBot->m_right = clear(aBot->m_right);
        delete aBot;
        aBot = nullptr;
    }
    return aBot;
}

void Swarm::insert(const Robot& robot){
    // Check that ID is valid and doesn't already exist
    if (robot.getID() >= MINID && robot.getID() <= MAXID && checkDuplicate(robot.getID(), m_root)){
        // Insert node and update heights
        m_root = insert1(robot.getID(), robot.getType(), robot.getState(), m_root);
        updateHeights();

        // If AVL tree, check for imbalance and if needed rebalance
        if (m_type == AVL){ 
            reBalanceTree(m_root);
        }
        // If splay tree, splay inserted node
        else if (m_type == SPLAY){
            splay(m_root, robot.getID());
            updateHeights();
        }
    }
}

bool Swarm::checkDuplicate(int id, Robot * aBot){
    // If node is null, return true
    if (aBot == nullptr){
        return true;
    }
    else{
        // If node's id doesn't match and none of its children's match, return true
        // Otherwise, return false
        if (aBot->m_id != id && checkDuplicate(id, aBot->m_left) &&
            checkDuplicate(id, aBot->m_right)){
            return true;
        }
        else{
            return false;
        }
    }
}

Robot * Swarm::insert1(int id, ROBOTTYPE type, STATE state, Robot * robot){
    // If new id is less than current node's, go left
    // If greater, go right
    // If reached nullptr, allocate memory and initialize object
    if (robot == nullptr){
        robot = new Robot(id, type, state);
    }
    else if (id < robot->m_id){
        robot->m_left = insert1(id, type, state, robot->m_left);
    }
    else if (id > robot->m_id){
        robot->m_right = insert1(id, type, state, robot->m_right);
    }
    return robot;
}

void Swarm::remove(int id){
    // Check that tree is not splay
    if (m_type != SPLAY){
        updateHeights();
        m_root = remove(id, m_root);
        updateHeights();

        // If AVL tree, check for imbalance, if needed rebalance
        if (m_type == AVL){
            reBalanceTree(m_root);
        }
    }
}

Robot * Swarm::remove(int id, Robot * aBot){
    // Traverse tree until you find node to remove
    // If id is not in tree, do nothing
    if (aBot == nullptr){
        return aBot;
    }
    else if (id < aBot->m_id){
        aBot->m_left = remove(id, aBot->m_left);
    }
    else if (id > aBot->m_id){
        aBot->m_right = remove(id, aBot->m_right);
    }
    // If node to remove has 2 children
    else if (aBot->m_left != nullptr && aBot->m_right != nullptr){
        // Find min of node's right child
        Robot * temp = findMin(aBot->m_right);
        // Replace node's value with min's
        aBot->m_id = temp->m_id;
        // Delete min
        aBot->m_right = remove(temp->m_id, aBot->m_right);
    }
    // If node to remove has 0 or 1 child
    else {
        Robot * temp = aBot;
        // Replace node with its child
        if (aBot->m_left == nullptr){
            aBot = aBot->m_right;
        }
        else{
            aBot = aBot->m_left;
        }
        // Delete node
        delete temp;
        temp = nullptr;
    }

    return aBot;
}

Robot * Swarm::findMin(Robot * aBot){
    if (aBot == nullptr || aBot->m_left == nullptr){
        return aBot;
    }
    else {
        return findMin(aBot->m_left);
    }
}

const Swarm & Swarm::operator=(const Swarm & rhs){
    // Check that object is not being assigned to itself
    if (this != &rhs){
        // Deallocate existing memory
        clear();
        // Allocate new memory and copy values
        preOrderCopy(rhs.m_root);
        m_type = rhs.m_type;
        updateHeights();
    }
    return *this;
}

void Swarm::preOrderCopy(Robot * aBot){
    if (aBot != nullptr){
        // Copy node and then copy node's left and right child
        Robot robot(aBot->getID(), aBot->getType(), aBot->getState());
        insert(robot);
        preOrderCopy(aBot->m_left);
        preOrderCopy(aBot->m_right);
    }
}

TREETYPE Swarm::getType() const{
    return m_type;
}

void Swarm::setType(TREETYPE type){
    m_type = type;
    // If type is AVL, check for imbalance, if needed rebalance
    if (type == AVL){
        while (!testBalance(m_root)){
            reBalanceTree(m_root);
            updateHeights();
        }
    }
    else if (type == NONE){
        clear();
    }
}

void Swarm::dumpTree() const 
{
    dump(m_root);
}

void Swarm::dump(Robot* aBot) const{
    if (aBot != nullptr){
        cout << "(";
        dump(aBot->m_left);//first visit the left child
        cout << aBot->m_id << ":" << aBot->m_height;//second visit the node itself
        dump(aBot->m_right);//third visit the right child
        cout << ")";
    }
}

void Swarm::updateHeights(){
    if (m_root != nullptr){
        m_root->m_height = findHeight(m_root);
    }
}

int Swarm::findHeight(Robot *aBot){
    // If node is null, return -1
    if (aBot == nullptr){
        return -1;
    }

    // Get height of left child and right child
    int leftHeight = findHeight(aBot->m_left);
    int rightHeight = findHeight(aBot->m_right);

    // Set node's height to whichever child's height is greater plus 1
    if (leftHeight > rightHeight || leftHeight == rightHeight){
        aBot->m_height = leftHeight + 1;
    }
    else if (leftHeight < rightHeight){
        aBot->m_height = rightHeight + 1;
    }

    return aBot->m_height;
}

void Swarm::reBalanceTree(Robot * aBot){
    // Check that node exists
    if (aBot != nullptr){
        // Check that node is balanced
        checkBalance(aBot);
        // Check that node's left child and right child are balanced
        if (aBot->m_left != nullptr){
            reBalanceTree(aBot->m_left);
        }
        if (aBot->m_right != nullptr){
            reBalanceTree(aBot->m_right);
        }
    }
}

int Swarm::getHeight(Robot * aBot){
    if (aBot == nullptr){
        return -1;
    }
    else{
        return aBot->m_height;
    }
}

void Swarm::checkBalance(Robot * aBot){
    // Check that node has at least 1 child
    if (aBot->m_left != nullptr || aBot->m_right != nullptr){
        // Get heights of node's children and grandchildren 
        int LCHeight = getHeight(aBot->m_left);
        int RCHeight = getHeight(aBot->m_right);
        int LCLCHeight = -1;
        int LCRCHeight = -1;
        int RCLCHeight = -1;
        int RCRCHeight = -1;

        if (LCHeight != -1){
            LCLCHeight = getHeight(aBot->m_left->m_left);
            LCRCHeight = getHeight(aBot->m_left->m_right);
        }
        if (RCHeight != -1){
            RCLCHeight = getHeight(aBot->m_right->m_left);
            RCRCHeight = getHeight(aBot->m_right->m_right);
        }

        // If node is imbalanced
        if (LCHeight - RCHeight != 1 &&
            LCHeight - RCHeight != -1 &&
            LCHeight - RCHeight != 0){
            // Node’s right child has greater height and 
            // right child of right child has a greater height
            if (RCHeight > LCHeight &&
                RCRCHeight > RCLCHeight){
                // Perform left rotation about node
                aBot = leftRotation(aBot);
            }
            // Node’s right child has greater height and 
            // left child of right child has a greater height
            else if (RCHeight > LCHeight &&
                    RCRCHeight < RCLCHeight){
                // Perform right rotation about node's right child,
                // left rotation about node
                aBot->m_right = rightRotation(aBot->m_right); 
                aBot = leftRotation(aBot);
            }
            // Node’s left child has greater height and 
            // left child of left child has a greater height
            else if (RCHeight < LCHeight &&
                    LCRCHeight < LCLCHeight){
                // Perform right rotation about node
                aBot = rightRotation(aBot);
            }
            // Node’s left child has greater height and 
            // right child of left child has a greater height
            else if (RCHeight < LCHeight &&
                    LCRCHeight > LCLCHeight){
                // Perform left rotation about node's left child,
                // right rotation about node
                aBot->m_left = leftRotation(aBot->m_left);
                aBot = rightRotation(aBot);
            }
        }
        // If node is balanced, but node's left child is imbalanced
        else if (LCLCHeight - LCRCHeight != 1 &&
                LCLCHeight - LCRCHeight != -1 &&
                LCLCHeight - LCRCHeight != 0){
            // Node's left child's left child has greater height
            // than left child's right child
            if (LCLCHeight > LCRCHeight){
                // Perform right rotation about node's left child
                aBot->m_left = rightRotation(aBot->m_left);
            }
            // Node's left child's right child has greater height
            // than left child's left child
            else {
                // Perform left rotation about node's left child
                aBot->m_left = leftRotation(aBot->m_left);
            }
        }
        // If node is balanced, but node's right child is imbalanced
        else if (RCLCHeight - RCRCHeight != 1 &&
                RCLCHeight - RCRCHeight != -1 &&
                RCLCHeight - RCRCHeight != 0){
            // Node's right child's left child has greater height
            // than right child's right child
            if (RCLCHeight > RCRCHeight){
                // Perform right rotation about node's right child
                aBot->m_right = rightRotation(aBot->m_right);
            }
            // Node's right child's right child has greater height
            // than right child's left child
            else {
                // Perform left rotation about node's right child
                aBot->m_right = leftRotation(aBot->m_right);
            }
        }
        updateHeights();
    }
}

Robot * Swarm::leftRotation(Robot * aBot){
    Robot * a = aBot->m_left;
    Robot * b = aBot->m_right->m_left;
    Robot * c = aBot->m_right->m_right;

    // Copy node's values
    int id = aBot->m_id;
    ROBOTTYPE type = aBot->m_type;
    STATE state = aBot->m_state; 

    // Replace node's values with right child's values
    aBot->m_id = aBot->m_right->m_id;
    aBot->m_type = aBot->m_right->m_type;
    aBot->m_state = aBot->m_right->m_state;

    // Replace right child's values with node's values
    aBot->m_right->m_id = id;
    aBot->m_right->m_type = type;
    aBot->m_right->m_state = state;

    // Adjust relationships to preserve BST property
    aBot->m_left = aBot->m_right;
    aBot->m_left->m_left = a;
    aBot->m_left->m_right = b;
    aBot->m_right = c;

    return aBot;
}

Robot * Swarm::rightRotation(Robot * aBot){
    Robot * a = aBot->m_left->m_left;
    Robot * b = aBot->m_left->m_right;
    Robot * c = aBot->m_right;

    // Copy node's values
    int id = aBot->m_id;
    ROBOTTYPE type = aBot->m_type;
    STATE state = aBot->m_state; 
    
    // Replace node's values with left child's values
    aBot->m_id = aBot->m_left->m_id;
    aBot->m_type = aBot->m_left->m_type;
    aBot->m_state = aBot->m_left->m_state;

    // Replace left child's values with node's values
    aBot->m_left->m_id = id;
    aBot->m_left->m_type = type;
    aBot->m_left->m_state = state;

    // Adjust relationships to preserve BST property
    aBot->m_right = aBot->m_left;
    aBot->m_left = a;
    aBot->m_right->m_left = b;
    aBot->m_right->m_right = c;

    return aBot;
}

Robot * Swarm::splay(Robot * aBot, int id){
    // If node is null or has id, return node
    if (aBot == nullptr || aBot->m_id == id){
        return aBot;
    }
    // If node's id is greater than id, move left
    if (aBot->m_id > id){
        // If node's left child is null, return node
        if (aBot->m_left == nullptr){
            return aBot;
        }
        // If node's left child's id is greater than id, move left (Case 1)
        if (aBot->m_left->m_id > id){
            aBot->m_left->m_left = splay(aBot->m_left->m_left, id);
            if (aBot->m_left->m_left != nullptr){
                // Perform right rotation on grandparent
                aBot->m_left = rightRotation(aBot->m_left);
            }
        }
        // Otherwise, if less, move right (Case 2)
        else if (aBot->m_left->m_id < id){
            aBot->m_left->m_right = splay(aBot->m_left->m_right, id);
            if (aBot->m_left->m_right != nullptr){
                // Perform left rotation on parent
                aBot->m_left = leftRotation(aBot->m_left);
            }
        }
        // If node's left child is null, return node
        if (aBot->m_left == nullptr){
            return aBot;
        }
        else{
            // Perform right rotation on parent (for Case 1)
            // or right rotation on grandparent (for Case 2)
            aBot = rightRotation(aBot);
            return aBot;
        }
    }
    // If node's id is less than id, move right
    else if (aBot->m_id < id){
        // If node's right child is null, return node
        if (aBot->m_right == nullptr){
            return aBot;
        }
        // If node's right child's id is greater than id, move left (Case 3)
        if (aBot->m_right->m_id > id){
            aBot->m_right->m_left = splay(aBot->m_right->m_left, id);
            if (aBot->m_right->m_left != nullptr){
                // Perform right rotation on parent
                aBot->m_right = rightRotation(aBot->m_right);
            }
        }
        // Otherwise, if less, move right (Case 4)
        else if (aBot->m_right->m_id < id){
            aBot->m_right->m_right = splay(aBot->m_right->m_right, id);
            if (aBot->m_right->m_right != nullptr){
                // Perform left rotation on grandparent
                aBot->m_right = leftRotation(aBot->m_right);
            }
        }
        // If node's right child is null, return node
        if (aBot->m_right == nullptr){
            return aBot;
        }
        else{
            // Perform left rotation on grandparent (for Case 3)
            // or left rotation on parent (for Case 4)
            aBot = leftRotation(aBot);
            return aBot;
        }
    }
    return aBot;
}

bool Swarm::testBalance(Robot * aBot){
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