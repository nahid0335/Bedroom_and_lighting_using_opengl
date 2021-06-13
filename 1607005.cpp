#include<GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include<math.h>
#include<bits/stdc++.h>

using namespace std;

#define PI 3.1415927
const int windowWidth = 700;
const int windowheight = 700;
const float windowratio = 1.0 * windowWidth / windowheight;

GLfloat eyeX = 0;
GLfloat eyeY = 8;
GLfloat eyeZ = -20;

GLfloat lookX = 0;
GLfloat lookY = 8;
GLfloat lookZ = -16;

float inithight = 0.5;
float initwight = 0.5;
float initdense = 0.5;

float roomHeight = 20;
float roomWidth = 30;
float roomLength = 20;

float walldepth = 1;

float wearDropWidth = 5;
float wearDropHeight = 8;
float wearDropLength = 3;

float portHoleWidth = 2.75;
float portHoleHeight = 4;
float portHoleLength = 0.1;

float dressingTableWidth = 2;
float dressingTableHeight = 4;
float dressingTableLength = 8;

float bedWidth = 8;
float bedHeight = 2;
float bedLength = 12;

float rotationYAngle = 0;
float rotationXAngle = 0;

float fanAngle = 0.0;
bool fanRotate = true;
float hourAngle = 0.0;
float minuteAngle = 0.0;

bool cameraMode = true;
float eyeAngle = 0;
float lookAngleX = 180;
float lookAngleY = 180;

bool light0_On = true;
bool light1_On = true;
bool light2_On = false;

bool no_ambient = false;
bool no_diffuse = false;
bool no_specular = false;


class rotatePosition {
public:
    float position1, position2,position3,pangle;
};


static GLfloat vertex_unitCube[8][3] =
{
    {0.0, 0.0, 0.0},    //0
    {0.0, 0.0, 1.0},    //1
    {1.0, 0.0, 1.0},    //2
    {1.0, 0.0, 0.0},    //3
    {0.0, 1.0, 0.0},    //4
    {0.0, 1.0, 1.0},    //5
    {1.0, 1.0, 1.0},    //6
    {1.0, 1.0, 0.0}     //7
};

static GLubyte unitCube_indices[6][4] =
{
    {1, 2, 6, 5},
    {2, 3, 7, 6},
    {3, 0, 4, 7},
    {0, 1, 5, 4},
    {4, 5, 6, 7},
    {1, 0, 3, 2}
};

static void getNormal3p(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2-x1;
    Uy = y2-y1;
    Uz = z2-z1;

    Vx = x3-x1;
    Vy = y3-y1;
    Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;
    Ny = Uz*Vx - Ux*Vz;
    Nz = Ux*Vy - Uy*Vx;

    glNormal3f(Nx,Ny,Nz);
}



void draw_cylinder(GLfloat topRadius,GLfloat baseRadius,GLfloat height,bool baseOpen=false,float colorR=0.5, float colorG=0.5, float colorB=0.5, float alpha=1)
{
    GLfloat xb = 0.0;
    GLfloat yb = 0.0;
    GLfloat xt = 0.0;
    GLfloat yt = 0.0;
    GLfloat angle = 0.0;
    GLfloat angle_stepsize = 0.1;

    GLfloat m_no[] = {0, 0, 0, 1.0};
    GLfloat m_ambient[] = {colorR,colorG,colorB,1};
    GLfloat m_diffuse[] = {colorR,colorG,colorB,1};
    GLfloat m_specular[] = {1,1,1,1};
    GLfloat m_shininess[] = {20};

    GLfloat m_emmission[] = {1,1,1,1};

    glMaterialfv(GL_FRONT, GL_AMBIENT, m_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, m_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, m_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, m_shininess);

    if(!baseOpen &&light2_On)
        glMaterialfv(GL_FRONT, GL_EMISSION, m_emmission);
    else
        glMaterialfv(GL_FRONT, GL_EMISSION, m_no);

    /** Draw the tube */
    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
    while( angle < 2*PI )
    {
        xb = baseRadius * cos(angle);
        yb = baseRadius * sin(angle);
        xt = topRadius * cos(angle);
        yt = topRadius * sin(angle);
        glVertex3f(xt, yt, height);
        glVertex3f(xb, yb, 0.0);
        angle = angle + angle_stepsize;
    }
    glVertex3f(topRadius, 0.0, height);
    glVertex3f(baseRadius, 0.0, 0.0);
    glEnd();

    /** Draw the circle on top of cylinder */
    glBegin(GL_POLYGON);
    angle = 0.0;
    while( angle < 2*PI )
    {
        xt = topRadius * cos(angle);
        yt = topRadius * sin(angle);
        glVertex3f(xt, yt, height);
        angle = angle + angle_stepsize;
    }
    glVertex3f(topRadius, 0.0, height);
    glEnd();

    if(baseOpen){;
        glBegin(GL_POLYGON);
        angle = 0.0;
        while( angle < 2*PI )
        {
            xb = baseRadius * cos(angle);
            yb = baseRadius * sin(angle);
            glVertex3f(xb, yb, 0.0);
            angle = angle + angle_stepsize;
        }
        glVertex3f(baseRadius, 0.0, 0.0);
        glEnd();
    }
}


