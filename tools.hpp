#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

template <class T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vect) {
   out << "[";
   uint i=0;
   for(i=0;i<vect.size()-1;i++){
     out<<setw(2)<<vect[i]<<",";
   }
   out<<setw(2)<<vect[i]<<"]";
   return out;
}

#ifndef ZERO
  #define ZERO 1.0e-6
#endif

#endif // TOOLS_H
