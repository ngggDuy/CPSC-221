/**
 * @file rr.cpp
 * Implementation of rainbowRipple.
 *
 * Modify and turn in this file for the PA!
 *
 */
#include "rr.h"

// This shows all the function declarations (and some of the
// functions) that I used to implement rainbowRipple.
// You can create additional functions and/or delete/replace functions
// as you wish.

void ERROR_badGaps(string sgc) {
// Report error and exit.
//
// ** You don't need to modify this function. **
//
  cerr << "ERROR: Unable to parse string specifying gaps and colors: "
       << "\"" << sgc << "\""
       << endl;
  exit(-1);
}

vector<pair<int,RGBAPixel>> parseGaps(string sgc) {
// Convert a string containing a sequence of number,character pairs,
// where number is an integer >= 0 and character is one of roygbiv,
// into a vector<pair<int,RGBAPixel>> of the sequence of pairs except
// the characters are replaced by the corresponding RGBAPixel.
// For example, the string "0r10g7b" converts to the vector
// [(0,RGBAPixel(255,0,0)),(10,RGBAPixel(0,255,0)),(7,RGBAPixel(0,0,255))]
//
// ** You don't need to modify this function. **
//
  vector<pair<int,RGBAPixel>> vgc;
  RGBAPixel c;
  size_t idx = 0;
  size_t i = 0;
  int g;
  while( idx < sgc.size() ) {
    try {
      g = stoi(sgc.substr(idx), &i);
    } catch( invalid_argument& ia ) {
      ERROR_badGaps(sgc);
    }
    idx += i;
    if( g < 0 || idx >= sgc.size() ) ERROR_badGaps(sgc);
    switch( sgc[idx] ) {
    case 'R':
    case 'r':
      c =  RGBAPixel(255,0,0);	//red
      break;
    case 'O':
    case 'o':
      c = RGBAPixel(255,165,0);	//orange
      break;
    case 'Y':
    case 'y':
      c = RGBAPixel(255,255,0);	//yellow
      break;
    case 'G':
    case 'g':
      c = RGBAPixel(0,255,0);	//green
      break;
    case 'B':
    case 'b':
      c = RGBAPixel(0,0,255);	//blue
      break;
    case 'I':
    case 'i':
      c = RGBAPixel(75,0,130);	//indigo
      break;
    case 'V':
    case 'v':
      c = RGBAPixel(238,130,238);//violet
      break;
    default:
      ERROR_badGaps(sgc);
    }
    vgc.push_back(make_pair(g,c));
    idx++;
  }
  return vgc;
}

bool closeEnough(RGBAPixel c1, RGBAPixel c2) {
// Returns true iff color c1 is close enough to color c2
//
// ** You don't need to modify this function. **
//
  int dist = (c1.r-c2.r)*(c1.r-c2.r) + (c1.g-c2.g)*(c1.g-c2.g) +
    (c1.b-c2.b)*(c1.b-c2.b);

  return (dist <= 80);
}

bool withinImage(pair<int,int> next, int width, int height) {
  return (next.first >= 0 && next.first < width) &&
      (next.second >= 0 && next.second < height);
}

bool good(PNG & image, vector<vector<int>> & D,
		pair<int,int> curr, pair<int,int> next) {
// Returns true iff a neighbor "next" of a pixel "curr" is:
// 1. within the image,
// 2. unvisited, and
// 3. close in color to "curr".
// An entry in distance table D is -1 only if the pixel has not been 
// visited.
int w = image.width();
int h = image.height();
if (!withinImage(next, w, h))
  return false;

RGBAPixel* c1 = image.getPixel(next.first, next.second);
RGBAPixel* c2 = image.getPixel(curr.first, curr.second);

return D[next.second][next.first] == -1 && 
      closeEnough(*c1, *c2);
}