void unitCube(float colorR=0.5, float colorG=0.5, float colorB=0.5,bool emmission_on=false, float alpha=1)
{

    GLfloat m_no[] = {0, 0, 0, 1.0};
    GLfloat m_ambient[] = {colorR,colorG,colorB,1};
    GLfloat m_diffuse[] = {colorR,colorG,colorB,1};
    GLfloat m_specular[] = {1,1,1,1};
    GLfloat m_shininess[] = {20};

    GLfloat m_emmission[] = {1,1,1,1};

    glMaterialfv(GL_FRONT, GL_AMBIENT, m_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, m_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, m_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, m_shininess);

    if(emmission_on && (light0_On||light1_On))
        glMaterialfv(GL_FRONT, GL_EMISSION, m_emmission);
    else
        glMaterialfv(GL_FRONT, GL_EMISSION, m_no);

    glBegin(GL_QUADS);
    for (GLint i = 0; i <6; i++)
    {
        getNormal3p(vertex_unitCube[unitCube_indices[i][0]][0], vertex_unitCube[unitCube_indices[i][0]][1], vertex_unitCube[unitCube_indices[i][0]][2],
                    vertex_unitCube[unitCube_indices[i][1]][0], vertex_unitCube[unitCube_indices[i][1]][1], vertex_unitCube[unitCube_indices[i][1]][2],
                    vertex_unitCube[unitCube_indices[i][2]][0], vertex_unitCube[unitCube_indices[i][2]][1], vertex_unitCube[unitCube_indices[i][2]][2]);

        for (GLint j=0; j<4; j++)
        {
            glVertex3fv(&vertex_unitCube[unitCube_indices[i][j]][0]);
        }
    }
    glEnd();
}

void axes()
{
    float length = 10;
    float width = 0.3;

    // X-axis
    glPushMatrix();
    glTranslatef(length/2,0,0);
    glScalef(length,width,width);
    glTranslatef(-0.5,-0.5,-0.5);
    unitCube(0.8,0.1,0.1);  //red
    glPopMatrix();

    // Y-axis
    glPushMatrix();
    glTranslatef(0,length/2,0);
    glScalef(width,length,width);
    glTranslatef(-0.5,-0.5,-0.5);
    unitCube(0.1,0.8,0.1);  //green
    glPopMatrix();

    // Z-axis
    glPushMatrix();
    glTranslatef(0,0,length/2);
    glScalef(width,width,length);
    glTranslatef(-0.5,-0.5,-0.5);
    unitCube(0.1,0.1,0.8);  //blue
    glPopMatrix();
}

void roomFloor(float x,float y,float z,float xx,float yy,float zz,float cr=0.5,float cg=0.5,float cb=0.5)
{
    glPushMatrix();
        glTranslatef(xx,yy,zz);
        glScalef(x,y,z);
        glTranslatef(-inithight,-initwight,-initdense);
        unitCube(cr,cg,cb);
    glPopMatrix();
}

void building(){
    roomFloor(roomWidth,walldepth,roomLength,0,-walldepth/2,0); //bottom ->1st 3 parameter scale,2nd 3 translation,3rd 3 color
    roomFloor(roomWidth,walldepth,roomLength,0,roomHeight+walldepth/2,0,0.61,0.56,0.49);   //top
    roomFloor(walldepth,roomHeight,roomLength,(roomWidth/2-walldepth/2),roomHeight/2,0,0.18,0.55,0.34);    //left
    roomFloor(walldepth,roomHeight,roomLength,-(roomWidth/2-walldepth/2),roomHeight/2,0,0.18,0.55,0.34);   //right
    roomFloor(roomWidth,roomHeight,walldepth,0,roomHeight/2,(roomLength/2-walldepth/2),0.55,0.39,0.42);    //back
}

void weardrop(float x,float z,int n,float angle=0){
    wearDropHeight = 2*n+(n+1)*0.5;

    glPushMatrix();
        glTranslatef(x,0,z);
        glTranslatef(0,wearDropHeight/2,0);
        glRotatef(angle,0,1,0);
        glTranslatef(0,-wearDropHeight/2,0);
        glPushMatrix();
            for(int i=0;i<n;i++)
            {
                float yshift = 1.5+i*2.5;
                //drawer
                glPushMatrix();
                    glTranslatef(0,yshift,-(wearDropLength/2)+0.3);
                    glScalef(wearDropWidth-1,2,1);
                    glTranslatef(-inithight,-initwight,-initdense);
                    unitCube(0.165,0.09,0.059);
                glPopMatrix();

                //handle
                glPushMatrix();
                    glTranslatef(0,yshift,-wearDropLength/2);
                    glScalef(1,.2,.5);
                    glTranslatef(-inithight,-initwight,-initdense);
                    unitCube(1,1,1);
                glPopMatrix();
            }

            //body
            glPushMatrix();
                glTranslatef(0,wearDropHeight/2,0);
                glScalef(wearDropWidth,wearDropHeight,wearDropLength);
                glTranslatef(-inithight,-initwight,-initdense);
                //unitCube(89,39,25);
                unitCube(0.35,0.15,0.098);
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();

}


