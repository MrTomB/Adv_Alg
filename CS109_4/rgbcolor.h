/*-----------------------------
Thomas Burch (tburch1@ucsc.edu)
CMPS 109 Program 4
OOP, Inheritance, OpenGL
8/3/15
------------------------------*/

#ifndef __RGBCOLOR_H__
#define __RGBCOLOR_H__

#include <string>
#include <unordered_map>
using namespace std;

#include <GL/freeglut.h>

struct rgbcolor {
   union {
      GLubyte ubvec[3];
      struct {
         GLubyte red;
         GLubyte green;
         GLubyte blue;
      };
   };
   explicit rgbcolor(): red(0), green(0), blue(0) {}
   explicit rgbcolor (GLubyte red, GLubyte green, GLubyte blue):
               red(red), green(green), blue(blue) {}
   explicit rgbcolor (const string&);
   const GLubyte* ubvec3() { return ubvec; }
   operator string() const;

   //ADDED
   static bool colorExists(const string&);
};

ostream& operator<< (ostream&, const rgbcolor&);

extern const std::unordered_map<std::string,rgbcolor> color_names;

#endif
