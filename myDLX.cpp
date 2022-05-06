#include "myDLX.hpp"
#include "tools.hpp"

dlx_cell * buildStructure(vector<vector<int>> &Y){
  int sizeY = 0;
  for(vector<int> subY : Y) sizeY += subY.size();
  dlx_cell *head = new dlx_cell[sizeY+::nCells+::nMinos+1];
  // Set head
  dlx_cell *cell_ptr = head;
  cell_ptr->value = -(::nCells+::nMinos);
  cell_ptr->R     = cell_ptr+1;
  // Now setting column headers
  for(int i=0;i<::nCells+::nMinos;i++){
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
  return head;
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

void outputSolution(dlx_cell *head, vector<dlx_cell*> &solution){
  dlx_cell *cell_ptr = head->R;
  vector<int> out(::nCells);
  int mino_type=-1;
  for(uint i=0;i<solution.size();i++){
    cell_ptr = solution[i];
    while(cell_ptr->value<::nCells) cell_ptr = cell_ptr->R;
    mino_type = cell_ptr->value-::nCells+1;
    cell_ptr = cell_ptr->R;
    while(cell_ptr->value<::nCells) {
      out[cell_ptr->value]=mino_type;
      cell_ptr = cell_ptr->R;
    }
  }
  if(outFormat=="bin"){
    for(uint i=0;i<out.size();i+=4){
      ::outFile<<(uint8_t)(out[i+2]*16+out[i+3]);
      ::outFile<<(uint8_t)(out[i]*16+out[i+1]);
    }
  }
  else{
    for(uint i=0;i<out.size();i++){
      if(out[i]<10) ::outFile<<out[i];
      else ::outFile<<char(97+out[i]-10);
    }
    ::outFile<<endl;
  }
}

int solve(dlx_cell *head,vector<dlx_cell *> &solution,int depth){
  int nSolTot = 0,nSol;
  dlx_cell *cell_ptr=head->R;

  /* if graph is void, output and return one found solution */
  if(cell_ptr == head){
#ifndef FAST
    if (::outFile.is_open()) outputSolution(head,solution);
#endif
    return 1;
  }

  /* search for the smallest column */
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
#ifndef FAST
  if (depth == 0) {
    cout<<"First level has "<<s<<" nodes\n";
    s = 1;
  }
#endif

  /* cover the smallest column */
  cover(head,c);
  /* loop over the column to run over all possibilities */
  dlx_cell *r = c->D,*j;
  while(r != c){
    /* choose position r for solution */
    solution[depth] = r;

    /* cover all the column corresponding to the
       cells occupied by r */
    j = r->R;
    while(j != r){
      cover(head,j->C);
      j = j->R;
    }
    /* look for next position */
    nSol = solve(head,solution,depth+1);
    nSolTot += nSol;
#ifndef FAST
    if(depth == 0){
      cout<<" * Level "<<s++<<": "<<nSol<<" solutions\n";
    }
#endif

    /* undo everything in order to put graph into
       its initial state */
    solution[depth] = nullptr;
    j = r->L;
    while(j != r){
      uncover(head,j->C);
      j = j->L;
    }
    r = r->D;
  }

  /* uncover chosen column and leave */
  uncover(head,c);
  return nSolTot;
}