void weardrops(){
    weardrop(roomWidth/2-walldepth-wearDropWidth/2,roomLength/2-walldepth-wearDropLength/2,3);
    weardrop(-(roomWidth/2-walldepth-wearDropLength/2),-(bedLength-roomLength/2+walldepth+1+wearDropWidth/2),2,-90);
}

void bed(float x, float z){
    float coverHight = 1;
    float headLength = 1;
    float headHight = 5;
    glPushMatrix();
        glTranslatef(x,0,z);
        glPushMatrix();
            //bad mat
            glPushMatrix();
                glPushMatrix();
                    glTranslatef((bedWidth/2)+1.5,0.25,0);
                    glScalef(1,0.1,5);
                    glTranslatef(-inithight,-initwight,-initdense);
                    unitCube(0.22,0.29,0.67);
                glPopMatrix();
                glPushMatrix();
                    glTranslatef((bedWidth/2)+1.5,0.2,0);
                    glScalef(3,0.1,5);
                    glTranslatef(-inithight,-initwight,-initdense);
                    unitCube(1,0.24,0);
                glPopMatrix();
            glPopMatrix();

            //bad pillow 1
            glPushMatrix();
                glTranslatef(2,bedHeight+coverHight,3.5);
                glScalef((bedWidth/2)-1,1,2);
                glTranslatef(-inithight,-initwight,-initdense);
                unitCube(0,0,0.8);
            glPopMatrix();

            //bad pillow 2
            glPushMatrix();
                glTranslatef(-2,bedHeight+coverHight,3.5);
                glScalef((bedWidth/2)-1,1,2);
                glTranslatef(-inithight,-initwight,-initdense);
                unitCube(0,0,0.8);
            glPopMatrix();

            //bad shit
            glPushMatrix();
                glTranslatef(0,bedHeight+coverHight,-(3));
                glScalef(bedWidth-1,0.1,4);
                glTranslatef(-inithight,-initwight,-initdense);
                unitCube(0.55,0.04,0.31);
            glPopMatrix();

            //bed box left 2
            glPushMatrix();
                glTranslatef(bedWidth/2,0.5+(bedHeight-1)/2,-(0.5+((bedLength/2)-1)/2));
                glScalef(0.1,1,5);
                glTranslatef(-inithight,-initwight,-initdense);
                unitCube(0.3,0.16,0.1);
            glPopMatrix();

            //bed box left 1
            glPushMatrix();
                glTranslatef(bedWidth/2,0.5+(bedHeight-1)/2,0.5+((bedLength/2)-1)/2);
                glScalef(0.1,bedHeight-1,(bedLength/2)-1);
                glTranslatef(-inithight,-initwight,-initdense);
                unitCube(0.3,0.16,0.1);
            glPopMatrix();

            //bed box front
            glPushMatrix();
                glTranslatef(0,0.5+(bedHeight-1)/2,-bedLength/2);
                glScalef(bedWidth-1,bedHeight-1,0.1);
                glTranslatef(-inithight,-initwight,-initdense);
                unitCube(0.3,0.16,0.1);
            glPopMatrix();

            //bed head box
            glPushMatrix();
                glTranslatef(0,bedHeight+coverHight+2,bedLength/2-headLength);
                glScalef(bedWidth-1,headHight-coverHight-1,0.1);
                glTranslatef(-inithight,-initwight,-initdense);
                unitCube(0.35,0.16,0.075);
            glPopMatrix();

            //bed head
            glPushMatrix();
                glTranslatef(0,bedHeight+headHight/2,bedLength/2-headLength/2);
                glScalef(bedWidth,headHight,headLength);
                glTranslatef(-inithight,-initwight,-initdense);
                unitCube(0.3,0.16,0.1);
            glPopMatrix();

            //bed cover
            glPushMatrix();
                glTranslatef(0,bedHeight+coverHight/2,-headLength/2);
                glScalef(bedWidth,coverHight,bedLength-1);
                glTranslatef(-inithight,-initwight,-initdense);
                unitCube(0.52,0.44,1);
            glPopMatrix();

            //bed lower
            glPushMatrix();
                glTranslatef(0,bedHeight/2,0);
                glScalef(bedWidth,bedHeight,bedLength);
                glTranslatef(-inithight,-initwight,-initdense);
                unitCube(0.5,0.22,0.14);
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}

void beds(){
    bed(-(roomWidth/2-walldepth-bedWidth/2),roomLength/2-walldepth-bedLength/2);
}

