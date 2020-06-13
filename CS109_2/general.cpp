/*-----------------------------
Thomas Burch (tburch1@ucsc.edu)
CMPS 109 Program 2
Overloading and operators
7/14/15
------------------------------*/

#include <cstring>
using namespace std;

#include "general.h"

ydc_exn::ydc_exn (const string& what): runtime_error (what) {
}

string exec::execname_; // Must be initialized from main().
int exec::status_ = EXIT_SUCCESS;

void exec::execname (const string& argv0) {
   execname_ = basename (argv0.c_str());
   cout << boolalpha;
   cerr << boolalpha;
   DEBUGF ('Y', "execname = " << execname_);
}

ostream& note() {
   return cerr << exec::execname() << ": ";
}

ostream& error() {
   exec::status (EXIT_FAILURE);
   return note();
}
