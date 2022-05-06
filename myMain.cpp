#include "mino.hpp"
#include "tools.hpp"
#include "myDLX.hpp"
#include <map>
#include <string.h>
#include <iomanip>

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

int main(int argc,char *argv[]){

  clock_t debut,config,fin;
  debut = clock();

  /* Read arguments */
  if(argc<3){
    cout<<"Welcome to this puzzle solver!"<<endl;
    cout<<" Syntax : mino.exe minosChar (gridLines)x(gridCols) [filename output_format]"<<endl;
    cout<<" Example: mino.exe 'FILNPTUVWXYZ' 3x20"<<endl;
    cout<<"Come back later!"<<endl;
    return -3;
  }
  char* minosChar = argv[1];
  int nMinos      = strlen(minosChar);
  char* gridReader= strtok(argv[2],"x");
  vector<int> grid;
  int nCells = 1;
  while(gridReader != NULL){
    int gridSize = stoi(gridReader);
    grid.push_back(gridSize);
    nCells *= gridSize;
    gridReader = strtok(NULL,"x");
  }
  cout<<"Arguments: "<<minosChar<<" "<<grid<<endl;
  cout<<" * nMinos: "<<nMinos<<endl;
  cout<<" * nCells: "<<nCells<<endl;

  /* Comupute the structures */
  vector<mino> minoArray;
  vector<vector<int>> Y;
  int sizeY = 0;
  for(int i=0;i<nMinos;i++){
    mino nextMino(minos[minosChar[i]]);
    minoArray.push_back(nextMino);
    sizeY += nextMino.computeAllPositions(grid,Y,i+nCells);
  }
  dlx_cell head[sizeY+nMinos+nCells+1];
  buildStructure(Y,head,nCells,nMinos);

  /* Compute all the solutions */
  vector<dlx_cell *> solutions(nMinos);
  config = clock();
  int nSolutions = solve(head,solutions,0);
  fin    = clock();
  cout<<"------------------------------------\n";
  cout<<"Found "<<nSolutions<<" solutions in total!\n";
  cout<<"------------------------------------\n";
  cout<<"Temps d'exécution:\n";
  cout<<" * Configuration : "<<fixed<<double(config-debut)/double(CLOCKS_PER_SEC)<<setprecision(3);
  cout<<" s\n";
  cout<<" * Résolution    : "<<fixed<<double(fin-config)/double(CLOCKS_PER_SEC)<<setprecision(3);
  cout<<" s\n";
  cout<<"------------------------------------\n";
  cout<<" * Total         : "<<fixed<<double(fin-debut)/double(CLOCKS_PER_SEC)<<setprecision(3);
  cout<<" s\n";
  return 0;
}