void dressingTable(float x, float z, int n){
    dressingTableLength = 2*n+(n+1)*0.5;

    glPushMatrix();
        glTranslatef(x,0,z);
        glPushMatrix();
        //upper part
        //mirror
            glPushMatrix();
                glTranslatef(dressingTableWidth/2-0.5,dressingTableHeight+3,0);
                glScalef(0.1,dressingTableHeight+1,dressingTableLength-2.5);
                glTranslatef(-inithight,-initwight,-initdense);
                unitCube(1,1,1);
            glPopMatrix();
        //mirror box
            glPushMatrix();
                glTranslatef(dressingTableWidth/2-0.25,dressingTableHeight+3,0);
                glScalef(0.5,dressingTableHeight+2,dressingTableLength-2);
                glTranslatef(-inithight,-initwight,-initdense);
                unitCube(0.5,0.22,0.14);
            glPopMatrix();
          //lower part
            glPushMatrix();
                glTranslatef(0,0,-dressingTableLength/2);
                glPushMatrix();
                    for(int i=0;i<n;i++){
                        float zshift = 1.5+i*2.5;
                        //lower drawer
                        glPushMatrix();
                            glTranslatef(-dressingTableWidth/2,dressingTableHeight/2,zshift);
                            glScalef(0.1,dressingTableHeight-1,dressingTableWidth);
                            glTranslatef(-inithight,-initwight,-initdense);
                            unitCube(0.165,0.09,0.059);
                        glPopMatrix();

                         //handle
                        glPushMatrix();
                            glTranslatef(-dressingTableWidth/2-0.1,dressingTableHeight/2,zshift+0.5);
                            glScalef(0.1,1,.2);
                            glTranslatef(-inithight,-initwight,-initdense);
                            unitCube(1,1,1);
                        glPopMatrix();

                    }
                    //lower part
                    glPushMatrix();
                        glTranslatef(0,dressingTableHeight/2,dressingTableLength/2);
                        glScalef(dressingTableWidth,dressingTableHeight,dressingTableLength);
                        glTranslatef(-inithight,-initwight,-initdense);
                        unitCube(0.5,0.22,0.14);
                    glPopMatrix();  //lower part
                glPopMatrix();// lower part with translation to z-axis
            glPopMatrix();//lower part back to origin
        glPopMatrix();//total dressing table
    glPopMatrix();
}

void dressingTables(){
    dressingTable(roomWidth/2-walldepth-dressingTableWidth/2,-2,3);
}

void portHole(float x,float y, float z,int n, int m,float angle=0){
    portHoleHeight = 2*n+(n+1)*0.25;
    portHoleWidth = 2*m+(m+1)*0.25;

    glPushMatrix();
        glTranslatef(x,y,z);
        glTranslatef(0,portHoleHeight/2,0);
        glRotatef(angle,0,1,0);
        glTranslatef(0,-portHoleHeight/2,0);
        glPushMatrix();
            glTranslatef(-portHoleWidth/2,0,0);
            glPushMatrix();
                for(int i = 0;i<n;i++){
                    for(int j = 0;j<m;j++){
                        float xshift = 1.25+j*2.25;
                        float yshift = 1.25+i*2.25;
                        //window frame
                        glPushMatrix();
                            glTranslatef(xshift,yshift,-0.1);
                            glScalef(2,2,0.05);
                            glTranslatef(-inithight,-initwight,-initdense);
                            unitCube(1,1,1);
                        glPopMatrix();
                    }
                }
                //window frame
                glPushMatrix();
                    glTranslatef(portHoleWidth/2,portHoleHeight/2,0);
                    glScalef(portHoleWidth,portHoleHeight,portHoleLength);
                    glTranslatef(-inithight,-initwight,-initdense);
                    unitCube(0,0,0);
                glPopMatrix();//frame
            glPopMatrix();//window
        glPopMatrix();//take it to it's origin
    glPopMatrix();// hole window in origin


}

void windows(){
    portHole(2,1+roomHeight/4,roomLength/2-walldepth,4,3);
    portHole(-(roomWidth/2-walldepth),1+roomHeight/4,roomLength/2-walldepth-portHoleWidth/2-4,3,2,-90);
}


