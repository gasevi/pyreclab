#ifndef __MAX_HEAP_H__
#define __MAX_HEAP_H__

#include <utility>
#include <vector>
#include <stddef.h>

class MaxHeap
{
public:

   void push( std::pair<double, size_t>& e );

   std::pair<double, size_t> pop();

   bool empty();

   size_t size();

private:

   std::vector< std::pair<double, size_t> > m_vector;

};

#endif // __MAX_HEAP_H__

