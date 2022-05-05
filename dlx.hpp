#include <vector>
#include <map>
#include <list>
#include <algorithm>

using namespace std;

typedef map<int,list<int>> typeX;
typedef vector<vector<int>> typeY;
typedef typeX::iterator map_it;
typedef pair<int,list<int>> map_pair;
int solve(typeX &X,typeY &Y,vector<int> &solution,int depth);
void select(typeX &X,typeY &Y,int r,list<list<int>> &cols);
void deselect(typeX &X,typeY &Y,int r,list<list<int>> &cols);
void getDLXDataStructure(typeX &X,typeY &Y);