void fan(float x,float y, float z){
    glPushMatrix();
        glTranslatef(x,y,z);
        glPushMatrix();
            glRotatef(fanAngle, 0,1,0);
            glPushMatrix();
                //blade 1
                glPushMatrix();
                    glTranslatef(0,0.5,-4);
                    glScalef(2,0.1,4);
                    glTranslatef(-inithight,-initwight,-initdense);
                    unitCube(0.94,1,1);
                glPopMatrix();
                //blade 2
                glPushMatrix();
                    glTranslatef(0,0.5,4);
                    glScalef(2,0.1,4);
                    glTranslatef(-inithight,-initwight,-initdense);
                    unitCube(0.94,1,1);
                glPopMatrix();
                //blade 3
                glPushMatrix();
                    glTranslatef(4,0.5,0);
                    glScalef(4,0.1,2);
                    glTranslatef(-inithight,-initwight,-initdense);
                    unitCube(0.94,1,1);
                glPopMatrix();
                //blade 4
                glPushMatrix();
                    glTranslatef(-4,0.5,0);
                    glScalef(4,0.1,2);
                    glTranslatef(-inithight,-initwight,-initdense);
                    unitCube(0.94,1,1);
                glPopMatrix();
                //side connector 1
                glPushMatrix();
                    glTranslatef(-2,0.5,0);
                    glScalef(1,0.5,0.5);
                    glTranslatef(-inithight,-initwight,-initdense);
                    unitCube(0.9,0.9,0.98);
                glPopMatrix();
                //side connector 2
                glPushMatrix();
                    glTranslatef(2,0.5,0);
                    glScalef(1,0.5,0.5);
                    glTranslatef(-inithight,-initwight,-initdense);
                    unitCube(0.9,0.9,0.98);
                glPopMatrix();
                //side connector 3
                glPushMatrix();
                    glTranslatef(0,0.5,2);
                    glScalef(0.5,0.5,1);
                    glTranslatef(-inithight,-initwight,-initdense);
                    unitCube(0.9,0.9,0.98);
                glPopMatrix();
                 //side connector 4
                glPushMatrix();
                    glTranslatef(0,0.5,-2);
                    glScalef(0.5,0.5,1);
                    glTranslatef(-inithight,-initwight,-initdense);
                    unitCube(0.9,0.9,0.98);
                glPopMatrix();
                //stand
                glPushMatrix();
                    glTranslatef(0,3,0);
                    glScalef(0.5,4,0.5);
                    glTranslatef(-inithight,-initwight,-initdense);
                    unitCube(0.18,0.31,0.31);
                glPopMatrix();
                //circle
                glPushMatrix();
                    glTranslatef(0,0.5,0);
                    glScalef(2,1,2);
                    glPushMatrix();
                        glRotatef(90,1,0,0);
                        glTranslatef(0,0,-0.5);
                        draw_cylinder(1,1,1,true,0,0.75,1);
                    glPopMatrix();
                glPopMatrix();//circle
            glPopMatrix();//total fan
        glPopMatrix();//translated
    glPopMatrix();//rotated
}


void fans(){
    fan(0,roomHeight-walldepth-5,0);
}


void watch(float x,float y, float z){
    glPushMatrix();
        glTranslatef(x,y,z);
        glPushMatrix();
            //hour hand
            glPushMatrix();
                glRotatef(hourAngle,0,0,1);
                glPushMatrix();
                    glTranslatef(0,0.25,-0.3);
                    glScalef(0.2,1,0.1);
                    glTranslatef(-inithight,-initwight,-initdense);
                    unitCube(0,0,0);
                glPopMatrix();
            glPopMatrix();
            //minute hand
            glPushMatrix();
                glRotatef(minuteAngle,0,0,1);
                glPushMatrix();
                    glTranslatef(0,0.3,-0.4);
                    glScalef(0.2,1.2,0.1);
                    glTranslatef(-inithight,-initwight,-initdense);
                    unitCube(0,0,0);
                glPopMatrix();
            glPopMatrix();
            glPushMatrix();
                //inside
                glPushMatrix();
                    glTranslatef(0,0,-0.2);
                    glScalef(2,2,0.1);
                    glTranslatef(-inithight,-initwight,-initdense);
                    unitCube(1,1,1);
                glPopMatrix();
                //frame
                glPushMatrix();
                    glScalef(2.5,2.5,0.2);
                    glTranslatef(-inithight,-initwight,-initdense);
                    unitCube(0.22,0.28,0.31);
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();//hole watch
    glPopMatrix();
}

void watches(){
    watch(-(roomWidth/2-bedWidth/2-walldepth)+1,3*(roomHeight/4)-1.5,(roomLength/2)-walldepth-0.1);
}

void tubelight(float x,float y,float z,bool emmission){
    glPushMatrix();
        glTranslatef(x,y,z);
        glPushMatrix();
            glScalef(0.5,0.5,5);
            glTranslatef(-inithight,-initwight,-initdense);
            unitCube(1,1,1,emmission);
        glPopMatrix();
    glPopMatrix();
}

void tubelights(){
    if(light0_On){
        tubelight(roomWidth/2-walldepth-0.25,3*(roomHeight/4),-4,true);
    }else{
        tubelight(roomWidth/2-walldepth-0.25,3*(roomHeight/4),-4,false);
    }
    if(light1_On){
        tubelight(-(roomWidth/2-walldepth-0.25),3*(roomHeight/4),-4,true);
    }else{
        tubelight(-(roomWidth/2-walldepth-0.25),3*(roomHeight/4),-4,false);
    }
}

