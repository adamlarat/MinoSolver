#include "mino.hpp"
#include "tools.hpp"
#include "myDLX.hpp"
#include <map>

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
  /* DEBUG!!!
  vector<vector<int>> mat{{2,4,5},{0,3,6},{1,2,5},{0,3},{1,6},{3,4,6}};
  int matSize = 16;
  int nCols = 7, nRows = 6;
  dlx_cell tete[matSize+nCols+1];
  buildStructure(mat,tete,nCols,0);
  vector<dlx_cell *> sols(nRows);
  solve(tete,sols,0);
  return 0;
  */


  string minoList = "FILNPTUVWXYZ";
  int grid[2] = {3,20};
  int nCells = grid[0]*grid[1];
  int nMinos = minoList.size();
  vector<mino> minoArray;
  vector<vector<int>> Y;
  int sizeY = 0;
  for(int i=0;i<nMinos;i++){
    mino nextMino(minos[minoList[i]]);
    minoArray.push_back(nextMino);
    sizeY += nextMino.computeAllPositions(grid,Y,i+nCells);
  }
  dlx_cell head[sizeY+nMinos+nCells+1];
  buildStructure(Y,head,nCells,nMinos);
  vector<dlx_cell *> solutions(nMinos);
  solve(head,solutions,0);
  return 0;
}
