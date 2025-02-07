// CMSC 341 - Fall 2024 - Project 3
#include "mqueue.h"
MQueue::MQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure){
  m_priorFunc = priFn;
  m_heapType = heapType;
  m_structure = structure;
  m_heap = nullptr;
  m_size = 0;
}

MQueue::~MQueue(){
  clear();
}

void MQueue::clear() {
  m_heap = clear(m_heap); 
  m_size = 0;
}

Node * MQueue::clear(Node * aNode){
  // Check that memory exists
  if (aNode != nullptr){
    // Clear node's left and right child before clearing node
    aNode->m_left = clear(aNode->m_left);
    aNode->m_right = clear(aNode->m_right);
    delete aNode;
    aNode = nullptr;
  }
  return aNode;
}

MQueue::MQueue(const MQueue& rhs){
  // Check for self-assignment
  if (this != &rhs){
    // Copy member variables
    m_priorFunc = rhs.m_priorFunc;
    m_heapType = rhs.m_heapType;
    m_structure = rhs.m_structure;
    m_size = rhs.m_size;
    m_heap = nullptr;

    // Create deep copy of passed tree
    m_heap = copyTree(m_heap, rhs.m_heap);
    if (m_heap != nullptr){
      m_heap->m_npl = updateNPLs(m_heap);
    }
  }
}

MQueue& MQueue::operator=(const MQueue& rhs) {
  // Check for self-assignment
  if (this != &rhs){
    // Deallocate existing memory
    clear();
    // Allocate new memory and copy values
    m_priorFunc = rhs.m_priorFunc;
    m_heapType = rhs.m_heapType;
    m_structure = rhs.m_structure;
    m_heap = copyTree(m_heap, rhs.m_heap);
    m_size = rhs.m_size;
    if (m_heap != nullptr){
      m_heap->m_npl = updateNPLs(m_heap);
    }
  }
  return *this;
}

Node * MQueue::copyTree(Node * obj, Node * toCopy){
  if (toCopy != nullptr){
    // Copy node and then copy node's left and right child
    obj = new Node(toCopy->m_order);
    obj->m_left = copyTree(obj->m_left, toCopy->m_left);
    obj->m_right = copyTree(obj->m_right, toCopy->m_right);
  }
  return obj;
}

void MQueue::mergeWithQueue(MQueue& rhs) {
  // If priority functions or heap types are different, throw domain error
  if (!(rhs.m_priorFunc == m_priorFunc && rhs.m_heapType == m_heapType)){
    throw std::domain_error("Incorrect domain");
  }
  // Check for self-assignment
  if (this != &rhs){
    // Merge heaps, update NPL values and size
    Node * temp = merge(m_heap, rhs.m_heap);
    m_heap = temp;
    m_heap->m_npl = updateNPLs(m_heap);
    m_size = m_size + rhs.m_size;

    // Passed heap becomes empty
    rhs.m_size = 0;
    rhs.m_heap = nullptr;
  }
}

Node * MQueue::swap(Node * aNode){
  Node * temp = aNode->m_left;
  aNode->m_left = aNode->m_right;
  aNode->m_right = temp;
  return aNode;
}