void lamp(float x,float y,float z){
    glPushMatrix();
        glTranslatef(x,y,z);
        glPushMatrix();
            //circle
            glPushMatrix();
                glTranslatef(0,2,0);
                //glScalef(2,2,2);
                glPushMatrix();
                    glRotatef(-90,1,0,0);
                    glTranslatef(0,0,-1);
                    draw_cylinder(0.5,1,2,false,0.93,0.68,0.055);
                glPopMatrix();
            glPopMatrix();//circle
            //stand
            glPushMatrix();
                glTranslatef(0,1,0);
                glScalef(0.2,2,0.2);
                glTranslatef(-inithight,-initwight,-initdense);
                unitCube(0.65,0.165,0.165);
            glPopMatrix();//stand
        glPopMatrix();// lamp
    glPopMatrix();
}

void lamps(){
    lamp(-(roomWidth/2-walldepth-wearDropLength/2),wearDropHeight,-(bedLength-roomLength/2+walldepth+1+wearDropWidth/2));
}

void floordesign(){
    glPushMatrix();
        bool c = false;
        for(int i = 1;i<roomLength;i++){
            for(int j=1;j<roomWidth;j++){
                if(c){
                    glPushMatrix();
                        glTranslatef(roomWidth/2-j,0,roomLength/2-i);
                        glScalef(1,0.1,1);
                        glTranslatef(-inithight,-initwight,-initdense);
                        unitCube(0,0,0);
                    glPopMatrix();
                    c=false;
                }else{
                    glPushMatrix();
                        glTranslatef(roomWidth/2-j,0,roomLength/2-i);
                        glScalef(1,0.1,1);
                        glTranslatef(-inithight,-initwight,-initdense);
                        unitCube(1,1,1);
                    glPopMatrix();
                    c=true;
                }
            }
        }
    glPopMatrix();
}


void chair(float x, float z){
    glPushMatrix();
        glTranslatef(x,0,z);
        glPushMatrix();
            //support upper 1
            glPushMatrix();
                glTranslatef(-1.25,4,0);
                glScalef(0.5,0.5,2);
                glTranslatef(-inithight,-initwight,-initdense);
                unitCube(0.8,0.52,0.25);
            glPopMatrix();
            //support upper 1
            glPushMatrix();
                glTranslatef(-1.25,5,0);
                glScalef(0.5,0.5,2);
                glTranslatef(-inithight,-initwight,-initdense);
                unitCube(0.8,0.52,0.25);
            glPopMatrix();
            //support lower right
            glPushMatrix();
                glTranslatef(0,1,-1.25);
                glScalef(2,0.5,0.5);
                glTranslatef(-inithight,-initwight,-initdense);
                unitCube(0.8,0.52,0.25);
            glPopMatrix();
            //support lower left
            glPushMatrix();
                glTranslatef(0,1,1.25);
                glScalef(2,0.5,0.5);
                glTranslatef(-inithight,-initwight,-initdense);
                unitCube(0.8,0.52,0.25);
            glPopMatrix();
            //leg back right
            glPushMatrix();
                glTranslatef(-1.25,3,-1.25);
                glScalef(0.5,6,0.5);
                glTranslatef(-inithight,-initwight,-initdense);
                unitCube(0.8,0.52,0.25);
            glPopMatrix();
            //leg front left
            glPushMatrix();
                glTranslatef(1.25,1.5,1.25);
                glScalef(0.5,3,0.5);
                glTranslatef(-inithight,-initwight,-initdense);
                unitCube(0.8,0.52,0.25);
            glPopMatrix();
            //leg front right
            glPushMatrix();
                glTranslatef(1.25,1.5,-1.25);
                glScalef(0.5,3,0.5);
                glTranslatef(-inithight,-initwight,-initdense);
                unitCube(0.8,0.52,0.25);
            glPopMatrix();
            //leg back left
            glPushMatrix();
                glTranslatef(-1.25,3,1.25);
                glScalef(0.5,6,0.5);
                glTranslatef(-inithight,-initwight,-initdense);
                unitCube(0.8,0.52,0.25);
            glPopMatrix();
            //base
            glPushMatrix();
                glTranslatef(0,3.2,0);
                glScalef(3,0.4,3);
                glTranslatef(-inithight,-initwight,-initdense);
                unitCube(0.8,0.52,0.25);
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}

void chairs(){
    chair(roomWidth/2-walldepth-dressingTableWidth-4,-dressingTableLength/2+2);
}

void sphare(){
    glPushMatrix();
    glTranslatef(0,2,0);
    glScalef(2,2,2);
    glutSolidSphere(1,16,16);
    glPopMatrix();
}


