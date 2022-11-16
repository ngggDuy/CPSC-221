#include "chain.h"
#include "chain_given.cpp"
#include <cmath>
#include <iostream>

// PA1 functions

/**
 * Destroys the current Chain. This function should ensure that
 * memory does not leak on destruction of a chain.
 */
Chain::~Chain() {
  clear();
}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
void Chain::insertBack(const Block &ndata) {
  Node *newNode = new Node(ndata);
  insertAt(newNode, length_ + 1);
}


/**
 * Swaps the two nodes at the indexes "node1" and "node2".
 * The indexes of the nodes are 1 based.
 * assumes i and j are valid (in {1,length_} inclusive)
 * 
 */
void Chain::swap(int i, int j) {
  Node *node1 = deleteAt(i);
  Node *node2 = deleteAt(j-1);
  insertAt(node2, i);
  insertAt(node1, j);
}


/**
 * Reverses the chain
 */
void Chain::reverse() {
  if (length_ < 2) return;
  Node *prev = head_;
  Node *curr = head_->next;
  while (curr != NULL & curr != head_) { 
      Node *next = curr->next; 
      curr->next = prev;
      prev = curr;
      curr = next;
  }
  head_->next = prev;
}

/*
* Modifies the current chain by "rotating" every group of k nodes by one position.
* In every k-node sub-chain, remove the first node, and place it in the last 
* position of the sub-chain. If the last sub-chain has length less than k,
* then don't change it at all. 
* Some examples with the chain a b c d e:
*   k = 1: a b c d e
*   k = 2: b a d c e
*   k = 3: b c a d e
*   k = 4: b c d a e
*/
void Chain::rotate(int k) {
  if (k < 2) return;
  int count = 1;

  Node *curr = head_->next;
  while (curr != head_) {
    if (count % k == 1 && count < length_) {
      Node * shift = deleteAt(count);
      insertAt(shift, count + k - 1);
    }
    curr = curr->next;
    count++;
  }
}

/**
 * Destroys all dynamically allocated memory associated with the
 * current Chain class.
 */
void Chain::clear() {
  Node *curr = head_->next;
  while (curr != head_) {
    Node *next = curr->next;
    head_->next = next;
    delete curr;
    curr = next;
  }
  length_ = 0;
  delete head_;
  head_ = NULL;
}

/* makes the current object into a copy of the parameter:
 * All member variables should have the same value as
 * those of other, but the memory should be completely
 * independent. This function is used in both the copy
 * constructor and the assignment operator for Chains.
 */
void Chain::copy(Chain const &other) {
  width_ = other.width_;
  height_ = other.height_;
  length_ = 0;
  head_ = new Node(other.head_->data);
  head_->next = head_;

  Node *curr = other.head_->next; 

  while(curr != other.head_) {
    insertBack(curr->data);
    curr = curr->next;
  } 
}

Chain::Node* Chain::deleteAt(int i) {
  Node* prev = head_;
  Node* curr = head_->next;
  int count = 1;
  while (curr != head_) {
    if (count == i) {
      Node* next = curr->next;
      prev->next = next;
      length_--;
      return curr;
    }
    prev = curr;
    curr = curr->next;
    count++;
  }
  return new Node();
}

void Chain::insertAt(Node *n, int i) {
  int count = 1;
  if (i == 1) {
    Node *next = head_->next;
    head_->next = n;
    n->next = next;
    length_++;
    return;
  }
  Node *curr = head_->next;
  while(curr != head_) {
    if (count == i - 1) {
      Node *next = curr->next;
      curr->next = n;
      n->next = next;
      length_++;
      return;
    }
    curr = curr->next;
    count++;
  }
}
