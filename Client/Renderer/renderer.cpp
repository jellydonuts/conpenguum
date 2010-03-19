#include "renderer.h"
#include <QtGlobal>
#include <QPainter>
#include <QtGui>
#include <QGLWidget>
#include <QtOpenGL>
#include "matrix.h"
#include <cmath>
#include <vector>
#include <cassert>

using namespace std;

Renderer::Renderer(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers|QGL::AlphaChannel), parent)
{

}

void Renderer::buildRenderList(vector<UpdateObject> objectlist)
{
    //if we have more than 40k render objects, we're fucked.
    assert(MAXRENDERCOUNT > objectlist.size()/*+map objects*/);

    int i, xOffset, yOffset;

    //grab the first render object, this should be the client's ship, and thus our center
    xOffset = SCRCENTREW - objectlist[0].getPos().x();
    yOffset = SCRCENTREH - objectlist[0].getPos().y();

    renderList[0].texture = textures[0];
    renderList[0].x = objectlist[0].getPos().x+xOffset;
    renderList[0].y = objectlist[0].getPos().y+yOffset;
    renderList[0].rotation = objectlist[0].getRotation()*2;
    renderList[0].texOffsetX = 0;
    renderList[0].texOffsetY = 3/32.0;
    renderList[0].objectHeight = 1/32.0;
    renderList[0].objectWidth = 1/10.0;
    renderList[0].objectHeightPx = 50;
    renderList[0].objectWidthPx = 50;

    for(i = 1; i < objectlist.size(); i++)
    {
        if(objectlist[i].getRotation() == -1)
        {
            renderList[i].texture = textures[1];
            renderList[i].texOffsetX = 0;
            renderList[i].texOffsetY = 0;
            renderList[i].objectHeight = 1/10.0;
            renderList[i].objectWidth = 1/4.0;
            renderList[i].rotation = 0;
            renderList[i].objectHeightPx=16;
            renderList[i].objectWidthPx=16;
        }
        else if(objectlist[i].getRotation() == -2)
        {
            renderList[i].texture = textures[2];
            renderList[i].texOffsetX = 0;
            renderList[i].texOffsetY = 0;
            renderList[i].objectHeight = 1;
            renderList[i].objectWidth = 1;
            renderList[i].rotation = 0;
            renderList[i].objectHeightPx = 25;
            renderList[i].objectWidthPx = 25;
        }
        else
        {
            renderList[i].texture = textures[0];
            renderList[i].texOffsetX = 0;
            renderList[i].texOffsetY = 3/32.0;
            renderList[i].objectHeight = 1/32.0;
            renderList[i].objectWidth = 1/10.0;
            renderList[i].rotation = objectlist[i].getRotation()*2;
            renderList[i].objectHeightPx = 50;
            renderList[i].objectWidthPx = 50;
        }

        renderList[i].x = xOffset + objectlist[i].pos_.x();
        renderList[i].y = yOffset + objectlist[i].pos_.y();
    }
    renderCount = i;
}


void Renderer::Initialize()
{
    renderCount = 0;
    makeCurrent();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);

//    QDir dir(":/textures/");
//    QFileInfoList list = dir.entryInfoList();
//    for(int i = 0; i < list.size(); i++)
//    {
//        QFileInfo fileInfo = list.at(i);
//        QString str = fileInfo.fileName();
//        const char *buf = str.toStdString().c_str();
//        int foo = 5;
//    }

    textures[0] = bindTexture(QPixmap(QString(":/textures/ships.bmp"),"BMP"), GL_TEXTURE_2D);
    textures[1] = bindTexture(QPixmap(QString(":/textures/bullets.bmp"),"BMP"), GL_TEXTURE_2D); //10 high 4 wide;
    textures[2] = bindTexture(QPixmap(QString(":/textures/bg01.bmp"),"BMP"), GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void Renderer::resizeGL(int w, int h)
{
    glViewport(0,0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 30.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f,0.0f,-3.6f);
    gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble) h );
}