void lighting(){
     //light
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = {0.2, 0.2, 0.2, 1.0};
    GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position0[] = { roomWidth/2-walldepth-0.25,3*(roomHeight/4),-4, 1.0 };
    GLfloat light_position1[] = {-(roomWidth/2-walldepth-0.25),3*(roomHeight/4),-4, 1.0 };
    GLfloat light_position2[] = { -(roomWidth/2-walldepth-wearDropLength/2),wearDropHeight+3,-(bedLength-roomLength/2+walldepth+1+wearDropWidth/2), 1.0 };

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv(GL_LIGHT2, GL_POSITION, light_position2);

    // spot light
    GLfloat spot_direction[] = { -(roomWidth/2-walldepth-wearDropLength/2),-wearDropHeight-3,-(bedLength-roomLength/2+walldepth+1+wearDropWidth/2) };
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
    glLightf( GL_LIGHT2, GL_SPOT_CUTOFF, 90.0);

    if(light0_On){
        if(no_ambient){
            glLightfv( GL_LIGHT0, GL_AMBIENT, no_light);
        }else{
            glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient);
        }
        if(no_diffuse){
            glLightfv( GL_LIGHT0, GL_DIFFUSE, no_light);
        }else{
            glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        }
        if(no_specular){
            glLightfv( GL_LIGHT0, GL_SPECULAR, no_light);
        }else{
            glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular);
        }
    }else{
        glLightfv( GL_LIGHT0, GL_AMBIENT, no_light);
        glLightfv( GL_LIGHT0, GL_DIFFUSE, no_light);
        glLightfv( GL_LIGHT0, GL_SPECULAR, no_light);
    }

    if(light1_On){
        if(no_ambient){
            glLightfv( GL_LIGHT1, GL_AMBIENT, no_light);
        }else{
            glLightfv( GL_LIGHT1, GL_AMBIENT, light_ambient);
        }
        if(no_diffuse){
            glLightfv( GL_LIGHT1, GL_DIFFUSE, no_light);
        }else{
            glLightfv( GL_LIGHT1, GL_DIFFUSE, light_diffuse);
        }
        if(no_specular){
            glLightfv( GL_LIGHT1, GL_SPECULAR, no_light);
        }else{
            glLightfv( GL_LIGHT1, GL_SPECULAR, light_specular);
        }
    }else{
        glLightfv( GL_LIGHT1, GL_AMBIENT, no_light);
        glLightfv( GL_LIGHT1, GL_DIFFUSE, no_light);
        glLightfv( GL_LIGHT1, GL_SPECULAR, no_light);
    }

    if(light2_On){
        if(no_ambient){
            glLightfv( GL_LIGHT2, GL_AMBIENT, no_light);
        }else{
            glLightfv( GL_LIGHT2, GL_AMBIENT, light_ambient);
        }
        if(no_diffuse){
            glLightfv( GL_LIGHT2, GL_DIFFUSE, no_light);
        }else{
            glLightfv( GL_LIGHT2, GL_DIFFUSE, light_diffuse);
        }
        if(no_specular){
            glLightfv( GL_LIGHT2, GL_SPECULAR, no_light);
        }else{
            glLightfv( GL_LIGHT2, GL_SPECULAR, light_specular);
        }
    }else{
        glLightfv( GL_LIGHT2, GL_AMBIENT, no_light);
        glLightfv( GL_LIGHT2, GL_DIFFUSE, no_light);
        glLightfv( GL_LIGHT2, GL_SPECULAR, no_light);
    }
}


rotatePosition rotateAroundLookAt(bool anticlock,float Ax,float Ay,float Aangle)
{
    int x;
    if(anticlock){
        Aangle+=11.25;
    }else{
        Aangle-=11.25;
    }

    if(Aangle>=360){
        Aangle = 0;
    }else if(Aangle<=0){
        Aangle = 360;
    }

    if(Aangle>0 && Aangle<=90){
        if(anticlock){
            x=1;
        }else{
            x=3;
        }

    }else if(Aangle>90 && Aangle<=180){
        if(anticlock){
            x=2;
        }else{
            x=4;
        }
    }else if(Aangle>180 && Aangle<=270){
        if(anticlock){
            x=3;
        }else{
            x=1;
        }
    }else{
        if(anticlock){
            x=4;
        }else{
            x=2;
        }
    }
    switch(x)
    {
    case 1:
        Ax+=0.5;
        Ay+=0.5;
        break;
    case 2:
        Ax-=0.5;
        Ay+=0.5;
        break;
    case 3:
        Ax-=0.5;
        Ay-=0.5;
        break;
    case 4:
        Ax+=0.5;
        Ay-=0.5;
        break;
    }

    //cout<<Aangle<<" "<<Ax<<" "<<Ay<<endl;

    rotatePosition rpos;
    rpos.position1=Ax;
    rpos.position2=Ay;
    rpos.pangle = Aangle;
    return rpos;
}

