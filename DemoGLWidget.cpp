#include "DemoGLWidget.h"
#include <QPainter>
#include <QPaintEngine>
#include <math.h>

#include "config.h"
#include "Utils.h"
#include "Entity.h"
#include "critter.h"
#include "particle.h"
#include "synth.h"

#define WIDTH 512
#define HEIGHT 512

GLint dist = 1;
GLfloat kernels[7][9] = {
        { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f}, /* Identity */
        { 0.0f,-1.0f, 0.0f,-1.0f, 5.0f,-1.0f, 0.0f,-1.0f, 0.0f}, /* Sharpen */
        { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f}, /* Blur */
        { 1.0f, 2.0f, 1.0f, 2.0f, 4.0f, 2.0f, 1.0f, 2.0f, 1.0f}, /* Gaussian blur */
        { 0.0f, 0.0f, 0.0f,-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f}, /* Edge enhance */
        { 1.0f, 1.0f, 1.0f, 1.0f, 8.0f, 1.0f, 1.0f, 1.0f, 1.0f}, /* Edge detect */
        { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,-1.0f}  /* Emboss */
};


DemoGLWidget::DemoGLWidget(QWidget *parent)
    : QGLWidget(parent),
    entity1(0), entity2(0), entity3(0), entity4(0)
{
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setAutoBufferSwap(false);
#ifndef Q_WS_QWS
    setMinimumSize(300, 250);
#endif

    frames = 0;
    randomTarget();

    // Init FBO
    /*
    makeCurrent();
    if (QGLFramebufferObject::hasOpenGLFramebufferBlit()) {
        QGLFramebufferObjectFormat format;
        format.setSamples(4);
        format.setAttachment(QGLFramebufferObject::CombinedDepthStencil);

        render_fbo = new QGLFramebufferObject(512, 512, format);
        texture_fbo = new QGLFramebufferObject(512, 512);
    } else {
        render_fbo = new QGLFramebufferObject(1024, 1024);
        texture_fbo = render_fbo;
    }
*/

    /*
    fbo->width = WIDTH;
    fbo->height = HEIGHT;
    Utils::InitFBO(fbo);
    */

    // Init shaders
    /*
    QGLShader *vshader2 = new QGLShader(QGLShader::Vertex);
    const char *vsrc2 =
            "void main(void) \n"
            "{"
            "        gl_Position = ftransform();"
            "        gl_TexCoord[0] = gl_MultiTexCoord0;"
            "}";
    vshader2->compileSourceCode(vsrc2);

    QGLShader *fshader2 = new QGLShader(QGLShader::Fragment);
    const char *fsrc2 =
            "uniform int Width, Height;"
            "uniform int Dist;"
            "uniform mat3 Kernel;"
            "uniform sampler2D Tex0; \n"
            "void main (void)"
            "{"
            "        int i, j;"
            "        vec2 coord;"
            "        float contrib = 0.0;"
            "        vec4 sum = vec4(0.0);"
            "        // 3x3 convolution matrix"
            "        for(i = -1; i <= 1; i++) "
            "                for(j = -1; j <= 1; j++) {"
            "                        coord = gl_TexCoord[0].st + vec2(float(i) * (1.0/float(Width)) * float(Dist), float(j) * (1.0/float(Height)) * float(Dist));"
            "                        sum += Kernel[i+1][j+1] * texture2D(Tex0, coord.xy);"
            "                        contrib += Kernel[i+1][j+1];"
            "                }"
            "        gl_FragColor = sum/contrib;"
            "}";

    fshader2->compileSourceCode(fsrc2);

    program.addShader(vshader2);
    program.addShader(fshader2);
    program.link();
    */
}

DemoGLWidget::~DemoGLWidget()
{
}

void DemoGLWidget::initializeGL ()
{
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);

    if(!USE_SOUND) createEntities(NUM_ENTITIES);

    // Particles
    for (int i = 0; i < 100; ++i) {
        QPointF position(width()*((qrand()/(RAND_MAX+1.0))),
                         height()*((qrand()/(RAND_MAX+1.0))));
        qreal radius = qMin(7.0, 15 * qrand()/(RAND_MAX+1.0));
        qreal velocity = qMin(5.0, 7 * qrand()/(RAND_MAX+1.0));

        particles.append(new Particle(position, radius, velocity));
    }
}

