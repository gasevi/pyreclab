#include "Recall.h"

#include <iostream>
#include <algorithm>
#include <numeric>

using namespace std;

float recall( std::vector<std::string>& retrieved, std::vector<std::string>& relevant )
{
   float tp = 0;
   vector<string>::iterator ind;
   vector<string>::iterator end = retrieved.end();
   for( ind = retrieved.begin() ; ind != end ; ++ind )
   {
      if( find( relevant.begin(), relevant.end(), *ind ) != relevant.end() )
      {
         ++tp;
      }
   }
   return tp / relevant.size();
}


