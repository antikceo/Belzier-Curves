
#include "belzier.h"
#include <iostream>
using std::cerr;
using std::endl;


int SCREEN_HEIGHT = 480;
// Keep track of times clicked, on 3 clicks draw.
int NUMPOINTS = 0;
int ret[2];
int Udeg;
int Vdeg;
int num_CS;
char type;
char * values;
Point abc[4];
Point abc1[4];
Point abc2[12];
double degs = 1.0;
double zoom = 800.0;
GLfloat angle = 0;


float ctrlpoints[4][3] = {
    { 0.0, 200.0, 0.0}, { 100.0, 180.0, 0.0}, 
    {200.0, 160.0, 0.0}, {300.0, 130.0, 0.0}};

float ctrlpoints1[4][3] = {
    { 0.0, 200.0, 0.0}, { 100.0, 180.0, 0.0}, 
    {200.0, 160.0, 0.0}, {300.0, 130.0, 0.0}};

float ctrlpoints2[4][3] = {
    { 0.0, 100.0, 0.0}, { 100.0, 100.0, 0.0}, 
    {200.0, 100.0, 0.0}, {300.0, 100.0, 0.0}};

float ctrlpoints3[4][3] = {
    { 0.0, 0.0, 100.0}, { 100.0, 0.0, 100.0}, 
    {200.0, 0.0, 100.0}, {300.0, 100.0, 100.0}};



void theFile(char * fName){
    FILE *fp;
    
    if((fp = fopen(fName, "r"))==NULL)
    {
        printf("No file!");
    }
    
    char buffer[MAXNUM];
    char temp1[MAXNUM];
    
    int counter = 1; 
    int m = 0;
    while(!feof(fp))
    {
        memset(buffer,0, MAXNUM-1);
        fgets(buffer, MAXNUM, fp);
        if (counter ==1) {
            num_CS = atoi(buffer);
        }
        else if (counter ==2){
            strncpy(temp1, buffer, MAXNUM);
            char * pch = strtok (temp1," ");
            int itr = 1;
            while (pch != NULL)
            {
                //printf ("%s\n",pch);
                if (itr == 1){
                    Udeg = atoi(pch);
                    ret[0] = Udeg;
                }
                if (itr == 2){
                    Vdeg = atoi(pch);
                    ret[1] = Vdeg;
                    
                    values = new char[Udeg*3];
                    
                    /*for (int i = 0; i<Udeg; i++) {
                        values[i] = new  char[Vdeg];
                    }
                     */
                }
                pch = strtok (NULL, " ");
                itr++;
            }
        }
        else if (counter<=6 && counter>=3){
            
        }
        else{
            //printf ("Vals %d %d\n",Udeg, Vdeg);
            if (Vdeg == 0) {
                type = 'c';
                
                if (m<=Udeg) {
                    strncpy(temp1, buffer, MAXNUM);
                    char * pch = strtok (temp1," ");
                    int itr = 0;
                    while ( pch != NULL)
                    {
                        if ((m*3)+itr == Udeg*3) {
                            break;
                        }
                        //printf ("Vals %d %d %s\n",m, (m*3)+itr, pch);
                        values[(m*3)+itr] = atoi(pch);
                        //printf ("Vals %d %d %c\n", m, (m*3)+itr, values[(m*3)+itr]);
                        pch = strtok (NULL, " ");
                        itr++;
                    }  
                    m+=1;
                }
                else{
                    //m=0;
                    //counter = -1;
                }
            }
            else{
                type = 's';
            }
        }
        counter ++;
        
    }
}
     

void Special_Keys (int key, int x, int y)
{
    switch (key) {
            
        case GLUT_KEY_LEFT :  angle += 1;  break;
        case GLUT_KEY_RIGHT:  angle -= 1;  break;
    }
    
    glutPostRedisplay();
}

void Keyboard (unsigned char key, int x, int y)
{
    switch (key) {
            

        case 'x' : zoom += 1;  break;
        case 'z' : zoom -= 1;  break;
    }
    
    glutPostRedisplay();
}


void init(char * fileName) {
    //theFile(fileName);
    glClearColor(0.0,0.0,0.0,0.0);
    glColor3f(1.0,0.0,0.0);
    glPointSize(4.0);   
    
} 

void drawLine(Point p1, Point p2) {
    glBegin(GL_LINES);
    glVertex3f(p1.x, p1.y, p1.z);
    glVertex3f(p2.x, p2.y, p2.z);
    
    glEnd();
    glFlush();
}


