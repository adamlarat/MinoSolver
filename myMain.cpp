#include "mino.hpp"
#include "tools.hpp"
#include "myDLX.hpp"
#include <map>
#include <string>
#include <iomanip>
#include "grid.hpp"

/* GLOBAL VARIABLES */
char* minosChar;
int nMinos = 0;
int nCells = 1;
ofstream outFile;
string outFormat;
/* Track variables */
#ifdef TRACK
unsigned long visitedNodes=0,covered=0,uncovered=0;
unsigned long totalVisited=0,totalCovered=0,totalUncovered=0;
ofstream outTrack;
char sep=' ';
#endif
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
/*
  ::nCells = 5;
  ::nMinos = 2;
  dlx_cell *tete = buildStructure(KnuthExample);
  vector<dlx_cell *> sol(3);
  removeUselessPositions(tete);
  int un = solve(tete,sol,0);
  return un;
*/
  clock_t debut,config,fin;
  debut = clock();

  /* Read arguments */
  if(argc<3){
    cout<<"Welcome to this puzzle solver!"<<endl;
    cout<<" Syntax : mino.exe minosChar pyramideN|gridXxY[xZ] [filename]"<<endl;
    cout<<" Example: mino.exe 'FILNPTUVWXYZ' grid3x20"<<endl;
    cout<<"Come back later!"<<endl;
    return -3;
  }
  ::minosChar = argv[1];
  ::nMinos      = strlen(::minosChar);
  string gridString(argv[2]);
  vector<vector<float>> grid = gridReader(gridString,::nCells);
  /*
  char* gridReader= strtok(argv[2],"x");
  int gridSize;
  while(gridReader != NULL){
    gridSize = stoi(gridReader);
    ::grid.push_back(gridSize);
    ::nCells *= gridSize;
    gridReader = strtok(NULL,"x");
  }
  */
#ifndef FAST
  cout<<"Arguments: "<<::minosChar<<" "<<argv[2]<<endl;
  cout<<" * nMinos: "<<::nMinos<<endl;
  cout<<" * nCells: "<<::nCells<<endl;
#endif
  if(argc>3){
    string filename(argv[3]);
    string extension = filename.substr(filename.find_last_of(".")+1);
    ::outFormat = (extension=="bin")?"bin":"txt";
    ::outFile.open(&filename[0],(extension=="bin")?ofstream::binary:ofstream::out);
#ifndef FAST
    cout<<" * Filename: "<<filename<<", extension: "<<extension<<endl;
#endif
  }
#ifdef TRACK
  ::outTrack.open("TrackTree.txt",ofstream::out);
#endif

  /* Comupute the structures */
  vector<mino> minoArray;
  vector<vector<int>> Y;
  int minosSize = 0;
  int countPositions = 0;
  for(int i=0;i<::nMinos;i++){
    mino nextMino(::minos[::minosChar[i]]);
    minosSize += nextMino.size();
    nextMino.computeAllPositions(grid,Y,i+::nCells);
    minoArray.push_back(nextMino);
#ifndef FAST
    cout<<"Mino : "<<minosChar[i]<<" with "<<nextMino.size()<<" cells.";
    cout<<" M0 = "<<nextMino.getIndices();
    cout<<", n2d = "<<setw(2)<<nextMino.number2d();
    cout<<", n3dS = "<<setw(2)<<nextMino.number3dStraight();
    cout<<", n3dD = "<<setw(2)<<nextMino.number3dDiagged();
    cout<<", nPos2d = "<<setw(3)<<nextMino.nPositions2d;
    cout<<", nPos3dS = "<<setw(3)<<nextMino.nPositions3dStraight;
    cout<<", nPos3dD = "<<setw(3)<<nextMino.nPositions3dDiagged;
    cout<<", nPos = "<<setw(3)<<Y.size()-countPositions<<endl;
#endif
    countPositions = Y.size();
  }
  if(minosSize != ::nCells){
    cout<<"You are using the wrong number of minos, or of wrong size!\n";
    cout<<" * grid number of cells: "<<::nCells<<endl;
    cout<<" * sum of minos sizes  : "<<minosSize<<endl;
    return -1;
  }
#ifndef FAST
  cout<<"Found "<<Y.size()<<" different positions covering the grid\n";
#endif

  dlx_cell *head = buildStructure(Y);

  // Compute all the solutions
  vector<dlx_cell *> solutions(::nMinos);
  config = clock();
  removeUselessPositions(head);
  int nSolutions = solve(head,solutions,0);
  fin    = clock();

  /* Final output */
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

  ::outFile.close();
  delete[] head;
  return 0;
}
