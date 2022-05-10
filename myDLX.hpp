#ifndef DLX_H
#define DLX_H

#include <vector>
#include <string.h>
#include <iostream>
#include <fstream>

using namespace std;

class dlx_cell {
public:
  dlx_cell *L=nullptr,*R=nullptr;// left and right cells
  dlx_cell *U=nullptr,*D=nullptr;// up and down nCells
  dlx_cell *C=nullptr; // column header
  int value=-1;

  dlx_cell(){};
  ~dlx_cell(){};
};

dlx_cell * buildStructure(vector<vector<int>> &Y);
void cover(dlx_cell *c);
void uncover(dlx_cell *c);
void disableRow(dlx_cell *r);
int solve(dlx_cell *head,vector<dlx_cell *> &solution,int depth);
void removeUselessPositions(dlx_cell *head);
void outputSolution(vector<dlx_cell*> &solution);


/* GLOBAL VARIABLES */
extern int nCells,nMinos;
extern ofstream outFile;
extern string outFormat;
/* Track counters */
#ifdef TRACK
extern unsigned long visitedNodes,covered,uncovered;
extern unsigned long totalVisited,totalCovered,totalUncovered;
extern ofstream outTrack;
extern char sep;
#endif

#endif // DLX_H
