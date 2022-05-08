#include "mino.hpp"
#include <algorithm> //std::sort
#include <math.h> // pow()
#include <iostream> //cout
#include "tools.hpp"

/* Transformations of a mino */
vector<vector<float>> & shift(vector<vector<float>> & coords);
vector<vector<float>> & reorder(vector<vector<float>> &coords);
float dirMax(const vector<vector<float>> & coords,int dir);
float dirMin(const vector<vector<float>> & coords,int dir);
/* (Ox,Oz) symetry */
vector<vector<float>> hSymetry(const vector<vector<float>> & mino_coords);
/* (Oy,Oz) symetry */
vector<vector<float>> vSymetry(const vector<vector<float>> & mino_coords);
/* x,y coordinates swap. (Ox+Oy,Oz) symetry */
vector<vector<float>> xSymetry(const vector<vector<float>> & mino_coords);
/* Ox π/2 rotation */
vector<vector<float>> xRotation(const vector<vector<float>> & mino_coords);
/* Oy π/2 rotation */
vector<vector<float>> yRotation(const vector<vector<float>> & mino_coords);
/* (Ox+Oy) π/2 rotation */
vector<vector<float>> xpyRotation(const vector<vector<float>> & mino_coords);
/* (Ox-Oy) π/2 rotation */
vector<vector<float>> xmyRotation(const vector<vector<float>> & mino_coords);
/* function pointer */
typedef vector<vector<float>> (*trans_ptr)(const vector<vector<float>> &);
const int n2d = 3;
trans_ptr transformations2d[n2d] {&xSymetry,&hSymetry,&vSymetry};
const int n3d = 2;
trans_ptr transformations3dStraight[n3d] {&xRotation,&yRotation};
trans_ptr transformations3dDiagged[n3d] {&xpyRotation,&xmyRotation};
template<typename T> bool vectorOrder(const vector<vector<T>> &v1,const vector<vector<T>> &v2);
template<typename T> bool cellOrder(const vector<T> &c1,const vector<T> &c2);
bool areEqual(const vector<vector<float>> &v1, const vector<vector<float>> &v2);
int trouveCell(const vector<float> &coords,const vector<vector<float>> &grid);
bool notIn(const vector<vector<vector<float>>> &os, const vector<vector<float>> &newOs);

/* Constructor */
mino::mino(vector<int> _indices, int _d) :
n(_indices.size()),
d(_d),
indices(_indices),
coordinates(n,vector<float>(d))
{
  updateCoordinates();
  computeOrientations();
}

/* update mino coordinates from its indices (given in grid (n+1)x(n+1)) */
void mino::updateCoordinates() {
  for(int i=0;i<n;i++){
    coordinates[i][0] = indices[i]%(n+1); // x
    coordinates[i][1] = indices[i]/(n+1); // y
    coordinates[i][2] = 0.0;              // z
  }
  return;
}

/* update mino indices from its coordinates.
   Indices are given in an (n+1)x(n+1) grid */
void mino::updateIndices() {
  for(int i=0;i<n;i++){
    assert(fabs(coordinates[i][2])<ZERO);
    indices[i] = (n+1)*coordinates[i][1]+coordinates[i][0];
  }
  return;
}

/* compute all the different orientations of the n-mino */
void mino::computeOrientations() {
  // First find all possible 2D orientations of the mino
  orientations2d.push_back(coordinates);
  nOrientations2d++;
  vector<vector<float>> newTrans;
  for(int s=0;s<n2d;s++){
    for(uint i=0;i<orientations2d.size();i++) {
      newTrans = transformations2d[s](orientations2d[i]);
      if(notIn(orientations2d,newTrans)) {
        orientations2d.push_back(newTrans);
        nOrientations2d++;
      }
    }
  }
  // Next sort orientations uniquely
  sort(orientations2d.begin(),orientations2d.end(),vectorOrder<float>);
  // Store the unique representant and update indices
  coordinates = orientations2d[0];
  updateIndices();
  // Compute the 3d possible orientations
  for(uint i=0;i<orientations2d.size();i++) {
    for(int s=0;s<n3d;s++){
      newTrans = transformations3dStraight[s](orientations2d[i]);
      if(notIn(orientations2d,newTrans) &
         notIn(orientations3dStraight,newTrans) &
         notIn(orientations3dDiagged,newTrans)) {
        orientations3dStraight.push_back(newTrans);
        nOrientations3dStraight++;
      }
      newTrans = transformations3dDiagged[s](orientations2d[i]);
      if(notIn(orientations2d,newTrans) &
         notIn(orientations3dStraight,newTrans) &
         notIn(orientations3dDiagged,newTrans)) {
        orientations3dDiagged.push_back(newTrans);
        nOrientations3dDiagged++;
      }
    }
  }
  // and sort them
  sort(orientations3dStraight.begin(),orientations3dStraight.end(),vectorOrder<float>);
  sort(orientations3dDiagged.begin(),orientations3dDiagged.end(),vectorOrder<float>);
  return;
}

