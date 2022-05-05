/* The smart part of the algorithm comes from
   https://www.cs.mcgill.ca/~aassaf9/python/algorithm_x.html
   I did some slight improvements for C++ */
#include "dlx.hpp"
#include "tools.hpp"

int solve(typeX &X,typeY &Y,vector<int> &solution,int depth){
  int nSol = 0;
  if(X.empty()){
    //cout<<"Trouvé une solution !"<<endl;
    int nCells = 0;
    for(uint i=0;i<solution.size();i++){
      for(uint j=0;j<Y[solution[i]].size()-1;j++){
        int cell = Y[solution[i]][j];
        nCells = nCells>cell?nCells:cell;
      }
    }
    nCells++;
    vector<int> out(nCells);
    for(uint i=0;i<solution.size();i++){
      int Ysize = Y[solution[i]].size();
      int type = Y[solution[i]][Ysize-1]-nCells+1;
      for(int j=0;j<Ysize-1;j++){
        out[Y[solution[i]][j]] = type;
      }
    }
    for(int i=0;i<nCells;i++){
      if(out[i]<10) cout<<out[i];//<<",";
      else cout<<char(97+out[i]-10);
    }
    cout<<endl;
    return 1;
  }
  map_it c = min_element(X.begin(),X.end(),[](map_pair a,map_pair b){
    return a.second.size()<b.second.size();
  });
  vector<int> Xc;
  for(int i : c->second) Xc.push_back(i);
  //for(int r : c->second){
  for(int r : Xc){
    solution[depth] = r;
    list<list<int>> cols;
    select(X,Y,r,cols);
    nSol += solve(X,Y,solution,depth+1);
    deselect(X,Y,r,cols);
    solution[depth] = 0;
  }
  return nSol;
}

void select(typeX &X,typeY &Y,int r,list<list<int>> &cols){
  for(int j : Y[r]) {
    for(int i : X[j]){
      for(int k : Y[i]){
        if(k!=j) X[k].remove(i);
      }
    }
    cols.push_back(move(X[j]));
    X.erase(j);
  }
  return;
}

void deselect(typeX &X,typeY &Y,int r,list<list<int>> &cols){
  reverse(Y[r].begin(),Y[r].end());
  reverse(cols.begin(),cols.end());
  list<list<int>>::iterator colj = cols.begin();
  for(int j : Y[r]){
    //X.insert({j,*colj});
    X[j] = *colj;
    colj++;
    for(int i : X[j]){
      for(int k : Y[i]){
        if(k != j) X[k].push_back(i);
      }
    }
  }
  reverse(Y[r].begin(),Y[r].end());
  return;
}

void getDLXDataStructure(typeX &X,typeY &Y){
  for(uint r=0;r<Y.size();r++){
    for(int Yr : Y[r]){
      if(!X.count(Yr)) X[Yr] = list<int>();
      X[Yr].push_back(r);
    }
  }
  return;
}