Node * MQueue::merge(Node * p1, Node * p2){
  // If p1 is empty, return p2 and vice versa
  if (p1 == nullptr){
    return p2;
  }
  else if (p2 == nullptr){
    return p1;
  }
  prifn_t priority = getPriorityFn();
  int rightNPL = -1;
  int leftNPL = -1; 

  if (m_heapType == MINHEAP){
    if (priority(p1->m_order) <= priority(p2->m_order)){
      // If skew heap and p1 has greater priority, swap children of p1
      if (m_structure == SKEW){
        p1 = swap(p1);
      }
      p1->m_right = merge(p1->m_right, p2);
      m_heap->m_npl = updateNPLs(m_heap);

      // If leftist heap and right npl is greater than left npl, swap children
      if (m_structure == LEFTIST){
        if (p1->m_right != nullptr){
          rightNPL = p1->m_right->m_npl;
        }
        if (p1->m_left != nullptr){
          leftNPL = p1->m_left->m_npl;
        }
        if (rightNPL > leftNPL){
          p1 = swap(p1);
        }
      }
      return p1;
    }
    else {
      // If skew heap and p2 has greater priority, swap children of p2
      if (m_structure == SKEW){
        p2 = swap(p2);
      }

      p2->m_right = merge(p1, p2->m_right);
      m_heap->m_npl = updateNPLs(m_heap);

      // If leftist heap and right npl is greater than left npl, swap children
      if (m_structure == LEFTIST){
        if (p2->m_right != nullptr){
          rightNPL = p2->m_right->m_npl;
        }
        if (p2->m_left != nullptr){
          leftNPL = p2->m_left->m_npl;
        }
        if (rightNPL > leftNPL){
          p2 = swap(p2);
        }
      }
      return p2;
    }
  }
  else {
    if (priority(p1->m_order) >= priority(p2->m_order)){
      if (m_structure == SKEW){
        p1 = swap(p1);
      }
      p1->m_right = merge(p1->m_right, p2);
      m_heap->m_npl = updateNPLs(m_heap);

      if (m_structure == LEFTIST){
        if (p1->m_right != nullptr){
          rightNPL = p1->m_right->m_npl;
        }
        if (p1->m_left != nullptr){
          leftNPL = p1->m_left->m_npl;
        }
        if (rightNPL > leftNPL){
          p1 = swap(p1);
        }
      }
      return p1;
    }
    else {
      if (m_structure == SKEW){
        p2 = swap(p2);
      }

      p2->m_right = merge(p1, p2->m_right);
      m_heap->m_npl = updateNPLs(m_heap);

      if (m_structure == LEFTIST){
        if (p2->m_right != nullptr){
          rightNPL = p2->m_right->m_npl;
        }
        if (p2->m_left != nullptr){
          leftNPL = p2->m_left->m_npl;
        }
        if (rightNPL > leftNPL){
          p2 = swap(p2);
        }
      }
      return p2;
    }
  }
}

bool MQueue::insertOrder(const Order& order) {
  // Check that all order values are valid before adding
  if ((order.m_FIFO >= MINONE && order.m_FIFO <= MAX50) &&
    (order.m_processTime >= MINONE && order.m_processTime <= MAX12) &&
    (order.m_dueTime >= MINONE && order.m_dueTime <= MAX12) &&
    (order.m_slackTime >= MINONE && order.m_slackTime <= MAX12) &&
    (order.m_material >= MINZERO && order.m_material <= MAX100) &&
    (order.m_importance >= MINONE && order.m_importance <= MAX100) &&
    (order.m_workForce >= MAX100 && order.m_workForce <= MAX200) &&
    (order.m_quantity >= MIN1000 && order.m_quantity <= MAX10000)){
    Node * newNode = new Node(order);
    if (m_heap == nullptr){
      m_heap = newNode;
    }
    else{
      // Insert by merging
      m_heap = merge(m_heap, newNode);
    }
    // Update size and npl values
    m_size++;
    m_heap->m_npl = updateNPLs(m_heap);

    return true;
  }
  else{
    return false;
  }
}

int MQueue::updateNPLs(Node * aNode){
  if (aNode == nullptr){
    return -1;
  }
  int rightNPL = updateNPLs(aNode->m_right);
  int leftNPL = updateNPLs(aNode->m_left);
  
  // Set node's npl equal to lowest npl of children plus 1
  if (leftNPL <= rightNPL){
    aNode->m_npl = leftNPL + 1;
  }
  else{
    aNode->m_npl = rightNPL + 1;
  }
  return aNode->m_npl;
}

int MQueue::numOrders() const{
  return m_size;
}

prifn_t MQueue::getPriorityFn() const {
  return m_priorFunc;
}

Order MQueue::getNextOrder() {
  // If queue is empty, throw out of range error
  if (m_heap != nullptr){
    Node * temp = m_heap;
    Order order = temp->m_order;
    // Create new heap by merging root's left and right child
    m_heap = merge(m_heap->m_left, m_heap->m_right);
    m_size--;
    // Delete root and return order
    delete temp;
    temp = nullptr;
    return order;
  } 
  else{
    throw std::out_of_range("Index out of range");
  }
}

void MQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
  m_priorFunc = priFn;
  m_heapType = heapType;

  // Put all nodes in array
  Node * arr[m_size + 1];
  int size = m_size;
  m_size = 1;
  makeArray(m_heap, arr, size);
  m_size = size;

  // Reverse order of array
  reHeapify(arr);

  // Build new tree from array
  m_heap = build(arr, 1);
  m_heap->m_npl = updateNPLs(m_heap);              
}

