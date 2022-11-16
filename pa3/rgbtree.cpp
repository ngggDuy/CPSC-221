/**
 * @file rgbtree.cpp
 * Implementation of rgbtree class.
 */

#include <utility>
#include <algorithm>
#include "rgbtree.h"

using namespace std;


void rgbtree::buildTree(int start, int end, int k, int d) 
{
  quickSelect(start, end, k, d % 3);
  if (start < end) {    
    // left subtree
    buildTree(start, k - 1, (start + k - 1) / 2, (d + 1) % 3);
    // right subtree
    buildTree(k + 1, end, (k + 1 + end) / 2, (d + 1) % 3);
  }
  
}

rgbtree::rgbtree(const map<RGBAPixel,string>& photos)
{
  // const pair<RGBAPixel, string>
  for (auto & thumb : photos) {
    tree.push_back(thumb.first);
  }
  buildTree(0, tree.size() - 1, (tree.size() - 1) / 2, 0);
}



RGBAPixel rgbtree::findNearestNeighbor(const RGBAPixel & query) const
{
  RGBAPixel p;
  return p;

}

// curDim = level mod k
bool rgbtree::smallerByDim(const RGBAPixel & first,
                                const RGBAPixel & second, int curDim) const
{
  unsigned char a, b;
  if (curDim == 0) {
    a = first.r;
    b = second.r;
  } else if (curDim == 1) {
    a = first.g;
    b = second.g;
  } else {
    a = first.b;
    b = second.b;
  }

  if (a == b) return first.operator<(second);
  return a < b;
}

/**
 * This function splits the trees[start..end] subarray at position start k
 */
void rgbtree::quickSelect(int start, int end, int k, int d)
{
  if (start < end) {
    int p = partition(start, end, d);
    if (k == p) {
      return;
    } else if (k < p) {
      quickSelect(start, p - 1, k, d);
    } else {
      quickSelect(p + 1, end, k, d);
    } 
  }
}
int rgbtree::EuclideanDistSquare(const RGBAPixel& query, const RGBAPixel& curr) const
{
  return (curr.r - query.r) * (curr.r - query.r) + 
         (curr.g - query.g) * (curr.g - query.g) +
         (curr.b - query.b) * (curr.b - query.b);
}


/**
 * This method does a partition around pivot and will be used 
 * in quick select. It uses tree[lo] as the default pivot.
 * It returns the index of the pivot in the updated vector.
 * You will likely need to modify and complete this code.
 */
int rgbtree::partition(int lo, int hi, int d) 
{
    int p = lo;
    for( int i=lo+1; i <= hi; i++ ) {
      if (smallerByDim(tree[i], tree[lo], d)) {
        p++; swap(tree[p], tree[i]);
      }
      swap(tree[lo], tree[p]);
    }
    return p;
}


/**
 * Helper function to help determine if the nearest neighbor could 
 * be on the other side of the KD tree.
 */
int rgbtree::distToSplit(const RGBAPixel& query, const RGBAPixel& curr, int dimension) const
{
  if (dimension == 0) {
    return pow(query.r - curr.r, 2);
  } else if (dimension == 1) {
    return pow(query.g - curr.g, 2);
  } else if (dimension == 2) {
    return pow(query.b - curr.b, 2);
  }
  return -1;

}

