/**
 * @file binarytree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file, as well as binarytree.h.
 */

#include <iostream>

using namespace std;

/**
 * @return The height of the binary tree. Recall that the height of a binary
 *  tree is just the length of the longest path from the root to a leaf, and
 *  that the height of an empty tree is -1.
 */
template <typename T>
int BinaryTree<T>::height() const
{
    // Call recursive helper function on root
    return height(root);
}

/**
 * Private helper function for the public height function, with an additional
 * Node* parameter to allow for recursive calls. NOTE: use this as an example
 * for writing your own private helper functions.
 * @param subRoot
 * @return The height of the subtree.
 */
template <typename T>
int BinaryTree<T>::height(const Node* subRoot) const
{
    // Base case
    if (subRoot == NULL)
        return -1;

    // Recursive definition
    return 1 + max(height(subRoot->left), height(subRoot->right));
}

/**
 * Prints out the values of the nodes of a binary tree in order.
 * That is, everything to the left of a node will be printed out before that
 * node itself, and everything to the right of a node will be printed out after
 * that node.
 */
template <typename T>
void BinaryTree<T>::printLeftToRight() const
{
    // Your code here
    printLeftToRight(root);
    // Do not remove this line - used for correct print output
    cout << endl;
}

/**
 * Private helper function for the public printLeftToRight function, with an additional
 * Node* parameter to allow for recursive calls. 
 * @param subRoot
 */
template <typename T>
void BinaryTree<T>::printLeftToRight(const Node* subRoot) const
{
    if (subRoot != NULL) {
        // first recur on left child
        printLeftToRight(subRoot->left);
        // print node data
        cout << subRoot->elem << " ";
        // now recur on right
        printLeftToRight(subRoot->right);
    }
}
/**
 * Flips the tree over a vertical axis, modifying the tree itself
 * (i.e. not creating a flipped copy).
 */
template <typename T>
void BinaryTree<T>::mirror()
{
    // Your code here
    mirror(root);
}

/**
 * Private helper function for the public printLeftToRight function, with an additional
 * Node* parameter to allow for recursive calls. 
 * @param subRoot
 */
template <typename T>
void BinaryTree<T>::mirror(Node* subRoot) 
{
    // Base case
    if (subRoot == NULL) 
        return;
    else 
    {
        struct Node* temp;
        // Recursive definition
        mirror(subRoot->left);
        mirror(subRoot->right);

        // change pointers for temp node
        temp = subRoot->left;
        subRoot->left = subRoot->right;
        subRoot->right = temp;
    }
}

/**
 * Prints out all the possible paths from the root of the tree to any leaf node.
 * That is, all sequences starting at the root node and continuing downwards,
 * ending at a leaf node. Paths ending in a left node should be printed before
 * paths ending in a node further to the right.
 */
template <typename T>
void BinaryTree<T>::printPaths() const
{
    // Your code here
    queue<Node*> paths;
    printPaths(root, paths);
}

/**
 * Private helper function for the public printPaths function, with an additional
 * Node* parameter to allow for recursive calls and queue<Node*> parameter to 
 * store current paths.
 * @param subRoot
 * @param paths
 */
template <typename T>
void BinaryTree<T>::printPaths(Node* subRoot, queue<Node*> paths) const 
{
    // Base case
    if (subRoot == NULL) 
        return;

    if (subRoot->left == NULL && subRoot->right == NULL) {
        cout << "Path: ";
        Node* curr;
        paths.push(subRoot);
        while (!paths.empty()) {
            curr = paths.front();
            cout << curr->elem << " ";
            paths.pop();
        }
        cout << endl;
        return;
    }

    // Recursive cases
    paths.push(subRoot);
    printPaths(subRoot->left, paths);
    printPaths(subRoot->right, paths);
}

/**
 * Each node in a tree has a distance from the root node - the depth of that
 * node, or the number of edges along the path from that node to the root. This
 * function returns the sum of the distances of all nodes to the root node (the
 * sum of the depths of all the nodes). Your solution should take O(n) time,
 * where n is the number of nodes in the tree.
 * @return The sum of the distances of all nodes to the root.
 */
template <typename T>
int BinaryTree<T>::sumDistances() const
{
    // Your code here
    return sumDistances(root, 0);
}

/**
 * Private helper function for the public sumDistances function, with an additional
 * Node* parameter to allow for recursive calls and int parameter to 
 * store current distance.
 * @param subRoot
 * @param currDistance
 */
template <typename T>
int BinaryTree<T>::sumDistances(Node* subRoot, int currDistance) const 
{
    // Base case
    if (subRoot == NULL) return 0;

    // Recursive definition
    return sumDistances(subRoot->left, currDistance + 1) + sumDistances(subRoot->right, currDistance + 1) + currDistance;
}

/**
 * @return True if an in-order traversal of the tree would produce a
 *  nondecreasing list output values, and false otherwise. This is also the
 *  criterion for a binary tree to be a binary search tree.
 */
template <typename T>
bool BinaryTree<T>::isOrdered() const
{
    // Your code here
    return isOrdered(root, INT_MIN, INT_MAX);
}

/**
 * Private helper function for the public isOrdered function, with an additional
 * Node* parameter to allow for recursive calls and 2 int parameters to 
 * keep track of the rolling minimum and maximum values each node has to be 
 * checked against.
 * @param subRoot
 * @param min
 * @param max
 */
template <typename T>
bool BinaryTree<T>::isOrdered(Node* subRoot, int min, int max) const 
{
    // Base case
    if (subRoot == NULL) return true;

    // check variant
    if (subRoot->elem < min || subRoot->elem > max) return false;

    // Recursive call into children, updating max and min
    return isOrdered(subRoot->left, min, subRoot->elem - 1) && isOrdered(subRoot->right, subRoot->elem - 1, max);

}
