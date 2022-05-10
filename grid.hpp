#ifndef GRID_H
#define GRID_H

#include <vector>
#include <string>
#include <math.h>

float s = 0.5*sqrt(2.0);

vector<vector<int>> KnuthExample = {
  {2,4,5},
  {0,3,6},
  {1,2,5},
  {0,3},
  {1,6},
  {3,4,6}
};

vector<vector<float>> gridReader(string arg_str,int &nCells){
  int i = 0;
  vector<vector<float>> grid;
  if(arg_str.substr(0,8)=="pyramide"){
    int pyramideSize = stoi(arg_str.substr(8));
    nCells = ((2*pyramideSize+1)*(pyramideSize+1)*pyramideSize)/6;
    grid = vector<vector<float>> (nCells,vector<float>(3,0.0));
    for(int iz=0;iz<pyramideSize;iz++){
      for(int iy=0;iy<pyramideSize-iz;iy++){
        for(int ix=0;ix<pyramideSize-iz;ix++){
          grid[i][0] = 0.5*iz+ix;
          grid[i][1] = 0.5*iz+iy;
          grid[i][2] = iz*s;
          i++;
        }
      }
    }
  }
  else if(arg_str.substr(0,4)=="grid"){
    vector<int> gridSize(3,1);
    nCells = 1;
    size_t pos=0;
    string subStr = arg_str.substr(4);
    int j=0;
    while( (pos=subStr.find("x")) != string::npos){
      gridSize[j] = stoi(subStr.substr(0,pos));
      subStr.erase(0,pos+1);
      nCells *= gridSize[j];
      j++;
    }
    gridSize[j] = stoi(subStr.substr(0,pos));
    nCells *= gridSize[j];
    grid = vector<vector<float>> (nCells,vector<float>(3,0.0));
    for(int iz=0;iz<gridSize[2];iz++){
      for(int iy=0;iy<gridSize[1];iy++){
        for(int ix=0;ix<gridSize[0];ix++){
          grid[i][0] = ix;
          grid[i][1] = iy;
          grid[i][2] = iz;
          i++;
        }
      }
    }
  }
  else {
    cout<<"Accepted token for the grid generations are:\n";
    cout<<"pyramideN, gridXxY[xZ]";
    cout<<"Come back soon!";
    exit(-2);
  }
  return grid;
}
#endif // GRID_H
