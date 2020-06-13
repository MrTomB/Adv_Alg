/*-----------------------------
Thomas Burch (tburch1@ucsc.edu)
CMPS 109 Program 2
Overloading and operators
7/14/15
------------------------------*/

//
// Assuming that for any given type T, there are operators
// bool operator< (const T&, const T&);
// bool operator== (const T&, const T&);
// as fundamental comparisons for type T, define the other
// six operators in terms of the basic ones.
//

#ifndef __REL_OPS_H__
#define __REL_OPS_H__

template <typename value>
inline bool operator!= (const value &left, const value &right) {
   return not (left == right); 
}

template <typename value>
inline bool operator>  (const value &left, const value &right) {
   return right < left; 
}

template <typename value>
inline bool operator<= (const value &left, const value &right) {
   return not (right < left);
}

template <typename value>
inline bool operator>= (const value &left, const value &right) {
   return not (left < right); 
}

#endif