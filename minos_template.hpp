#include <vector>

using namespace std;

template<int n,int d=2> class mino {
private:
  unt indices[n]; // Indices of the mino in a (n+1)x(n+1) grid
  int coordinates[n][d]; // Coordinates of the mino in d dimensions
  vector<vector<int>> orientations; // different orientations of the mino
  int nOrientations; // number of different orientations
public:
  /* constructors and destructor */
  mino() {n=0;d=2;nOrientations=0;};
  mino(vector<int> &_indices);
  ~mino() {};

  int size() {return n;};
  vector<int> & indices() {return *indices;};
  vector<vector<int>> & coordinates() {return *coordinates;};
  vector<vector<int>> & orientations() {return *orientations;};

  /* update mino coordinates from its indices (given in grid (n+1)x(n+1)) */
  void updateCoordinates();
  /* update mino indices from its coordinates.
     Indices are given in an (n+1)x(n+1) grid */
  void updateIndices();
  /* compute all the different orientation of the n-mino */
  void computeOrientations();

  /* shift mino as left and up as possible */
  void shift();
  /* reorder mino cells by lexicagraphical order
     (or by indices order ) if you prefer... */
  void reorder();

  /* Horizontal symetry */
  mino hSymetry();
  /* Vertical symetry */
  mino vSymetry();
  /* x,y coordinates swap. First bissectrice symetry */
  mino xSymetry();
};
