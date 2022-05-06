#include "mino.hpp"
#include <algorithm> //std::sort
#include <math.h> // pow()
#include <assert.h> // assert()
#include <iostream> //cout

/* Transformations of a mino */
vector<vector<int>> & shift(vector<vector<int>> & coords);
vector<vector<int>> & reorder(vector<vector<int>> &coords);
int xMax(vector<vector<int>> & coords);
int yMax(vector<vector<int>> & coords);
/* Horizontal symetry */
vector<vector<int>> hSymetry(vector<vector<int>> & mino_coords);
/* Vertical symetry */
vector<vector<int>> vSymetry(vector<vector<int>> & mino_coords);
/* x,y coordinates swap. First bissectrice symetry */
vector<vector<int>> xSymetry(vector<vector<int>> & mino_coords);
/* function pointer */
typedef vector<vector<int>> (*trans_ptr)(vector<vector<int>> &);
trans_ptr transformations[3] {&xSymetry,&hSymetry,&vSymetry};

/* Constructor */
mino::mino(vector<int> _indices, int _d) :
n(_indices.size()),
d(_d),
indices(_indices),
coordinates(n,vector<int>(d))
{
  updateCoordinates();
  computeOrientations();
}

/* update mino coordinates from its indices (given in grid (n+1)x(n+1)) */
void mino::updateCoordinates() {
  for(int i=0;i<n;i++){
    coordinates[i][0] = indices[i]/(n+1);
    coordinates[i][1] = indices[i]%(n+1);
  }
  return;
}

/* update mino indices from its coordinates.
   Indices are given in an (n+1)x(n+1) grid */
void mino::updateIndices() {
  for(int i=0;i<n;i++){
    indices[i] = (n+1)*coordinates[i][0]+coordinates[i][1];
  }
  return;
}

/* compute all the different orientation of the n-mino */
void mino::computeOrientations() {
  // First find all possible orientations of the mino
  vector<vector<vector<int>>> _orientations;
  _orientations.push_back(coordinates);
  for(int s=0;s<3;s++){
    for(uint i=0;i<_orientations.size();i++) {
      vector<vector<int>> newTrans = transformations[s](_orientations[i]);
      bool isNew = true;
      for(uint j=0;j<_orientations.size();j++) {
        if(_orientations[j] == newTrans) {isNew=false;break;}
      }
      if(isNew){
        _orientations.push_back(newTrans);
      }
    }
  }
  nOrientations = _orientations.size();
  // Next sort orientations uniquely
  sort(_orientations.begin(),_orientations.end(),
    [](vector<vector<int>> pos1,vector<vector<int>> pos2){
      assert(pos1.size() == pos2.size());
      int n = pos1.size();
      int sum1=0,sum2=0;
      for(uint i=0;i<pos1.size();i++){
        sum1 += pow(2,(pos1[i][0]*(n+1)+pos1[i][1]));
        sum2 += pow(2,(pos2[i][0]*(n+1)+pos2[i][1]));
      }
      return sum1<sum2;
    }
  );
  orientations = _orientations;
  // Finally store the unique representant
  coordinates = orientations[0];
  updateIndices();
  return;
}

vector<vector<int>> mino::getOrientationsAsIndices(){
  vector<vector<int>> allOrientations(nOrientations,vector<int>(n));
  for(int i=0;i<nOrientations;i++){
    for(int j=0;j<n;j++){
      allOrientations[i][j] = orientations[i][j][0]*(n+1)+orientations[i][j][1];
    }
  }
  return allOrientations;
}

void mino::computeAllPositions(vector<int> grid,vector<vector<int>> &positions,int minoIndex){
  for(int i=0;i<nOrientations;i++){
    vector<vector<int>> orientation = orientations[i];
    int yShift = 0;
    vector<int> position(n+1);
    position[n] = minoIndex;
    while(yMax(orientation)<grid[0]){
      while(xMax(orientation)<grid[1]){
        for(uint j=0;j<orientation.size();j++) {
          position[j] = grid[1]*orientation[j][0]+orientation[j][1];
          orientation[j][1]++;
        }
        positions.push_back(position);
      }
      yShift++;
      orientation = orientations[i];
      for(uint j=0;j<orientation.size();j++) {
        orientation[j][0] += yShift;
      }
    }
  }
  return;
}

/* Transformations of a mino */
vector<vector<int>> & shift(vector<vector<int>> & coords){
  int n = coords.size();
  int xMin=n,yMin=n;
  for(int i=0;i<n;i++){
    xMin = coords[i][1]<xMin?coords[i][1]:xMin;
    yMin = coords[i][0]<yMin?coords[i][0]:yMin;
  }
  for(int i=0;i<n;i++){
    coords[i][1] -= xMin;
    coords[i][0] -= yMin;
  }
  return coords;
}

vector<vector<int>> & reorder(vector<vector<int>> &coords){
  sort(coords.begin(),coords.end(),
    [](vector<int> c1,vector<int> c2){
      if(c1[0]==c2[0]) {
        return c1[1]<c2[1];
      }
      else {
        return c1[0]<c2[0];
      }
    }
  );
  return coords;
}
/* Horizontal symetry */
vector<vector<int>> hSymetry(vector<vector<int>> & mino_coords){
  int n = mino_coords.size();
  vector<vector<int>> hMino(mino_coords);
  for(int i=0;i<n;i++) {
    hMino[i][0] = n-hMino[i][0];
  }
  return reorder(shift(hMino));
}
/* Vertical symetry */
vector<vector<int>> vSymetry(vector<vector<int>> & mino_coords){
  int n = mino_coords.size();
  vector<vector<int>> vMino(mino_coords);
  for(int i=0;i<n;i++) {
    vMino[i][1] = n-vMino[i][1];
  }
  return reorder(shift(vMino));
}
/* x,y coordinates swap. First bissectrice symetry */
vector<vector<int>> xSymetry(vector<vector<int>> & mino_coords){
  int n = mino_coords.size();
  vector<vector<int>> xMino(mino_coords);
  for(int i=0;i<n;i++) {
    int inter   = xMino[i][1];
    xMino[i][1] = xMino[i][0];
    xMino[i][0] = inter;
  }
  return reorder(shift(xMino));
}

int xMax(vector<vector<int>> &coords){
  int m=0;
  for(vector<int> p : coords){
    m = m<p[1]?p[1]:m;
  }
  return m;
}

int yMax(vector<vector<int>> &coords){
  int m=0;
  for(vector<int> p : coords){
    m = m<p[0]?p[0]:m;
  }
  return m;
}