void mino::computeAllPositions(vector<vector<float>> &grid,vector<vector<int>> &positions,int minoIndex){
  // First sort the grid by lexicographical order and get its extreme values
  sort(grid.begin(),grid.end(),cellOrder<float>);
  int gLines = grid.size(),gCols = grid[0].size();
  vector<vector<float>> grid_t(gCols,vector<float>(gLines));
  for(int i=0;i<gLines;i++){
    for(int j=0;j<gCols;j++) grid_t[j][i] = grid[i][j];
  }
  for(int i=0;i<gCols;i++) {
    sort(grid_t[i].begin(),grid_t[i].end());
    auto last = unique(grid_t[i].begin(),grid_t[i].end());
    grid_t[i].erase(last,grid_t[i].end());
  } // grid_t now contains all the possible values for x, y an z, in order
  // The search of possible position will be done in the box-enveloppe of the grid

  vector<vector<float>> orientation;
  vector<int> position(n+1);
  position[n] = minoIndex;
  vector<float> coords(3);
  vector<float> shift(3);
  int j;
  contribute(-1);// mise-à-zéro des compteurs
  for(int orien=0;orien<number();orien++){
    orientation = getOrientation(orien);
    for(uint iz=0;iz<grid_t[2].size();iz++){
      shift[2] = grid_t[2][iz]-orientation[0][2];
      for(uint iy=0;iy<grid_t[1].size();iy++){
        shift[1] = grid_t[1][iy]-orientation[0][1];
        for(uint ix=0;ix<grid_t[0].size();ix++){
          shift[0] = grid_t[0][ix]-orientation[0][0];
          for(j=0;j<n;j++){
            for(d=0;d<3;d++) coords[d] = orientation[j][d]+shift[d];
            position[j] = trouveCell(coords,grid);
            if(position[j]<0) break;
          }
          if(j==n) {
            positions.push_back(position);
            contribute(orien);
          }
        }
      }
    }
  }
}

/* this function finds the index of a cell within a grid.
   if cell is not found, return -1 */
int trouveCell(const vector<float> &coords,const vector<vector<float>> &grid){
  int gridLen = grid.size();
  for(int i=0;i<gridLen;i++){
    if(fabs(coords[2]-grid[i][2])<ZERO){
      if(fabs(coords[1]-grid[i][1])<ZERO){
        if(fabs(coords[0]-grid[i][0])<ZERO){
          return i;
        }
      }
    }
  }
  return -1;
}

/* return the maximal value of a vector of vector on the dir column */
float dirMax(const vector<vector<float>> &coords,int dir){
  float ans = coords[0][dir];
  for(uint i=1;i<coords.size();i++) ans = max(ans,coords[i][dir]);
  return ans;
}

/* return the minimal value of a vector of vector on the dir column */
float dirMin(const vector<vector<float>> &coords,int dir){
  float ans = coords[0][dir];
  for(uint i=1;i<coords.size();i++) ans = min(ans,coords[i][dir]);
  return ans;
}

/* Transformations of a mino */
// Shift back to smallest positive numbers
vector<vector<float>> & shift(vector<vector<float>> & coords){
  int n = coords.size();
  float xMin = dirMin(coords,0);
  float yMin = dirMin(coords,1);
  float zMin = dirMin(coords,2);
  for(int i=0;i<n;i++){
    coords[i][0] -= xMin;
    coords[i][1] -= yMin;
    coords[i][2] -= zMin;
  }
  return coords;
}
// sort cells by lexicographical order
vector<vector<float>> & reorder(vector<vector<float>> &coords){
  sort(coords.begin(),coords.end(),cellOrder<float>);
  return coords;
}

/* Horizontal symetry */
vector<vector<float>> hSymetry(const vector<vector<float>> & mino_coords){
  int n = mino_coords.size();
  vector<vector<float>> hMino(mino_coords);
  for(int i=0;i<n;i++) {
    hMino[i][1] = -hMino[i][1];
  }
  return reorder(shift(hMino));
}
/* Vertical symetry */
vector<vector<float>> vSymetry(const vector<vector<float>> & mino_coords){
  int n = mino_coords.size();
  vector<vector<float>> vMino(mino_coords);
  for(int i=0;i<n;i++) {
    vMino[i][0] = -vMino[i][0];
  }
  return reorder(shift(vMino));
}
/* x,y coordinates swap. First bissectrice symetry */
vector<vector<float>> xSymetry(const vector<vector<float>> & mino_coords){
  int n = mino_coords.size();
  vector<vector<float>> xMino(mino_coords);
  for(int i=0;i<n;i++) {
    xMino[i][0] = mino_coords[i][1];
    xMino[i][1] = mino_coords[i][0];
  }
  return reorder(shift(xMino));
}

