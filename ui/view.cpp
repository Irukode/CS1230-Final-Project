#include "view.h"

#include "viewformat.h"
#include <QApplication>
#include <QKeyEvent>
#include <iostream>
#include "cs123_lib/sphere.h"
#include "gl/shaders/ShaderAttribLocations.h"
#include "lib/resourceloader.h"
#include "shapes/openglshape.h"
#include "gl/textures/Texture2D.h"
#include "Settings.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <glm/gtx/string_cast.hpp>

View::View(QWidget *parent) : QGLWidget(ViewFormat(), parent),
    m_time(), m_timer(), m_captureMouse(false), m_width(width()), m_height(height()),
    m_quad(nullptr), m_sphere(nullptr),
    m_blurFBO1(nullptr), m_blurFBO2(nullptr),
    m_program(0), m_phongprogram(0),
    m_angleX(-0.5f), m_angleY(0.5f), m_zoom(4.f), m_fps(60.0f),
    m_increment(0)
{
    // View needs all mouse move events, not just mouse drag events
    setMouseTracking(false);

    // Hide the cursor
    if (m_captureMouse) {
        QApplication::setOverrideCursor(Qt::BlankCursor);
    }

    // View needs keyboard focus
    setFocusPolicy(Qt::StrongFocus);

    // The update loop is implemented using a timer
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));
}

View::~View()
{
}

void View::initializeGL() {
    // All OpenGL initialization *MUST* be done during or after this
    // method. Before this method is called, there is no active OpenGL
    // context and all OpenGL calls have no effect.
    ResourceLoader::initializeGlew();

    // Start a timer that will try to get 60 frames per second (the actual
    // frame rate depends on the operating system and other running programs)
    m_time.start();
    m_timer.start(1000 / 60);

    // Set the color to set the screen when the color buffer is cleared.scale
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Creates the shader program that will be used for drawing.
    m_program = ResourceLoader::createShaderProgram(":shaders/quad.vert", ":shaders/default.frag");

    // Initialize sphere with radius 0.5 centered at origin.
    std::vector<GLfloat> sphereData = SPHERE_VERTEX_POSITIONS;
    m_sphere = std::make_unique<OpenGLShape>();
    m_sphere->setVertexData(&sphereData[0], sphereData.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES, NUM_SPHERE_VERTICES);
    m_sphere->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_sphere->buildVAO();

    std::vector<GLfloat> quadData = {-1.f, 1.f, 0.f, 0.f, 1.f,
                                    -1.f, -1.f, 0.f, 0.f, 0.f,
                                    1.f, 1.f, 0.f, 1.f, 1.f,
                                    1.f, -1.f, 0.f, 1.f, 0.f};
    m_quad = std::make_unique<OpenGLShape>();
    m_quad->setVertexData(&quadData[0], quadData.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, 4);
    m_quad->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_quad->setAttribute(ShaderAttrib::TEXCOORD0, 2, 3*sizeof(GLfloat), VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_quad->buildVAO();

    m_sphereTrans = std::make_unique<glm::mat4[]>(max_spheres);
    m_sphereBool = std::make_unique<bool[]>(max_spheres);
    for(int i = 0; i < max_spheres; i++) {
        m_sphereTrans[i] = glm::mat4(1.f);
        m_sphereBool[i] = true;
    }
    m_sphereTrans[1] = glm::translate(glm::vec3(0,1.1,0)) * glm::scale(glm::vec3(.75, .75, .75)) * m_sphereTrans[1];
    m_sphereTrans[2] = glm::translate(glm::vec3(0,-1.1,0)) * glm::scale(glm::vec3(.75, .75, .75))  * m_sphereTrans[2];
    m_sphereTrans[3] = glm::translate(glm::vec3(1.1,0,0)) * glm::scale(glm::vec3(.5, .5, .5))  * m_sphereTrans[3];
    m_sphereTrans[4] = glm::translate(glm::vec3(-1.1,0,0)) * glm::scale(glm::vec3(.5, .5, .5))  * m_sphereTrans[4];
    m_sphereTrans[5] = glm::translate(glm::vec3(0,0,1.1)) * glm::scale(glm::vec3(.25, .25, .25))  * m_sphereTrans[5];
    m_sphereTrans[6] = glm::translate(glm::vec3(0,0,-1.1)) * glm::scale(glm::vec3(.25, .25, .25))  * m_sphereTrans[6];

}

void View::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // TODO: Implement the demo rendering here
    glUseProgram(m_program);
    for(int i = 0; i < max_spheres; i++) {
        if(m_sphereTrans[i][0][0] >= 1.0f){
            m_sphereBool[i] = true;
        }
        if(m_sphereTrans[i][0][0] <= .25f){
            m_sphereBool[i] = false;
        }
        if(m_sphereBool[i]) {
            m_sphereTrans[i] = glm::scale(glm::vec3(.99,.99,.99)) * m_sphereTrans[i];
        } else {
            m_sphereTrans[i] = glm::scale(glm::vec3(1.01, 1.01, 1.01)) * m_sphereTrans[i];
        }
    }
    glm::vec4 camera = glm::vec4(0.f, 0.f, 0.f, 1.f);
    glm::vec2 uResolution(m_width, m_height);
    glUniformMatrix4fv(glGetUniformLocation(m_program, "cam2world"), 1, GL_FALSE, glm::value_ptr(glm::inverse(m_view)));
    glUniform4fv(glGetUniformLocation(m_program, "eye"), 1, glm::value_ptr(camera));
    glUniform2fv(glGetUniformLocation(m_program, "uResolution"), 1, glm::value_ptr(uResolution));
    glUniformMatrix4fv(glGetUniformLocation(m_program, "Spheres"), max_spheres, GL_FALSE, glm::value_ptr(m_sphereTrans[0]));
    rebuildMatrices();

    glViewport(0,0,m_width, m_height);

    m_quad->draw();
    glUseProgram(0);
}

void View::resizeGL(int w, int h) {
    m_width = w;
    m_height = h;

    rebuildMatrices();
}

/// Mouse interaction code below.

void View::mousePressEvent(QMouseEvent *event) {
    m_prevMousePos = event->pos();
}

void View::mouseMoveEvent(QMouseEvent *event) {
    m_angleX += 3 * (event->x() - m_prevMousePos.x()) / (float) width();
    m_angleY += 3 * (event->y() - m_prevMousePos.y()) / (float) height();
    m_prevMousePos = event->pos();
    rebuildMatrices();
}

void View::wheelEvent(QWheelEvent *event) {
    m_zoom -= event->delta() / 100.f;
    rebuildMatrices();
}

void View::rebuildMatrices() {
    m_view = glm::translate(glm::vec3(0, 0, -m_zoom)) *
             glm::rotate(m_angleY, glm::vec3(1,0,0)) *
             glm::rotate(m_angleX, glm::vec3(0,1,0));

    m_projection = glm::perspective(0.8f, (float)width()/height(), 0.1f, 100.f);
    update();
}

void View::mouseReleaseEvent(QMouseEvent *event) {

}

void View::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) QApplication::quit();

    // TODO: Handle keyboard presses here
}

void View::keyReleaseEvent(QKeyEvent *event) {

}

void View::tick() {
    // Get the number of seconds since the last tick (variable update rate)
    float seconds = m_time.restart() * 0.001f;

    // TODO: Implement the demo update here

    // Flag this view for repainting (Qt will call paintGL() soon after)
    update();
}

