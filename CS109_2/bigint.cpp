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

#include "bigint.h"
#include "debug.h"
#include "relops.h"


bigint::bigint (long that): uvalue (that), is_negative (that < 0) {
   DEBUGF ('~', this << " -> " << uvalue)
   
    is_negative = false; 
    if((to_string(that).size() > 0) &&
        (to_string(that)[0] == '-')) {
        is_negative = true;
    }
    uvalue = 
    ubigint(to_string(that).substr(is_negative ? 1 : 0));  
}

bigint::bigint (const ubigint& uvalue, bool is_negative):
                uvalue(uvalue), is_negative(is_negative) {
}

bigint::bigint (const string& that) {
    is_negative = false;

    if((that.size() > 0) &&
        (that[0] == '_')) {
        is_negative = true;
    }

    uvalue = ubigint(that.substr(is_negative ? 1 : 0));

}


bigint bigint::operator+() const {
   return *this;
}

bigint bigint::operator-() const {
    return -(*this);
}

bigint bigint::operator+ (const bigint& that) const {

  ubigint result;
  bigint value;

  if(is_negative == that.is_negative){      
      result = uvalue + that.uvalue;
      value = result;
      value.is_negative = is_negative;
   	return value;

  }else{
  		result = (*this < that) ? (uvalue - that.uvalue)
  								: (that.uvalue - uvalue);
  		value = result;
  		if(*this < that){
  			value.is_negative = is_negative;
  		}else{
  			value.is_negative = that.is_negative;
  		}
  		return value;
  	}
  }

bigint bigint::operator- (const bigint& that) const {
  ubigint result;
  bigint value;
  if(is_negative == that.is_negative){ 
  		result = (*this < that) ? (that.uvalue - uvalue)
  								: (uvalue - that.uvalue);
  	value = result;
  	if(*this < that){
  		value.is_negative = not that.is_negative;
  	}else{
  		value.is_negative = is_negative;
  	}
  	return value;
  }else{
  		result = uvalue + that.uvalue;
  		value = result;
  		value.is_negative = is_negative;
  		return value;
  }
}

bigint bigint::operator* (const bigint& that) const {
ubigint result;
bigint value;
  if(is_negative == that.is_negative){
    result = uvalue * that.uvalue;
    value = result;
    value.is_negative = false;
  }else{
    result = uvalue * that.uvalue;
    value = result;
    value.is_negative = true;  
  }
   return value;
}

bigint bigint::operator/ (const bigint& that) const {
ubigint result;
ubigint negOne(-1);
bigint value;
  if(is_negative && !that.is_negative){
    result = (uvalue*negOne) / that.uvalue;
    value = result;
    value.is_negative = true;
    return value;
  }
  if(that.is_negative && !is_negative){
    result = uvalue / (that.uvalue*negOne);
    value = result;
    value.is_negative = true;
    return value;
  }
  if(that.is_negative && is_negative){
    result = (uvalue*negOne) / (that.uvalue*negOne);
    value = result;
    value.is_negative = false;
    return value;
    }else{
    result = uvalue / that.uvalue;
    value = result;
    value.is_negative = false;
    return value;
  }
    return value;
}

bigint bigint::operator% (const bigint& that) const {
ubigint result;
ubigint negOne(-1);
bigint value;
  if(is_negative && !that.is_negative){
    result = (uvalue*negOne) % that.uvalue;
    value = result;
    value.is_negative = true;
    return value;
  }
  if(that.is_negative && !is_negative){
    result = uvalue % (that.uvalue*negOne);
    value = result;
    value.is_negative = true;
    return value;
  }
  if(that.is_negative && is_negative){
    result = (uvalue*negOne) % (that.uvalue*negOne);
    value = result;
    value.is_negative = false;
    return value;
  }else{
    result = uvalue % that.uvalue;
    value = result;
    value.is_negative = false;
    return value;
  }
    return value;

}

bool bigint::operator== (const bigint& that) const {
  if (is_negative != that.is_negative){
        return false;
  }
   return is_negative == that.is_negative
    and uvalue == that.uvalue;
}

bool bigint::operator< (const bigint& that) const {
bool retbool;
 if(is_negative){
      if(!that.is_negative){
      retbool = true;
    }else{
      retbool = that.uvalue < uvalue;
    }
  }else{
    if(that.is_negative){
      retbool = false;
    }else{
      retbool = uvalue < that.uvalue;
    } 
} 
    return retbool;
}

ostream& operator<< (ostream& out, const bigint& that) {
    if (that.is_negative){
        out << '-';
    }
    out << that.uvalue;
   return out;
}