Node * MQueue::build(Node * arr[], int index){
  // If node has left child, update left child pointer
  if (index*2 <= m_size){
    arr[index]->m_left = arr[index * 2];
    build(arr, index * 2);
  }
  else{
    arr[index]->m_left = nullptr;
  }

  // Do same for right child
  if ((index * 2) + 1 <= m_size){
    arr[index]->m_right = arr[(index *2) + 1];
    build(arr, (index * 2) + 1);
  }
  else{
    arr[index]->m_right = nullptr;
  }
  
  return arr[index];
}

void MQueue::reHeapify(Node * heap[]){
  for (int i = m_size / 2; i >= 1; i--){
      downHeap(heap, i);
  }
}

void MQueue::downHeap(Node * heap[], int index){
  int minChild = index;
  int left = index*2;
  int right = (index*2) + 1;
  prifn_t priority = getPriorityFn();
  if (m_heapType == MINHEAP){
    if (left <= m_size && priority(heap[left]->m_order) < priority(heap[index]->m_order)){
        minChild = left;
    }
    if (right <= m_size && priority(heap[right]->m_order) < priority(heap[minChild]->m_order)){
        minChild = right;
    }
    if (index != minChild){
        Node * temp = heap[minChild];
        heap[minChild] = heap[index];
        heap[index] = temp;
        downHeap(heap, minChild);
    }
  }
  else{
    if (left <= m_size && priority(heap[left]->m_order) > priority(heap[index]->m_order)){
        minChild = left;
    }
    if (right <= m_size && priority(heap[right]->m_order) > priority(heap[minChild]->m_order)){
        minChild = right;
    }
    if (index != minChild){
        Node * temp = heap[minChild];
        heap[minChild] = heap[index];
        heap[index] = temp;
        downHeap(heap, minChild);
    }
  }
}

void MQueue::makeArray(Node * aNode, Node * arr[], int size){
  if (aNode != nullptr){
    arr[m_size] = aNode;
    m_size++;
    makeArray(aNode->m_left, arr, size);
    makeArray(aNode->m_right, arr, size);
  }
}

void MQueue::setStructure(STRUCTURE structure){
  // Check that new structure is different
  if (m_structure != structure){
    m_structure = structure;

    // If changing from skew to leftist heap, rebuild heap
    // If changing from leftist to skew, do nothing
    if (m_structure == LEFTIST){
      Node * arr[m_size];
      int size = m_size;
      m_size = 0;
      makeArray(m_heap, arr, size);
      m_size = size;
      m_heap = nullptr;

      for (int i = 0; i < m_size; i++){
        arr[i]->m_left = nullptr;
        arr[i]->m_right = nullptr;
        m_heap = merge(m_heap, arr[i]);
        m_heap->m_npl = updateNPLs(m_heap);
      }
    }
  }
}

STRUCTURE MQueue::getStructure() const {
    return m_structure;
}

HEAPTYPE MQueue::getHeapType() const{
    return m_heapType;
}

void MQueue::printOrderQueue() const {
  cout << "Contents of the queue:" << endl;
  preOrderTraversal(m_heap);
}

void MQueue::preOrderTraversal(Node * aNode) const{
  prifn_t priority = m_priorFunc;
  if (aNode != nullptr){
    cout << "[" << priority(aNode->m_order) << "] " << aNode->m_order << endl;
    preOrderTraversal(aNode->m_left);
    preOrderTraversal(aNode->m_right);
  }
}

void MQueue::dump() const {
  if (m_size == 0) {
    cout << "Empty heap.\n" ;
  } else {
    dump(m_heap);
  }
  cout << endl;
}

void MQueue::dump(Node *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->m_left);
    if (m_structure == SKEW)
        cout << m_priorFunc(pos->m_order) << ":" << pos->m_order.m_customer;
    else
        cout << m_priorFunc(pos->m_order) << ":" << pos->m_order.m_customer << ":" << pos->m_npl;
    dump(pos->m_right);
    cout << ")";
  }
}

ostream& operator<<(ostream& sout, const Order& order) {
  sout  << "Customer: " << order.getCustomer()
        << ", importance: " << order.getImportance() 
        << ", quantity: " << order.getQuantity() 
        << ", order placement time: " << order.getFIFO();
  return sout;
}

ostream& operator<<(ostream& sout, const Node& node) {
  sout << node.m_order;
  return sout;
}