vector<pair<int,int>> neighbors(pair<int,int> curr) {

  // possible new neighbors array
  vector<pair<int,int>> n;
  
  pair<int, int> top (curr.first, curr.second - 1);     // TOP
  pair<int, int> left (curr.first - 1, curr.second);    // LEFT
  pair<int, int> right (curr.first + 1, curr.second);   // RIGHT
  pair<int, int> bottom (curr.first, curr.second + 1);  // BOTTOM
 
  n.push_back(top);
  n.push_back(left);
  n.push_back(right);
  n.push_back(bottom);
  return n;
}

void initialiseColors(vector<pair<int,RGBAPixel>> & vgc, int & modConstant) {
  // total gives us constant modding operand
  int total = 0;
  for (int i = 0; i < (int) vgc.size(); i++) {
    total += vgc[i].first + 1;
    // this line gives us other modding operand
    vgc[i].first = total;
  }
  modConstant = total;
}

void rainbowRipple(PNG & image, pair<int,int> start, string sgc) {
// Color certain pixels in "image" based on their distance from "start"
// using the gaps-and-colors string "sgc" to specify the color pattern.
// The distance is determined by Breadth-First-Search which is a
// Level-Order Traversal of the tree of shortest paths from "start"
// whose edges are between good neighbors.  

// The neighbors of pixel
// (x,y) are (x-1,y), (x,y+1), (x+1,y), (x,y-1).  The good neighbors
// of (x,y) are those neighbors that
// 1. lie within the image,
// 2. are close to the color of pixel (x,y), and
// 3. have not been visited before.

// BFS visits "start", then the good neighbors of "start", then the
// good neighbors of these good neighbors, etc.  This forms a tree,
// level by level. The distance of a pixel from "start" is its level
// in this tree (or -1 if it hasn't been visited).  As soon as BFS
// visits a pixel it knows its distance from "start" and it can color
// the pixel based on the gaps-and-colors string "sgc".  (* However,
// it should wait to color the pixel until after its good neighbors
// are computed, since the original pixel color determines what
// neighbors are good. *)

// Coloring:
// Suppose (gap_1,color_1), (gap_2,color_2), ..., (gap_n,color_n) is the
// sequence of number,color pairs in "sgc".  Those pixels at distance
// gap_1 from "start" are colored color_1, those at distamce
// gap_1 + 1 + gap_2 are colored color_2, etc.  The gaps are the
// number of uncolored pixels between colored pixels along a shortest
// path.  The sequence specified by "sgc" should be viewed as
// repeating forever, so a pixel at distance 33 from "start" would be
// colored blue using the sequence (9,red),(4,blue),(3,green) since
// 33 = (9+1+4+1+3+1)+9+1+4.

  // initialising table D for distances
  vector<vector<int>> D(image.height(), vector<int> (image.width(), -1));

  // initialising vector for sgc
  int modConstant = 0;
  vector<pair<int,RGBAPixel>> vgc = parseGaps(sgc);
  initialiseColors(vgc, modConstant);
  
  // queue for BFS
  Queue<pair<int, int>> q;     

  // mark the starting pixel as visited and enqueue
  D[start.second][start.first] = 0;      
  q.enq(start);                          

  // temporary neighbors vector
  vector<pair<int, int>> nVect;                       

  // main loop
  while (!q.empty()) {
    
    // dequeue a pixel and find neighbors
    pair<int, int> curr;  
    curr = q.deq();
    nVect = neighbors(curr);

    int distance = D[curr.second][curr.first] + 1;
    // find and queue good neighbors, fill distance table
    for (int i = 0; i < 4; i++) {
        if (good(image, D, curr, nVect[i])) {
          q.enq(nVect[i]);
          D[nVect[i].second][nVect[i].first] = distance;
        }
    }

    // color curr
    RGBAPixel* pixel = image.getPixel(curr.first, curr.second);
    int key = distance % modConstant;
      for (int i = 0; i < (int) vgc.size(); i++) {
        if (key == 0 || vgc[i].first == key) {
          *pixel = (key == 0 ? vgc[vgc.size()-1].second : vgc[i].second);
          break;
        }
      }
  }
}








