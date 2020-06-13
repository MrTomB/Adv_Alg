/* 
Thomas Burch (1314305)
tburch1@ucsc.edu
7/27/15
*/

#ifndef __XLESS_H__
#define __XLESS_H__

//
// We assume that the type type_t has an operator< function.
//

template <typename Type>
struct xless {
   bool operator() (const Type& left, const Type& right) const {
      return left < right;
   }
};

#endif