/* Ox π/2 rotation */
vector<vector<float>> xRotation(const vector<vector<float>> & mino_coords){
  int n = mino_coords.size();
  vector<vector<float>> xMino(mino_coords);
  for(int i=0;i<n;i++) {
    xMino[i][1] = -mino_coords[i][2];
    xMino[i][2] =  mino_coords[i][1];
  }
  return reorder(shift(xMino));
}
/* Oy π/2 rotation */
vector<vector<float>> yRotation(const vector<vector<float>> & mino_coords){
  int n = mino_coords.size();
  vector<vector<float>> xMino(mino_coords);
  for(int i=0;i<n;i++) {
    xMino[i][0] =  mino_coords[i][2];
    xMino[i][2] = -mino_coords[i][0];
  }
  return reorder(shift(xMino));
}
/* (Ox+Oy) π/2 rotation */
vector<vector<float>> xpyRotation(const vector<vector<float>> & mino_coords){
  int n = mino_coords.size();
  float s = 0.5*sqrt(2.0);
  vector<vector<float>> xMino(mino_coords);
  for(int i=0;i<n;i++) {
    xMino[i][0] = 0.5*( mino_coords[i][0]+mino_coords[i][1])-s*mino_coords[i][2];
    xMino[i][1] = 0.5*( mino_coords[i][0]+mino_coords[i][1])+s*mino_coords[i][2];
    xMino[i][2] =  s *( mino_coords[i][0]-mino_coords[i][1]);
  }
  return reorder(shift(xMino));
}
/* (Ox-Oy) π/2 rotation */
vector<vector<float>> xmyRotation(const vector<vector<float>> & mino_coords){
  int n = mino_coords.size();
  float s = 0.5*sqrt(2.0);
  vector<vector<float>> xMino(mino_coords);
  for(int i=0;i<n;i++) {
    xMino[i][0] = 0.5*( mino_coords[i][0]-mino_coords[i][1])-s*mino_coords[i][2];
    xMino[i][1] = 0.5*(-mino_coords[i][0]+mino_coords[i][1])-s*mino_coords[i][2];
    xMino[i][2] =  s *( mino_coords[i][0]+mino_coords[i][1]);
  }
  return reorder(shift(xMino));
}

template<typename T>
bool vectorOrder(const vector<vector<T>> &v1,const vector<vector<T>> &v2) {
  assert(v1.size() == v2.size());
  assert(v1[0].size() == v2[0].size());
  int n = v1.size(), d = v1[0].size();
  for(int i=n-1;i>=0;i--){
    for(int j=d-1;j>=0;j--){
      if(fabs(v1[i][j]-v2[i][j])>ZERO){
        return v1[i][j]<v2[i][j];
      }
    }
  }
  cout<<"The two vectors are equal\n";
  cout<<v1<<endl;
  cout<<v2<<endl;
  assert(false);
}

template<typename T>
bool cellOrder(const vector<T> &c1,const vector<T> &c2) {
  assert(c1.size() == c2.size());
  int d = c1.size();
  for(int j=d-1;j>=0;j--){
    if(fabs(c1[j]-c2[j])>ZERO){
      return c1[j]<c2[j];
    }
  }
  cout<<"The two cells are equal\n";
  cout<<c1<<endl;
  cout<<c2<<endl;
  assert(false);
}

template<typename T>
void transpose(const vector<vector<T>> &vec){
  int n = vec.size(),p=vec[0].size();
}

bool areEqual(const vector<vector<float>> &v1, const vector<vector<float>> &v2){
  assert(v1.size()==v2.size());
  int n = v1.size();
  for(int i=0;i<n;i++){
    assert(v1[i].size()==v2[i].size());
    for(uint j=0;j<v1[i].size();j++){
      if(fabs(v1[i][j]-v2[i][j])>ZERO) return false;
    }
  }
  return true;
}

bool notIn(const vector<vector<vector<float>>> &os, const vector<vector<float>> &newOs){
  for(uint j=0;j<os.size();j++) {
    if(areEqual(os[j],newOs)) return false;
  }
  return true;
}
