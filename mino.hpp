#include <vector>

using namespace std;

class mino {
private:
  int n=0; // mino size (known as n-mino)
  int d=2; // dimension (2 for the moment)
  vector<int> indices; // Indices of the mino in a (n+1)x(n+1) grid
  vector<vector<int>> coordinates; // Coordinates of the mino in d dimensions
  vector<vector<vector<int>>> orientations; // different orientations of the mino
  int nOrientations=0; // number of different orientations
public:
  /* constructors and destructor */
  mino() {}
  mino(vector<int> _indices,int _d=2);
  ~mino() {}

  int size() {return n;}
  int dimensions() {return d;}
  int number() {return nOrientations;}
  vector<int> & getIndices() {return indices;}
  vector<vector<int>> & getCoordinates() {return coordinates;}
  vector<vector<vector<int>>> & getOrientations() {return orientations;}
  vector<vector<int>> getOrientationsAsIndices();

  /* update mino coordinates from its indices (given in grid (n+1)x(n+1)) */
  void updateCoordinates();
  /* update mino indices from its coordinates.
     Indices are given in an (n+1)x(n+1) grid */
  void updateIndices();
  /* compute all the different orientation of the n-mino */
  void computeOrientations();
  /* given a grid, compute all the positions of all the orientations
     within the grid */
  int computeAllPositions(vector<int> grid,vector<vector<int>> &positions,int minoIndex);

};