Point drawBezierGen (Point * p, int u, double t) {
    
    int rows = u+1;
    Point  q[4] ;
    for (int i=0; i<rows ; i++) {
        q[i].x = p[i].x;
        q[i].y = p[i].y;
        q[i].z = p[i].z;
    }
    for (int k=1; k<rows; k++) {
        for (int i = 0; i<(rows-k); i++) {
            q[i].x = (1-t)*q[i].x + t *q[i+1].x;
            q[i].y = (1-t)*q[i].x + t *q[i+1].y;
            q[i].z = (1-t)*q[i].z + t *q[i+1].z;
        }
    }
    
    return q[0];
}
    
//I know this will work if tested correctly, I just didn't have enough time.
Point bezierSurface(Point * p, int u, int v, int t ) {
    int rows = v+1;
    int cols = u+1;
    Point q[4];
    Point r[3];
    
    
    for (int j= 0 ; j<rows; j++) {
        for (int i=0; i<cols ; i++) {
            q[i].x = p[i].x;
            q[i].y = p[i].y;
            q[i].z = p[i].z;
        }
        for (int k=1; k<cols; k++) {
            for (int i = 0; i<(cols-k); i++) {
                q[i].x = (1-t)*q[i].x + t *q[i+1].x;
                q[i].y = (1-t)*q[i].x + t *q[i+1].y;
                q[i].z = (1-t)*q[i].z + t *q[i+1].z;
            }
        }
        r[j] = q[0];
    }
    
    for (int j= 0 ; j<rows; j++) {
        for (int k=1; k<cols; k++) {
            for (int i = 0; i<(cols-k); i++) {
                r[j].x = (1-t)*r[j].x + t *r[j+1].x;
                r[j].y = (1-t)*r[j].x + t *r[j+1].y;
                r[j].z = (1-t)*r[j].z + t *r[j+1].z;
            }
        }
        
    }
       return r[0]; 
}



// Cubic Bezier
Point drawBezier(Point A, Point B, Point C, Point D, double t) {
    Point P;
    
    P.x = pow((1 - t), 3) * A.x + 3 * t * pow((1 -t), 2) * B.x + 3 * (1-t) * pow(t, 2)* C.x + pow (t, 3)* D.x;
    P.y = pow((1 - t), 3) * A.y + 3 * t * pow((1 -t), 2) * B.y + 3 * (1-t) * pow(t, 2)* C.y + pow (t, 3)* D.y;
    P.z = pow((1 - t), 3) * A.z + 3 * t * pow((1 -t), 2) * B.z + 3 * (1-t) * pow(t, 2)* C.z + pow (t, 3)* D.z;
    
    return P;
}



void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,zoom,0.0,zoom);
    
    glRotatef(angle, 0, 0, 1);
    for(int i = 0; i<4; i++){
        abc[i].setxyz(ctrlpoints[i][0],ctrlpoints[i][1], ctrlpoints[i][2]);
        //printf("Points: %f %f \n", ctrlpoints[i][0],ctrlpoints[i][1]);
    }
    
    for(int i = 0; i<4; i++){
        abc1[i].setxyz(ctrlpoints2[i][0],ctrlpoints2[i][1], ctrlpoints2[i][2]);
    }
    
    for(int i = 0; i<4; i++){
        abc2[i].setxyz(ctrlpoints1[i][0],ctrlpoints1[i][1], ctrlpoints1[i][2]);
    }
    
    Point Old = abc[0];
    
    for(double t = 0.0;t <= 1.0; t += 0.1) {
        //Point P = drawBezier(abc[0], abc[1], abc[2], abc[3],  t);
        Point P = drawBezierGen(abc, 3, t);
            drawLine(Old, P);
            Old = P;

    }
    
    Point Old1 = abc1[0];
    for(double t = 0.0;t <= 1.0; t += 0.1) {
        Point P = drawBezierGen(abc1, 3, t);
        drawLine(Old1, P);
        Old1 = P;
        
    }
    
 
    Point Old2  = abc2[0];
    for(double t = 0.0;t <= 1.0; t += 0.1) {
        Point P = bezierSurface(abc2, 3, 2, t);
        
        drawLine(Old, P);
        Old2 = P;
    }

    glutSolidTeapot(300);
    glFlush();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(1000,1000);
    glutInitWindowPosition(100,150);
    glutCreateWindow("Bezier Curve");
    init(argv[1]);
    glutDisplayFunc(display);
    glutKeyboardFunc (Keyboard  );
    glutSpecialFunc  (Special_Keys);
    glutMainLoop();
    
    return 0;
}