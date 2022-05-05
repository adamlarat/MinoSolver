#include "mino.hpp"
#include "tools.hpp"
#include "dlx.hpp"



map<char,vector<int>> minos = {
    //Pentaminos
    {'F', {1,6,7,8,12}},
    {'I', {0,1,2,3, 4}},
    {'L', {0,1,2,3, 6}},
    {'N', {1,2,3,6, 7}},
    {'P', {0,1,2,6, 7}},
    {'T', {0,6,7,8,12}},
    {'U', {0,1,2,6, 8}},
    {'V', {0,1,2,6,12}},
    {'W', {1,2,6,7,12}},
    {'X', {1,6,7,8,13}},
    {'Y', {0,1,2,3, 7}},
    {'Z', {2,6,7,8,12}},
    // Tetraminos
    {'i', {0,1,2,3}},
    {'l', {0,1,2,5}},
    {'n', {1,2,5,6}},
    {'o', {0,1,5,6}},
    {'t', {0,1,2,6}},
    // Triminos
    {'<', {0,1,4}},
    {'|', {0,1,2}},
    // Domino
    {'-', {0,1}},
    // Unimino
    {'.', {0}}
};

int main(){
  /*
  for(auto minoList = minos.begin();minoList != minos.end();minoList++){
    mino myMino(minoList->second);
    cout<<"Mino "<<minoList->first<<": has "<<myMino.number()<<" different orientations\n";
    cout<<"Principal representant: "<<myMino.getIndices()<<endl;
    vector<vector<int>> allOrientations = myMino.getOrientationsAsIndices();
    for(int i=0;i<myMino.number();i++){
      cout<<" * Orientation "<<i<<": "<<allOrientations[i]<<endl;
    }
  }
  */
  string minoList = "FILNPTUVWXYZ";
  int grid[2] = {3,20};
  int nCells = grid[0]*grid[1];
  int nMinos = minoList.size();
  vector<mino> minoArray;
  typeY Y;
  for(int i=0;i<nMinos;i++){
    mino nextMino(minos[minoList[i]]);
    minoArray.push_back(nextMino);
    nextMino.computeAllPositions(grid,Y,i+nCells);
  }
  typeX X;
  getDLXDataStructure(X,Y);
  vector<int> solutions(nMinos);
  solve(X,Y,solutions,0);
  return 0;
}
