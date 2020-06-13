/* 
Thomas Burch (1314305)
tburch1@ucsc.edu
7/27/15
*/

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>

using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

using str_str_map = listmap<string,string>;
using str_str_pair = str_str_map::value_type;

using str_key = str_str_map::key_type;
using str_value = str_str_map::mapped_type;

void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            traceflags::setflags (optarg);
            break;
         default:
            complain() << "-" << (char) optopt << ": invalid option"
                       << endl;
            break;
      }
   }
}

string trim(const string& str) {
   size_t first = str.find_first_not_of (" \t");
   if (first == string::npos) return "";
   size_t last = str.find_last_not_of (" \t");
   return str.substr (first, last - first + 1);
}

void handle_lines(
  str_str_map& test, string& filename, istream& infile){
    str_key Key;
    str_value Value;
    for(int i = 1 ;; ++i){
      string line;
      getline (infile, line);
      string trimLine = trim(line);
    if(infile.eof()){
      break;
    }
    cout << filename << ": " 
      << i << ": " << line << endl;
    if(trimLine.size() == 0 
      or trimLine[0] == '#'){
          continue;
    }
    size_t pos = line.find_first_of ("=");
    if(pos == string::npos){
      Key = trim(line);
      str_str_map::iterator itor = test.find(Key);
    if(itor == test.end()){
      cout << Key << ": Key not found" << endl;
    }else{
      cout << "During iteration: " << *itor << endl;
    }
      continue;
    }else{
      Key = trim(line.substr(0, pos == 0 ? 0 : pos));
      Value = trim(line.substr(pos + 1));
    }
    if(Key == "" and Value == ""){
    for(str_str_map::iterator itor  = test.begin();
        itor != test.end();
        ++itor){
        cout << "During iteration: " 
        << *itor << endl;
        }
    }else if(Key == ""){
    for(str_str_map::iterator itor = test.begin();
        itor != test.end();
        ++itor){
    if(itor->second == Value){
        cout << "During iteration: " 
        << *itor << endl;
    }
    }
    }else if(Value == ""){
        str_str_map::iterator itor = test.find(Key);
    if(itor != test.end()){
        test.erase(itor);
    }
    }else{
      str_str_pair pair(Key, Value);
      cout << "Before insert: " 
      << pair << endl;
      test.insert(pair);
    }
    }
}

int main (int argc, char** argv) {
  sys_info::set_execname (argv[0]);
  scan_options (argc, argv);

  str_str_map test;

  // Use istream pointer to use one variable for cin and files
  int exit_sucess = 0;
  string execname(argv[0]);
  string filename("-");
  if(argc == 1){
    handle_lines(test, filename, cin);
  }
  for(int i = 1; i < argc; i++){
    filename = string(argv[i]);
  if (filename == "-"){
    handle_lines(test, filename, cin);
  }else{
    ifstream infile(filename);
  if(infile.fail()){
    cerr << execname << ": " << filename 
    << ": No such file or directory" << endl;
    exit_sucess = 1;
    continue;
  }else{
    handle_lines(test, filename, infile);
    infile.close();
  }
  }
  }
  cout << "EXIT_SUCCESS" << endl;
  return exit_sucess; 
}

