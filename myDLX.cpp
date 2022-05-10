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

void cover(dlx_cell *c){
  c->R->L = c->L;
  c->L->R = c->R;
#ifdef TRACK
  ::covered++;
#endif
  dlx_cell *i = c->D;
  dlx_cell *j;
  while(i != c){
    j = i->R;
    while(j != i){
      j->D->U = j->U;
      j->U->D = j->D;
      j->C->value--;
      j = j->R;
#ifdef TRACK
      ::covered++;
#endif
    }
    i = i->D;
  }
}

void uncover(dlx_cell *c){
  dlx_cell *i = c->U, *j;
  while(i != c){
    j = i->L;
    while(j != i){
      j->C->value++;
      j->D->U = j;
      j->U->D = j;
      j = j->L;
#ifdef TRACK
      ::uncovered++;
#endif
    }
    i = i->U;
  }
  c->R->L = c;
  c->L->R = c;
#ifdef TRACK
  ::uncovered++;
#endif
}

void disableRow(dlx_cell *r){
  dlx_cell *j = r->R;
  while(j != r) {
    j->U->D = j->D;
    j->D->U = j->U;
    j->C->value--;
    j = j->R;
  }
  r->U->D = r->D;
  r->D->U = r->U;
  r->C->value--;
  return;
}

void outputSolution(vector<dlx_cell*> &solution){
  dlx_cell *cell_ptr;
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

void removeUselessPositions(dlx_cell *head){
  dlx_cell *mino_ptr=head->L; //Point on last mino index
  dlx_cell *r,*j;
  vector<dlx_cell *> disabled_lines;
  for(int m=0;m<::nMinos;m++){
    cover(mino_ptr);
    r = mino_ptr->D;
    while(r != mino_ptr){
      /* cover all the column corresponding to the
         cells occupied by r */
      j = r->R;
      while(j != r){
        cover(j->C);
        j = j->R;
      }
      /* look for minimal column. If empty, position r is useless.*/
      dlx_cell *cell_ptr = head->R;
      while(cell_ptr != head){
        if(cell_ptr->value == 0){
          disabled_lines.push_back(r);
          break;
        }
        cell_ptr = cell_ptr->R;
      }
      /* undo everything in order to put graph into
         its initial state */
      j = r->L;
      while(j != r){
        uncover(j->C);
        j = j->L;
      }
      r = r->D;
    }
    uncover(mino_ptr);
    mino_ptr = mino_ptr->L;
  }
  for(dlx_cell *row : disabled_lines){
    disableRow(row);
  }
#ifndef FAST
  cout<<"Removed "<<disabled_lines.size()<<" useless positions\n";
#endif
}

int solve(dlx_cell *head,vector<dlx_cell *> &solution,int depth){
  int nSolTot = 0,nSol;
  dlx_cell *cell_ptr=head->R;

  /* if graph is void, output and return one found solution */
  if(cell_ptr == head){
#ifndef FAST
    if (::outFile.is_open()) outputSolution(solution);
#endif
#ifdef TRACK
    ::outTrack<<string(depth,::sep)<<"child {node {$1$}}\n";
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
#ifdef TRACK
if(s == 0) ::outTrack<<string(depth,::sep)<<"child {node {$0$}}\n";
else ::outTrack<<string(depth,::sep)<<"child {node {$"<<s<<"$}\n"; //"["<<s<<",\n";
#endif
#ifndef FAST
  if (depth == 0) {
    cout<<"First level has "<<s<<" nodes. Column "<<c->D->value<<"\n";
    s = 0;
  }
#endif

  /* cover the smallest column */
  cover(c);
  /* loop over the column to run over all possibilities */
  dlx_cell *r = c->D,*j;
  while(r != c){
    /* choose position r for solution */
    solution[depth] = r;
#ifdef TRACK
    if(depth == 0){
      ::visitedNodes = 0;
      ::covered      = 0;
      ::uncovered    = 0;
    }
    ::visitedNodes++;
#endif

    /* cover all the column corresponding to the
       cells occupied by r */
    j = r->R;
    while(j != r){
      cover(j->C);
      j = j->R;
    }
    /* look for next position */
    nSol = solve(head,solution,depth+1);
    nSolTot += nSol;

    /* undo everything in order to put graph into
       its initial state */
    solution[depth] = nullptr;
    j = r->L;
    while(j != r){
      uncover(j->C);
      j = j->L;
    }
#ifndef FAST
    if(depth == 0){
      cout<<" * Level "<<setw(3)<<++s<<": "<<setw(6)<<nSol<<" solutions";
#ifdef TRACK
      cout<<", "<<setw(8)<<::visitedNodes<<" nodes visited";
      cout<<", "<<setw(12)<<::covered<<" cells updated";
      ::totalVisited += ::visitedNodes;
      ::totalCovered += ::covered;
      ::totalUncovered += ::uncovered;
#endif
      cout<<endl;
    }
#endif
    r = r->D;
  }
#ifdef TRACK
  if(depth == 0){
    cout<<"-----------------------------------------------------------------------------------\n";
    cout<<"    TOTAL   : "<<setw(6)<<nSolTot<<" solutions";
    cout<<", "<<setw(8)<<::totalVisited<<" nodes visited";
    cout<<", "<<setw(12)<<::totalCovered<<" cells updated";
    cout<<endl;
    cout<<"-----------------------------------------------------------------------------------\n";
  }
#endif
#ifdef TRACK
if(s>0) ::outTrack<<string(depth,::sep)<<"}\n"; //"]\n";
#endif
  /* uncover chosen column and leave */
  uncover(c);
  return nSolTot;
}