void Renderer::Render()
{
    //clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    //glBindTexture(GL_TEXTURE_2D, texName);

    for(int i = 0; i < renderCount; i++)
    {
        Matrix rot;
        Matrix quad;
        Matrix txtr;
        Matrix trans;

        //set the 4 points of the ship
        //bottom left
        quad.points.push_back(vector<double>());
        quad.points[0].push_back(renderList[i].x);
        quad.points[0].push_back(renderList[i].y);
        quad.points[0].push_back(1);
        //bottom right
        quad.points.push_back(vector<double>());
        quad.points[1].push_back(renderList[i].objectWidthPx + renderList[i].x);
        quad.points[1].push_back(renderList[i].y);
        quad.points[1].push_back(1);
        //top right
        quad.points.push_back(vector<double>());
        quad.points[2].push_back(renderList[i].objectWidthPx + renderList[i].x);
        quad.points[2].push_back(renderList[i].objectHeightPx + renderList[i].y);
        quad.points[2].push_back(1);
        //top left
        quad.points.push_back(vector<double>());
        quad.points[3].push_back(renderList[i].x);
        quad.points[3].push_back(renderList[i].objectHeightPx + renderList[i].y);
        quad.points[3].push_back(1);
        if(renderList[i].rotation > 0)//skip all this if the object is rotationless or at 0;
        {
            //translate to origin
            trans.points.push_back(vector<double>());
            trans.points[0].push_back(1);
            trans.points[0].push_back(0);
            trans.points[0].push_back(0);
            trans.points.push_back(vector<double>());
            trans.points[1].push_back(0);
            trans.points[1].push_back(1);
            trans.points[1].push_back(0);
            trans.points.push_back(vector<double>());
            trans.points[2].push_back(-renderList[i].x);
            trans.points[2].push_back(-renderList[i].y);
            trans.points[2].push_back(1);

            quad = quad*trans;

            //setup the rotation matrix
            rot.points.push_back(vector<double>());
            rot.points[0].push_back(cos(DEGTORAD(renderList[i].rotation)));
            rot.points[0].push_back(sin(DEGTORAD(renderList[i].rotation)));
            rot.points[0].push_back(0);
            rot.points.push_back(vector<double>());
            rot.points[1].push_back(-sin(DEGTORAD(renderList[i].rotation)));
            rot.points[1].push_back(cos(DEGTORAD(renderList[i].rotation)));
            rot.points[1].push_back(0);
            rot.points.push_back(vector<double>());
            rot.points[2].push_back(0);
            rot.points[2].push_back(0);
            rot.points[2].push_back(1);

            quad = quad*rot;

            trans.points[2][0]=renderList[i].x;
            trans.points[2][1]=renderList[i].y;
            quad = quad*trans;
        }

        glBindTexture(GL_TEXTURE_2D, renderList[i].texture);

        glBegin(GL_QUADS);//begin points
            glTexCoord2f(renderList[i].texOffsetX, renderList[i].texOffsetY); glVertex2f(quad.points[0][0], quad.points[0][1]);
            glTexCoord2f(renderList[i].texOffsetX+renderList[i].objectWidth, renderList[i].texOffsetY); glVertex2f(quad.points[1][0], quad.points[1][1]);
            glTexCoord2f(renderList[i].texOffsetX+renderList[i].objectWidth, renderList[i].texOffsetY+renderList[i].objectHeight); glVertex2f(quad.points[2][0], quad.points[2][1]);
            glTexCoord2f(renderList[i].texOffsetX, renderList[i].texOffsetY+renderList[i].objectHeight); glVertex2f(quad.points[3][0], quad.points[3][1]);
        glEnd();
    }



    glFlush();
    glDisable(GL_TEXTURE_2D);

    //TODO: This is the text rendering part, this should be moved out to it's own function
    /*QPainter p(this); // used for text overlay
    saveGLState();
    // draw the overlayed text using QPainter
    p.setPen(QColor(197, 197, 197, 157));
    p.setBrush(QColor(197, 197, 197, 127));
    p.drawRect(QRect(0, 0, width(), 50));
    p.setPen(Qt::black);
    p.setBrush(Qt::NoBrush);
    const QString str1(tr("A simple OpenGL framebuffer object example."));
    const QString str2(tr("Use the mouse wheel to zoom, press buttons and move mouse to rotate, double-click to flip."));
    QFontMetrics fm(p.font());
    p.drawText(width()/2 - fm.width(str1)/2, 20, str1);
    p.drawText(width()/2 - fm.width(str2)/2, 20 + fm.lineSpacing(), str2);*/
    updateGL();
}


void Renderer::saveGLState()
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
}

void Renderer::restoreGLState()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glPopAttrib();
}