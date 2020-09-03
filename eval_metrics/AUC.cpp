#include "Recall.h"

#include <iostream>
#include <algorithm>
#include <numeric>

using namespace std;

double auc( vector<string>& retrieved, vector<string>& relevants, int nitems )
{
   int hit = 0;
   int miss = 0;
   double auc = 0;
   int K = retrieved.size();
   int num_pos_items = relevants.size();
   int num_neg_items = nitems - num_pos_items;
   for( int i = 0 ; i < K ; ++i )
   {
      if( find( relevants.begin(), relevants.end(), retrieved[i] ) != relevants.end() )
      {
         hit += 1;
      }
      else
      {
         miss += 1;
         auc += hit;
      }
   }
   auc += ((hit + num_pos_items) / 2.0) * (num_neg_items - miss);
   auc = auc / (num_pos_items * num_neg_items);
   return auc;
}


