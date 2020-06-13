/*-----------------------------
Thomas Burch (tburch1@ucsc.edu)
CMPS 109 Program 2
Overloading and operators
7/14/15
------------------------------*/

#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
using namespace std;

#include "ubigint.h"
#include "debug.h"

#include <limits>
#define LINE_LIMIT 69


ubigint::ubigint (unsigned long that): uvalue (that) {
   DEBUGF ('~', this << " -> " << uvalue)

    int value = 0;
      auto digit = to_string(that).cbegin();
    while (digit != to_string(that).cend()) {
        value = value * 10 + *digit++ - '0';
    }

    auto ritor  = to_string(that).crbegin();
    for (; ritor != to_string(that).crend() &&
          *ritor != '-';
           ritor++) {
        ubig_value.push_back(*ritor - '0');
    }
}

ubigint::ubigint (const string& that){
  int uvalue = 0;
   auto digit = that.cbegin();
    while (digit != that.cend()) {
        uvalue = uvalue * 10 + *digit++ - '0';
    }

    auto ritor  = that.crbegin();
    for (; ritor != that.crend() && *ritor != '_';
            ritor++) {
        ubig_value.push_back(*ritor - '0');
    }
}

void ubigint::clean(){
  auto itor = ubig_value.rbegin();
  for(;itor != ubig_value.rend() ;itor++){
    if(*itor != 0){
        break;
    }
    ubig_value.pop_back();
  }
}

//Do Add
ubigint addVecs(const ubigint& left,const ubigint& right){
     
      ubigint sum;
      int carry =0;
      auto lit = left.ubig_value.begin();
      auto rit = right.ubig_value.begin();

      int art_size = 
      (left.ubig_value.size()>right.ubig_value.size()) ?
      left.ubig_value.size() : right.ubig_value.size();

       for(int i =0; i<art_size+1; i++){
          int l = carry;
          int r = 0;
          if(lit != left.ubig_value.end()){
           l += *lit;
            ++lit;
          }
          if(rit != right.ubig_value.end()){
            r += *rit;
             ++rit;
           }
          sum.ubig_value.push_back( (l+r) %10);
          carry = (l+r)/10;
       }
       sum.clean();  
       return sum;
}

ubigint ubigint::operator+ (const ubigint& that) const {
   ubigint sum;
   sum = addVecs(*this, that);
   return sum;
}

//Do Sub -- left should always be bigger than right
ubigint subVecs(const ubigint& left,const ubigint& right){
      ubigint diff;
      int carry = 0;
      auto lit = left.ubig_value.begin();
      auto rit = right.ubig_value.begin();
      int art_size = left.ubig_value.size();
       for(int i =0; i<art_size;i++){
          int l = 0;
          int r =0;
          if(lit != left.ubig_value.end()){
            l = *lit;
            ++lit;
          }
          if(rit != right.ubig_value.end()){
            r = *rit;
            ++rit;
          }
          if(l - r - carry < 0){
            l += 10; 
            diff.ubig_value.push_back(l-r-carry);
            carry = 1; 
          }else{
          diff.ubig_value.push_back (l-r-carry);
          carry = 0;
          }
      }
       diff.clean();
       return diff;
}

ubigint ubigint::operator- (const ubigint& that) const {
   ubigint diff;
   diff = subVecs(*this, that);
   return diff;
}

ubigint multVecs(const ubigint& left, const ubigint& right){
  ubigint prod{}; 
  int value {};
  prod.ubig_value.resize
  (left.ubig_value.size()+right.ubig_value.size() +1);

  auto lit = left.ubig_value.begin(); 

  for(int i = 0; 
      i<static_cast<int>(left.ubig_value.size())
      ;i++){
      int carry = 0;
      auto rit = right.ubig_value.begin();

      for(int j =0;
          j < static_cast<int>(right.ubig_value.size());
          j++){

         value = prod.ubig_value[i+j] + (*lit)*(*rit) + carry;
         prod.ubig_value[i+j] = value%10;
         carry = value/10;
         ++rit;
      }
      prod.ubig_value[i+right.ubig_value.size()] = carry;
      ++lit;
   }
   prod.clean();
   return prod;
}

ubigint ubigint::operator* (const ubigint& that) const {
    ubigint mult;
    mult = multVecs(*this, that);
    return mult;
}

void ubigint::multiply_by_2() {
    ubig_value = addVecs(*this,*this).ubig_value;
    clean();
   
}

void ubigint::divide_by_2(){
  auto itor = ubig_value.rbegin(); 
  auto end = ubig_value.rend();
  int carry = 0;
  for(;itor != end ;itor++){
      if(*itor%2 == 1){
         *itor = *itor/2 + carry; 
         carry = 5;
      }else{
        *itor = *itor/2 + carry; 
         carry = 0;
      }
   }
   clean();
}

ubigint::quot_rem ubigint::divide (const ubigint& that) const {
   static const ubigint zero = 0;
   if (that == zero){
      throw domain_error ("ubigint::divide: by 0");
    }
   ubigint power_of_2 = 1;
   ubigint divisor = that; // right operand, divisor
   ubigint quotient = 0;
   ubigint remainder = *this; // left operand, dividend
   while (divisor < remainder) {
      divisor.multiply_by_2();
      power_of_2.multiply_by_2();
   }
   while (power_of_2 > zero) {
      if (divisor <= remainder) {
         remainder = remainder - divisor;
         quotient = quotient + power_of_2;
      }
      divisor.divide_by_2();
      power_of_2.divide_by_2();
   }
   return {quotient, remainder};
}

ubigint ubigint::operator/ (const ubigint& that) const {
   return divide (that).first;
}

ubigint ubigint::operator% (const ubigint& that) const {
   return divide (that).second;
}

bool ubigint::operator== (const ubigint& that) const {
   //size of vecs different return false.
    if (ubig_value.size() != that.ubig_value.size()){
        return false;
      }
   //compare the vecs digits individually return false if not same 
    for (size_t i = 0; i < ubig_value.size(); i++){
        if (ubig_value.at(i) != that.ubig_value.at(i)){
            return false;
      }
    }
    return true; //else true
}

// Returns true if left < right, assumes both are positive
// integers.
bool compareLeft (const ubigint& left, const ubigint& right) {
    if(left.ubig_value.size() < right.ubig_value.size()){
        return true;
    }else if(left.ubig_value.size() >
            right.ubig_value.size()){
        return false;
    }

    auto lit = left.ubig_value.rbegin();
    auto rit = right.ubig_value.rbegin();
    for (int i = 0; 
          i < (int)left.ubig_value.size();
          i++){
        if(*lit < *rit){
          return true;
        }else if(*lit > *rit){
          return false;
        }
        ++lit;
        ++rit;
    }
    return false;
}

bool ubigint::operator< (const ubigint& that) const {
   bool retval;
   retval = compareLeft(*this, that);
   return retval;
}


ostream& operator<< (ostream& out, const ubigint& that) {  
    size_t i = 0;
    auto ritor = that.ubig_value.crbegin();
   for(;ritor != that.ubig_value.crend();
         ++ritor){
           out << (int) *ritor;
           i++;
           if(i == LINE_LIMIT){
            cout<< "\\" << endl;
            i = 0;
           }
   } 
   return out;  
}





