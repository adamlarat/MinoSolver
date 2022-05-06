#include <vector>

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

void buildStructure(vector<vector<int>> &Y,dlx_cell *head,int nCells,int nMinos);
void cover(dlx_cell *head,dlx_cell *c);
void uncover(dlx_cell *head,dlx_cell *c);
int solve(dlx_cell *head,vector<dlx_cell *> &solution,int depth);