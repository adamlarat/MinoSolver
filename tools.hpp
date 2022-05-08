#include <iostream>
#include <vector>

using namespace std;

template <class T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vect) {
   out << "[";
   uint i=0;
   for(i=0;i<vect.size()-1;i++){
     out<<vect[i]<<", ";
   }
   out<<vect[i]<<"]";
   return out;
}

#ifndef ZERO
  #define ZERO 1.0e-6
#endif
