#include "Utils.h"

#include <QVector2D>
#include <math.h>
#include <QtOpenGL>

QPainter* Utils::painter = 0;
const qreal PI = 3.14159;

void Utils::setPainter(QPainter* painter)
{
    Utils::painter = painter;
}

void Utils::clearPainter()
{
    Utils::painter = 0;
}

void Utils::DrawLine(QPointF from, QPointF to)
{
    if(painter)
    {
        painter->beginNativePainting();
        painter->setPen(QPen(Qt::white));
        painter->drawLine(from, to);
        painter->endNativePainting();
    }
}

void Utils::DrawCircle(QPointF pos, qreal radius)
{
    painter->beginNativePainting();
    painter->setPen(QPen(Qt::white));
    painter->drawEllipse(pos, radius, radius);
    painter->endNativePainting();
}

void Utils::DrawCircle(QPointF pos, qreal radius, QBrush brush)
{
    painter->beginNativePainting();
    painter->setBrush(brush);
    painter->drawEllipse(pos, radius, radius);
    painter->endNativePainting();
}

QColor Utils::randomColor()
{
    int red = int(185 + 70.0*qrand()/(RAND_MAX+1.0));
    int green = int(185 + 70.0*qrand()/(RAND_MAX+1.0));
    int blue = int(205 + 50.0*qrand()/(RAND_MAX+1.0));
    int alpha = int(91 + 100.0*qrand()/(RAND_MAX+1.0));

    return QColor(red, green, blue, alpha);
}

// Angle in degrees from positive Y-axis in range [-180, 180]
qreal Utils::VectorToAngle(QVector2D vec)
{
    qreal x = vec.x();
    qreal y = vec.y();
    if(x > 0)
    {
       if(y > 0)
       {
            // Upper-Right 0..90
           return atan(x/y) * 180 / PI;
       }
       else
       {
           // Lower-Right 90..180
           return 180.0 * (1.0 - atan(x/-y)/PI);  // 180 - atan(x/y)*180/PI
       }
    }
    else
    {
        // Same as right side, use reverse x and negate the result
        if(y > 0)
        {
             // Upper-Left 0..-90
            return -(atan(-x/y) * 180 / PI);
        }
        else
        {
            // Lower-Left -90..-180
            return -(180.0 * (1.0 - atan(-x/-y)/PI));  // 180 - atan(x/y)*180/PI
        }
    }
}

// Returns unit vector that points from given angle away from positive Y
// (given in [-180, 180] degrees)
QVector2D Utils::AngleToVector(qreal angle)
{
    qreal radAngle = angle / 180 * PI;
    return QVector2D(sin(radAngle), cos(radAngle));
}

void Utils::InitFBO(FBO *fbo)
{
        glGenFramebuffers(1, &(fbo->frame));
        glBindFramebuffer(GL_FRAMEBUFFER, fbo->frame);

        glGenRenderbuffers(1, &(fbo->depth));
        glBindRenderbuffer(GL_RENDERBUFFER, fbo->depth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, fbo->width, fbo->height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo->depth);

        printf("GL Error: %d\n", glGetError());

        glGenTextures(1, &(fbo->texid));
                printf("GL Error: %d\n", glGetError());
        glBindTexture(GL_TEXTURE_2D, fbo->texid);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA4,  fbo->width, fbo->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        printf("GL Error: %d\n", glGetError());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo->texid, 0);


        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
                printf("FBO Error: %d\n", status);
                //exit(-1);
        }
}

void Utils::DeleteFBO(FBO *fbo)
{
        glDeleteTextures(1, &(fbo->texid));;
        glDeleteRenderbuffers(1, &(fbo->depth));
        glDeleteFramebuffers(1, &(fbo->frame));
}
