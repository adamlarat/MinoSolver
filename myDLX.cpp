#include "myDLX.hpp"
#include "tools.hpp"
#include <vector>
#include <iostream>

void buildStructure(vector<vector<int>> &Y,dlx_cell *head,int nCells,int nMinos){
  // Set head
  dlx_cell *cell_ptr = head;
  cell_ptr->value = -nCells;
  cell_ptr->R     = cell_ptr+1;
  // Now setting column headers
  for(int i=0;i<nCells+nMinos;i++){
    cell_ptr++;
    cell_ptr->value = 0;
    cell_ptr->L     = cell_ptr-1;
    cell_ptr->R     = cell_ptr+1;
    cell_ptr->U = cell_ptr->D = cell_ptr->C = cell_ptr;
  }
  // Closing the loop
  cell_ptr->R = head;
  head->L = cell_ptr;
  // Now looping over the cells
  int rowSize;
  dlx_cell *col_head,*previous;
  cell_ptr++;
  for(uint i=0;i<Y.size();i++){
    rowSize = Y[i].size();
    // Set up row loop
    for(int j=0;j<rowSize;j++){
      // for some reason -1%N = -1 in C :-(
      cell_ptr[j].L = cell_ptr+(j-1+rowSize)%rowSize;
      cell_ptr[j].R = cell_ptr+(j+1)%rowSize;
      cell_ptr[j].value = Y[i][j];
      col_head = head+1+Y[i][j];
      previous = col_head->U;
      cell_ptr[j].U = previous;
      cell_ptr[j].D = col_head;
      cell_ptr[j].C = col_head;
      previous->D   = cell_ptr+j;
      col_head->U   = cell_ptr+j;
      col_head->value++;
    }
    cell_ptr += rowSize;
  }
  return;
}

void cover(dlx_cell *head,dlx_cell *c){
  c->R->L = c->L;
  c->L->R = c->R;
  dlx_cell *i = c->D;
  dlx_cell *j;
  while(i != c){
    j = i->R;
    while(j != i){
      j->D->U = j->U;
      j->U->D = j->D;
      j->C->value--;
      j = j->R;
    }
    i = i->D;
  }
}

void uncover(dlx_cell *head,dlx_cell *c){
  dlx_cell *i = c->U, *j;
  while(i != c){
    j = i->L;
    while(j != i){
      j->C->value++;
      j->D->U = j;
      j->U->D = j;
      j = j->L;
    }
    i = i->U;
  }
  c->R->L = c;
  c->L->R = c;
}
void outputMat(dlx_cell *head,vector<dlx_cell*> &solution){
  dlx_cell *cell_ptr,*c;
  cout<<"[";
  int i = 0;
  cell_ptr = solution[i++];
  while(cell_ptr){
    cout<<"["<<cell_ptr->value;
    c = cell_ptr->R;
    while(c != cell_ptr){
      cout<<","<<c->value;
      c = c->R;
    }
    cout<<"]";
    cell_ptr = solution[i++];
  }
  cout<<"]"<<endl;
}

void outputSolution(dlx_cell *head, vector<dlx_cell*> &solution){
  dlx_cell *cell_ptr = head->R;
  int nCells = -head->value;
  vector<int> out(nCells);
  int mino_type=-1;
  for(uint i=0;i<solution.size();i++){
    cell_ptr = solution[i];
    while(cell_ptr->value<nCells) cell_ptr = cell_ptr->R;
    mino_type = cell_ptr->value-nCells+1;
    cell_ptr = cell_ptr->R;
    while(cell_ptr->value<nCells) {
      out[cell_ptr->value]=mino_type;
      cell_ptr = cell_ptr->R;
    }
  }
  for(uint i=0;i<out.size();i++){
    if(out[i]<10) cout<<out[i];
    else cout<<char(97+out[i]-10);
  }
  cout<<endl;
}

int solve(dlx_cell *head,vector<dlx_cell *> &solution,int depth){
  int nSol = 0;
  dlx_cell *cell_ptr=head->R;
  if(cell_ptr == head){
    outputSolution(head,solution);
    //outputMat(head,solution);
    return 1;
  }
  int s = cell_ptr->value;
  dlx_cell *c = cell_ptr;
  cell_ptr = cell_ptr->R;
  while(cell_ptr != head){
    if(cell_ptr->value<s){
      s = cell_ptr->value;
      c = cell_ptr;
    }
    cell_ptr = cell_ptr->R;
  }
  cover(head,c);
  dlx_cell *r = c->D,*j;
  while(r != c){
    solution[depth] = r;
    j = r->R;
    while(j != r){
      cover(head,j->C);
      j = j->R;
    }
    nSol += solve(head,solution,depth+1);
    solution[depth] = nullptr;
    j = r->L;
    while(j != r){
      uncover(head,j->C);
      j = j->L;
    }
    r = r->D;
  }
  uncover(head,c);
  return nSol;
}