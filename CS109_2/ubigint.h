/*-----------------------------
Thomas Burch (tburch1@ucsc.edu)
CMPS 109 Program 2
Overloading and operators
7/14/15
------------------------------*/

#ifndef __UBIGINT_H__
#define __UBIGINT_H__

#include <exception>
#include <iostream>
#include <limits>
#include <utility>
using namespace std;

#include "debug.h"
#include "relops.h"

class ubigint {
   friend ostream& operator<< (ostream&, const ubigint&);
   private:
      using quot_rem = pair<ubigint,ubigint>;
      //using unumber = unsigned long;
      //unumber uvalue {};

      using udigit_t = unsigned char;
      using ubigvalue_t = vector<udigit_t>;
      ubigvalue_t ubig_value;

      quot_rem divide (const ubigint&) const;
      void multiply_by_2();
      void divide_by_2();

      //AddedMethods
      long uvalue{};
      friend ubigint addVecs(
         const ubigint& left, const ubigint& right);

      friend ubigint subVecs(
         const ubigint& left, const ubigint& right);

      friend bool compareLeft(
         const ubigint& left, const ubigint& right);
      
      friend ubigint multVecs(
         const ubigint& left, const ubigint& right);

   public:

      ubigint() = default; // Need default ctor as well.
      ubigint (unsigned long);

      ubigint (const string&);

      ubigint operator+ (const ubigint&) const;
      ubigint operator- (const ubigint&) const;
      ubigint operator* (const ubigint&) const;
      ubigint operator/ (const ubigint&) const;
      ubigint operator% (const ubigint&) const;

      bool operator== (const ubigint&) const;
      bool operator<  (const ubigint&) const;

      void clean();
};

#endif