void DemoGLWidget::paintGL()
{
    // Render scene to FBO
    //render_fbo->bind();
    //glBindFramebuffer(GL_FRAMEBUFFER, fbo->frame);


    QPainter painter;
    painter.begin(this);
    Utils::setPainter(&painter);

    painter.beginNativePainting();

    float r = 0.05f;
    float g = 0.05f;
    float b = 0.2f;

    // Flash screen on snare hit
    float snareEnv = synth_get_current_envelope_for_instrument(4);

    if (snareEnv > 0.001f)
    {
        r += snareEnv * 0.2f;
        g += snareEnv * 0.2f;
        b += snareEnv * 0.2f;
    }

    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);

    painter.endNativePainting();


    // Create entities when an instrument is first played
    if (synth_get_current_note_for_instrument(0) != 0 &&
        critter == 0)
    {
        critter = new Critter(QPointF(500,200), 10, CRITTER_SPEED);
    }

    if (synth_get_current_note_for_instrument(1) != 0 &&
        entity1 == 0)
    {
        entity1 = new Entity(QPointF(300,200), 9, 4, 1);
        entities.append(entity1);
    }

    if (synth_get_current_note_for_instrument(2) != 0 &&
        entity2 == 0)
    {
        entity2 = new Entity(QPointF(300,200), 8, 3, 2);
        entities.append(entity2);
    }

    if (synth_get_current_note_for_instrument(3) != 0 &&
        entity3 == 0)
    {
        entity3 = new Entity(QPointF(300,200), 8, 3, 3);
        entities.append(entity3);
    }

    if (synth_get_current_note_for_instrument(4) != 0 &&
        entity4 == 0)
    {
        entity4 = new Entity(QPointF(300,200), 8, 3, 4);
        entities.append(entity4);
    }


    // Draw Entities and Critter    
    painter.setBrush(QBrush());

    foreach (Entity *entity, entities)
    {
        entity->drawEntity(&painter);
    }
    critter->drawCritter(&painter);

    foreach (Particle *particle, particles)
    {
        particle->draw(&painter);
    }

    // Display FPS
    QString framesPerSecond;
    framesPerSecond.setNum(frames /(time.elapsed() / 1000.0), 'f', 2);
    painter.setPen(Qt::white);
    painter.drawText(20, 40, framesPerSecond + " fps");

    // Update critter location
    critter->clearSteering();
    critter->steerForWander(STEER_WANDER_STRENGTH);
    bool reached = critter->steerToTarget(targetLocation, STEER_TO_TARGET_STRENGTH);
    if(reached) randomTarget();
    critter->move();
    critter->updateColor();

    // Move entities
    for(int i = 0; i < entities.size(); i++)
    {
        Entity *entity = entities[i];

        // Use all entities as local flock for now
        QMutableListIterator<Entity*> localFlock(entities);

        entity->clearSteering();
        entity->steerToTarget(critter->pos(), STEER_TO_TARGET_STRENGTH);
        entity->steerWithFlock(localFlock, STEER_SEPARATION_STRENGTH, STEER_COHESION_STRENGTH);
        entity->steerToAvoindWithinDistance(critter->pos(), STEER_AVOID_WITHIN_DISTANCE_STRENGTH);
        entity->move();
    }

    // Post processing
    /*
    int effectIndex = 3;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, fbo->texid);
    int loc = glGetUniformLocation(program.programId(), "Width");
    glUniform1i(loc, WIDTH);
    loc = glGetUniformLocation(program.programId(), "Height");
    glUniform1i(loc, HEIGHT);
    loc = glGetUniformLocation(program.programId(), "Dist");
    glUniform1i(loc, dist);
    loc = glGetUniformLocation(program.programId(), "Kernel");
    GLfloat* kernelMatrix = (GLfloat*)kernels;
    glUniformMatrix3fv(loc, 1, GL_FALSE, &kernelMatrix[effectIndex]);

    // TODO: Render fbo
    //painter.drawImage(rect(), render_fbo->toImage());
    */

    swapBuffers();
    painter.end();
    if (!(frames % 100)) {
        time.start();
        frames = 0;
    }
    frames ++;
}

void DemoGLWidget::createEntities(int number)
{
    for (int i = 0; i < number; ++i) {
        QPointF position(width()*(0.1 + (0.8*qrand()/(RAND_MAX+1.0))),
                        height()*(0.1 + (0.8*qrand()/(RAND_MAX+1.0))));
        qreal radius = qMin(width(), height())*(0.0175 + 0.0875*qrand()/(RAND_MAX+1.0));
        qreal velocity = ENTITY_SPEED;

        entities.append(new Entity(position, radius, velocity, 0));
    }    

    critter = new Critter(QPointF(500,200), 10, CRITTER_SPEED);
}

void DemoGLWidget::randomTarget()
{
    targetLocation = QPointF(width()*(0.1 + (0.8*qrand()/(RAND_MAX+1.0))),
                    height()*(0.1 + (0.8*qrand()/(RAND_MAX+1.0))));
    if(DEBUG) printf("RandomSteerToTarget: (%.2f, %.2f)\n", targetLocation.x(), targetLocation.y());
}

void DemoGLWidget::mouseMoveEvent (QMouseEvent* event)
{
    targetLocation = QPointF(event->x(), event->y());
    event->accept();

    critter->setExpandingColor(Utils::randomColor());
}

