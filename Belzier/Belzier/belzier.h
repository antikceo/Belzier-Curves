#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define MAXNUM 100
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iterator>
#include <math.h>
#include <sstream>
#include <ctype.h>

class Point {
public:
    float x, y, z;
    void setxyz(float x2, float y2, float z2) { x = x2; y = y2; z = z2;}
    const Point & operator=(const Point &rPoint) {
        x = rPoint.x;
        y = rPoint.y;
        z = rPoint.z;
        
        return *this;
    }
    
};

void special (int key, int x, int y);
void key_pressed (unsigned char key, int x, int y);
void Draw_Box (void);
void display (void);


Point drawBezier(Point A, Point B, Point C, Point D, double t);
void theFile(char * fName);