void test(){
    glPushMatrix();
    glTranslatef(0,2,0);
    glScalef(2,2,2);
    glutSolidSphere(1,16,16);
    glPopMatrix();
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-3, 3, -3, 3, 2.0, 50.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX,eyeY,eyeZ, lookX,lookY,lookZ, 0,1,0);
    //glViewport(0, 0, windowWidth, windowheight);

    glRotatef(rotationYAngle, 0,1,0);
    glRotatef(rotationXAngle, 1,0,0);
    axes();

    //building();
    //weardrops();
    //beds();
    //dressingTables();
    //windows();
    //fans();
    //watches();
    //tubelights();
    //lamps();
    //floordesign();
    //chairs();
    //lighting();
    //sphare();
    //draw_cylinder(1,2,2);
    test();



    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    rotatePosition rpos;
    switch (key)
    {
    case 27 :
    case 'q':
        exit(0);
        break;
    case '+':
        lookZ +=0.5;
        eyeZ +=0.5;
        break;
    case '-':
        lookZ -=0.5;
        eyeZ -=0.5;
        break;
    case 'A':
    case 'a':
        if(cameraMode){
            rpos = rotateAroundLookAt(false,eyeX,eyeZ,eyeAngle);
            eyeAngle = rpos.pangle;
            eyeX = rpos.position1;
            eyeZ = rpos.position2;
        }else{
            rpos = rotateAroundLookAt(false,lookX,lookZ,lookAngleX);
            lookAngleX = rpos.pangle;
            lookX = rpos.position1;
            lookZ = rpos.position2;
        }
        break;
    case 'D':
    case 'd':
        if(cameraMode){
            rpos = rotateAroundLookAt(true,eyeX,eyeZ,eyeAngle);
            eyeAngle = rpos.pangle;
            eyeX = rpos.position1;
            eyeZ = rpos.position2;
        }else{
            rpos = rotateAroundLookAt(true,lookX,lookZ,lookAngleX);
            lookAngleX = rpos.pangle;
            lookX = rpos.position1;
            lookZ = rpos.position2;
        }
        break;
    case 'R':
    case 'r':
        cameraMode = !cameraMode;
        break;
    case 'W':
    case 'w':
        rpos = rotateAroundLookAt(false,lookY,lookZ,lookAngleY);
        lookAngleY = rpos.pangle;
        lookY = rpos.position1;
        lookZ = rpos.position2;
        break;
    case 'S':
    case 's':
        rpos = rotateAroundLookAt(true,lookY,lookZ,lookAngleY);
        lookAngleY = rpos.pangle;
        lookY = rpos.position1;
        lookZ = rpos.position2;
        break;
    case 'T':
    case 't':
        fanRotate= !fanRotate;
        break;
    case ',':
        rotationYAngle+=2;
        break;
    case '.':
        rotationYAngle-=2;
        break;
    case '1':
        light0_On=!light0_On;

        break;
    case '2':
        light1_On=!light1_On;
        break;
    case '3':
        light2_On=!light2_On;
        break;
    case '4':
        no_ambient=!no_ambient;
        break;
    case '5':
        no_diffuse=!no_diffuse;
        break;
    case '6':
        no_specular=!no_specular;
        break;
    case 'z':
        rotationXAngle+=2;
        break;
    case 'x':
        rotationXAngle-=2;
        break;
    }

    glutPostRedisplay();
}

static void idle(void)
{
    if (fanRotate == true)
    {
        fanAngle += 0.8;
        if(fanAngle > 360.0)
            fanAngle -= 360.0*floor(fanAngle/360.0);
    }

    if (fanRotate == true)
    {
        minuteAngle += 0.2;
        if(minuteAngle > 360.0)
            minuteAngle -= 360.0*floor(minuteAngle/360.0);
    }

    if (fanRotate == true)
    {
        hourAngle += 0.05;
        if(hourAngle > 360.0)
            hourAngle -= 360.0*floor(hourAngle/360.0);
    }

    glutPostRedisplay();
}

static void windowResize(int width, int height)
{
    glViewport((width-windowWidth)/2, 0, height, height);
    //cout<<width<<" "<<height<<endl;
}

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(windowWidth,windowheight);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("1607005");

    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    glutReshapeFunc(windowResize);

//    glClearColor(1,1,1,1);

    glEnable(GL_DEPTH_TEST);
    glShadeModel( GL_SMOOTH );
    glEnable(GL_NORMALIZE);
    glEnable(GL_BLEND);
    glEnable(GL_LIGHTING);

    cout<<"Press + to zoom in"<<endl;
    cout<<"Press - to zoom out"<<endl;
    cout<<"Press a to move left"<<endl;
    cout<<"Press d to move right"<<endl;
    cout<<"Press w to move up"<<endl;
    cout<<"Press s to move down"<<endl;
    cout<<"Press q to exit"<<endl;
    cout<<"Press t to stop/start rotation"<<endl;
    cout<<"Press r to toggle camera mode"<<endl;
    cout<<"Press 1 to toggle Light 1"<<endl;
    cout<<"Press 2 to toggle Light 2"<<endl;
    cout<<"Press 3 to toggle Spot light"<<endl;
    cout<<"Press 4 to toggle ambient light"<<endl;
    cout<<"Press 5 to toggle diffuse light"<<endl;
    cout<<"Press 6 to toggle specular light"<<endl;


    glutMainLoop();

    return EXIT_SUCCESS;
}
