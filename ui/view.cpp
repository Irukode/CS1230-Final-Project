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

    //initialize glew
//    glewExperimental = GL_TRUE;
//    GLenum err = glewInit();
//    if (GLEW_OK != err) {
//        /* Problem: glewInit failed, something is seriously wrong. */
//        std::cerr << "Something is very wrong, glew initialization failed." << std::endl;
//    }
//    std::cout << "Using GLEW " <<  glewGetString( GLEW_VERSION ) << std::endl;

    // Start a timer that will try to get 60 frames per second (the actual
    // frame rate depends on the operating system and other running programs)
    m_time.start();
    m_timer.start(1000 / 60);

    // Set the color to set the screen when the color buffer is cleared.scale
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Creates the shader program that will be used for drawing.
    m_program = ResourceLoader::createShaderProgram(":shaders/default.vert", ":shaders/default.frag");
    m_phongprogram = ResourceLoader::createShaderProgram(":shaders/phong.vert", ":/shaders/phong.frag");
    m_textureProgram = ResourceLoader::createShaderProgram(":shaders/quad.vert", ":/shaders/texture.frag");

    // Initialize sphere with radius 0.5 centered at origin.
    std::vector<GLfloat> sphereData = SPHERE_VERTEX_POSITIONS;
    m_sphere = std::make_unique<OpenGLShape>();
    m_sphere->setVertexData(&sphereData[0], sphereData.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES, NUM_SPHERE_VERTICES);
    m_sphere->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_sphere->buildVAO();
//    std::shared_ptr<OpenGLShape> sphere1;
//    m_spheres.push_back(sphere1);
//    m_spheres[0] = std::make_unique<OpenGLShape>();
//    m_spheres[0]->setVertexData(&sphereData[0], sphereData.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES, NUM_SPHERE_VERTICES);
//    m_spheres[0]->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
//    m_spheres[0]->buildVAO();

    std::vector<GLfloat> quadData = {-1.f, 1.f, 0.f, 0.f, 1.f,
                                    -1.f, -1.f, 0.f, 0.f, 0.f,
                                    1.f, 1.f, 0.f, 1.f, 1.f,
                                    1.f, -1.f, 0.f, 1.f, 0.f};
    m_quad = std::make_unique<OpenGLShape>();
    m_quad->setVertexData(&quadData[0], quadData.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, 4);
    m_quad->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_quad->setAttribute(ShaderAttrib::TEXCOORD0, 2, 3*sizeof(GLfloat), VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_quad->buildVAO();


//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
//    glCullFace(GL_BACK);
//    glFrontFace(GL_CCW);

}

void View::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // TODO: Implement the demo rendering here
//    glUseProgram(m_phongprogram);
//    glm::mat4 m = glm::mat4(1.f);
//    glUniformMatrix4fv(glGetUniformLocation(m_phongprogram, "model"), 1, GL_FALSE, glm::value_ptr(m));
//    glUniformMatrix4fv(glGetUniformLocation(m_phongprogram, "view"), 1, GL_FALSE, glm::value_ptr(m_view));
//    glUniformMatrix4fv(glGetUniformLocation(m_phongprogram, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
    glUseProgram(m_textureProgram);
    glm::mat4 m = glm::mat4(1.f);
    glm::vec4 camera = m_view * glm::vec4(0.f, 0.f, 0.f, 1.f);
    glm::vec2 uResolution(m_width, m_height);
    glUniformMatrix4fv(glGetUniformLocation(m_textureProgram, "uResolution"), 1, GL_FALSE, glm::value_ptr(uResolution));
    glUniformMatrix4fv(glGetUniformLocation(m_textureProgram, "camera"), 1, GL_FALSE, glm::value_ptr(camera));
    glUniformMatrix4fv(glGetUniformLocation(m_textureProgram, "model"), 1, GL_FALSE, glm::value_ptr(m));
    glUniformMatrix4fv(glGetUniformLocation(m_textureProgram, "view"), 1, GL_FALSE, glm::value_ptr(m_view));
    glUniformMatrix4fv(glGetUniformLocation(m_textureProgram, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));

//    glUniformMatrix4fv(glGetUniformLocation(m_phongprogram, "model"), 1, GL_FALSE, glm::value_ptr(m));
//    glUniformMatrix4fv(glGetUniformLocation(m_phongprogram, "view"), 1, GL_FALSE, glm::value_ptr(m_view));
//    glUniformMatrix4fv(glGetUniformLocation(m_phongprogram, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));

    glViewport(0,0,m_width, m_height);

    glUseProgram(m_textureProgram);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_quad->draw();
    //glUseProgram(0);
}

void View::resizeGL(int w, int h) {
//    float ratio = static_cast<QGuiApplication *>(QCoreApplication::instance())->devicePixelRatio();
//    w = static_cast<int>(w / ratio);
//    h = static_cast<int>(h / ratio);
    m_width = w;
    m_height = h;

    m_blurFBO1 = std::make_unique<FBO>(1, FBO::DEPTH_STENCIL_ATTACHMENT::DEPTH_ONLY, m_width, m_height, TextureParameters::WRAP_METHOD::CLAMP_TO_EDGE);
    rebuildMatrices();
    //glViewport(0, 0, w, h);
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

//void View::mouseMoveEvent(QMouseEvent *event) {
//    // This starter code implements mouse capture, which gives the change in
//    // mouse position since the last mouse movement. The mouse needs to be
//    // recentered after every movement because it might otherwise run into
//    // the edge of the screen, which would stop the user from moving further
//    // in that direction. Note that it is important to check that deltaX and
//    // deltaY are not zero before recentering the mouse, otherwise there will
//    // be an infinite loop of mouse move events.
//    if(m_captureMouse) {
//        int deltaX = event->x() - width() / 2;
//        int deltaY = event->y() - height() / 2;
//        if (!deltaX && !deltaY) return;
//        QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));

//        // TODO: Handle mouse movements here
//    }
//}

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

