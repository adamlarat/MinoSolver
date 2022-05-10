#ifndef MINO_H
#define MINO_H

#include <vector>
#include <assert.h>

using namespace std;

class mino {
private:
  int n=0; // mino size (known as n-mino)
  int d=3; // dimension (probably useless now...)
  vector<int> indices; // Indices of the 2d-mino in a (n+1)x(n+1) grid
  vector<vector<float>> coordinates; // Coordinates of the mino in d dimensions. z components should be zero.
  /* different orientations of the mino */
  vector<vector<vector<float>>> orientations2d; // in the 2d plane
  vector<vector<vector<float>>> orientations3dStraight; // with a π/2 Ox or Oy rotation
  vector<vector<vector<float>>> orientations3dDiagged; // with a π/2 (Ox+Oy) rotation
  //vector<vector<int>> *orientations;
  int nOrientations2d=0,nOrientations3dStraight=0,nOrientations3dDiagged=0; // number of different orientations
public:
  int nPositions2d=0,nPositions3dStraight=0,nPositions3dDiagged=0; // number of different orientations
  /* constructors and destructor */
  mino() {}
  mino(vector<int> _indices,int _d=3);
  ~mino() {}

  int size() {return n;}
  int dimensions() {return d;}
  int number() {return nOrientations2d+nOrientations3dStraight+nOrientations3dStraight;}
  int number2d() {return nOrientations2d;}
  int number3dStraight() {return nOrientations3dStraight;}
  int number3dDiagged() {return nOrientations3dDiagged;}
  vector<int> & getIndices() {return indices;}
  vector<vector<float>> & getCoordinates() {return coordinates;}
  vector<vector<float>>   getOrientation(int i) {
    assert(i<number());
    if(i<nOrientations2d) return orientations2d[i];
    i -= nOrientations2d;
    if(i<nOrientations3dStraight) return orientations3dStraight[i];
    i -= nOrientations3dStraight;
    return orientations3dDiagged[i];
  }
  void contribute(int i){
    if(i<0){nPositions2d=0;nPositions3dStraight=0;nPositions3dDiagged=0;return;}
    assert(i<number());
    if(i<nOrientations2d) {nPositions2d++;return;}
    i -= nOrientations2d;
    if(i<nOrientations3dStraight) {nPositions3dStraight++;return;}
    nPositions3dDiagged++;
    return;
  }
  vector<vector<vector<float>>> & getOrientations2d() {return orientations2d;}
  vector<vector<vector<float>>> & getOrientations3dStraight() {return orientations3dStraight;}
  vector<vector<vector<float>>> & getOrientations3dDiagged() {return orientations3dDiagged;}

  /* update mino coordinates from its indices (given in grid (n+1)x(n+1)) */
  void updateCoordinates();
  /* update mino indices from its coordinates.
     Indices are given in an (n+1)x(n+1) grid */
  void updateIndices();
  /* compute all the different orientation of the n-mino */
  void computeOrientations();
  /* given a grid, compute all the positions of all the orientations
     within the grid */
  void computeAllPositions(vector<vector<float>> &grid,vector<vector<int>> &positions,int minoIndex);

};

#endif // MINO_H
