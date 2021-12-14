#include "view.h"

#include "viewformat.h"
#include <QApplication>
#include <QKeyEvent>
#include <iostream>

#include "view.h"
#include <QMouseEvent>
#include <sstream>

#include "shapes/cube.h"
#include "camera/OrbitingCamera.h"
#include "lib/resourceloader.h"
#include "uniforms/varsfile.h"
#include "gl/shaders/ShaderAttribLocations.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

UniformVariable *View::s_skybox = NULL;
UniformVariable *View::s_projection = NULL;
UniformVariable *View::s_model = NULL;
UniformVariable *View::s_view = NULL;
UniformVariable *View::s_mvp = NULL;
UniformVariable *View::s_time = NULL;
UniformVariable *View::s_size = NULL;
UniformVariable *View::s_mouse = NULL;
std::vector<UniformVariable*> *View::s_staticVars = NULL;

View::View(QWidget *parent) : QGLWidget(ViewFormat(), parent),
    m_time(), m_timer(), m_captureMouse(false)
{
    // View needs all mouse move events, not just mouse drag events
    setMouseTracking(true);

    // Hide the cursor
    if (m_captureMouse) {
        QApplication::setOverrideCursor(Qt::BlankCursor);
    }

    // View needs keyboard focus
    setFocusPolicy(Qt::StrongFocus);

    // The update loop is implemented using a timer
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));

    camera = new OrbitingCamera();
//    QObject::connect(camera, SIGNAL(viewChanged(glm::mat4)), this, SLOT(viewChanged(glm::mat4)));
//    QObject::connect(camera, SIGNAL(projectionChanged(glm::mat4)), this, SLOT(projectionChanged(glm::mat4)));
//    QObject::connect(camera, SIGNAL(modelviewProjectionChanged(glm::mat4)), this, SLOT(modelviewProjectionChanged(glm::mat4)));

    activeUniforms = new QList<const UniformVariable *>();
    current_shader = NULL;
    wireframe_shader2 = NULL;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000.0f/60.0f);

    s_staticVars = new std::vector<UniformVariable*>();
}

View::~View()
{
    delete camera;

    delete activeUniforms;
    delete timer;

    delete skybox_shader;
    foreach (const UniformVariable *v, permUniforms) {
        delete v;
    }
}

void View::initializeGL() {
    // All OpenGL initialization *MUST* be done during or after this
    // method. Before this method is called, there is no active OpenGL
    // context and all OpenGL calls have no effect.

    //initialize glew
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        std::cerr << "Something is very wrong, glew initialization failed." << std::endl;
    }
    std::cout << "Using GLEW " <<  glewGetString( GLEW_VERSION ) << std::endl;

    // Start a timer that will try to get 60 frames per second (the actual
    // frame rate depends on the operating system and other running programs)
    m_time.start();
    m_timer.start(1000 / 60);

//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
//    glCullFace(GL_BACK);
//    glFrontFace(GL_CCW);
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_CUBE_MAP);

    glDisable(GL_COLOR_MATERIAL);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

//    skybox_shader = ResourceLoader::newShaderProgram(context(), ":/shaders/skybox.vert", ":/shaders/skybox.frag");
//    //wireframe_shader = ResourceLoader::newShaderProgram(context(), ":/shaders/standard.vert", ":/shaders/color.frag");

//    s_skybox = new UniformVariable(this->context()->contextHandle());
//    s_skybox->setName("skybox");
//    s_skybox->setType(UniformVariable::TYPE_TEXCUBE);
//    //top, bottom, left, right, front, back
//    s_skybox->parse(":/skybox/posy.jpg,:/skybox/negy.jpg,:/skybox/negx.jpg,:/skybox/posx.jpg,:/skybox/posz.jpg,:/skybox/negz.jpg");

//    s_model = new UniformVariable(this->context()->contextHandle());
//    s_model->setName("model");
//    s_model->setType(UniformVariable::TYPE_MAT4);
//    s_model->parse("1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1");

//    s_projection = new UniformVariable(this->context()->contextHandle());
//    s_projection->setName("projection");
//    s_projection->setType(UniformVariable::TYPE_MAT4);

//    s_view = new UniformVariable(this->context()->contextHandle());
//    s_view->setName("view");
//    s_view->setType(UniformVariable::TYPE_MAT4);

//    s_mvp = new UniformVariable(this->context()->contextHandle());
//    s_mvp->setName("mvp");
//    s_mvp->setType(UniformVariable::TYPE_MAT4);

//    s_time = new UniformVariable(this->context()->contextHandle());
//    s_time->setName("time");
//    s_time->setType(UniformVariable::TYPE_TIME);

//    s_size = new UniformVariable(this->context()->contextHandle());
//    s_size->setName("size");
//    s_size->setType(UniformVariable::TYPE_FLOAT2);

//    s_mouse = new UniformVariable(this->context()->contextHandle());
//    s_mouse->setName("mouse");
//    s_mouse->setType(UniformVariable::TYPE_FLOAT3);

//    s_staticVars->push_back(s_skybox);
//    s_staticVars->push_back(s_model);
//    s_staticVars->push_back(s_projection);
//    s_staticVars->push_back(s_view);
//    s_staticVars->push_back(s_mvp);
//    s_staticVars->push_back(s_time);
//    s_staticVars->push_back(s_size);
//    s_staticVars->push_back(s_mouse);

    std::vector<GLfloat> cubeData = CUBE_DATA_POSITIONS;
    m_cube = std::make_unique<OpenGLShape>();
    m_cube->setVertexData(&cubeData[0], cubeData.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES, NUM_CUBE_VERTICES);
    m_cube->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_cube->setAttribute(ShaderAttrib::NORMAL, 3, 3*sizeof(GLfloat), VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_cube->buildVAO();

    gl = QOpenGLFunctions(context()->contextHandle());
    skybox_cube = std::make_unique<OpenGLShape>();
    skybox_cube->setVertexData(&cubeData[0], cubeData.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES, NUM_CUBE_VERTICES);
    skybox_cube->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    skybox_cube->setAttribute(ShaderAttrib::NORMAL, 3, 3*sizeof(GLfloat), VBOAttribMarker::DATA_TYPE::FLOAT, false);
    skybox_cube->buildVAO();

    //m_shape = m_sphere.get();
}

void View::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // TODO: Implement the demo rendering here
    skybox_shader->bind();
    s_skybox->setValue(skybox_shader);
    s_projection->setValue(skybox_shader);
    s_view->setValue(skybox_shader);
    glCullFace(GL_FRONT);
    skybox_cube->draw();
    glCullFace(GL_BACK);
    skybox_shader->release();

}

void View::resizeGL(int w, int h) {
//    float ratio = static_cast<QGuiApplication *>(QCoreApplication::instance())->devicePixelRatio();
//    w = static_cast<int>(w / ratio);
//    h = static_cast<int>(h / ratio);
//    glViewport(0, 0, w, h);

    glViewport(0, 0, w, h);
    s_size->parse(QString("%1,%2").arg(QString::number(w), QString::number(h)));
    camera->setAspectRatio(((float) w) / ((float) h));
    update();
}

void View::wheelEvent(QWheelEvent *event)
{
    camera->mouseScrolled(event->delta());
}

void View::mousePressEvent(QMouseEvent *event) {
    camera->mouseDown(event->x(), event->y());
    mouseDown = true;
    s_mouse->parse(QString("%1,%2,%3").arg(
                       QString::number(event->x()),
                       QString::number(event->y()),
                       QString::number(mouseDown)));
}

void View::mouseMoveEvent(QMouseEvent *event) {
    // This starter code implements mouse capture, which gives the change in
    // mouse position since the last mouse movement. The mouse needs to be
    // recentered after every movement because it might otherwise run into
    // the edge of the screen, which would stop the user from moving further
    // in that direction. Note that it is important to check that deltaX and
    // deltaY are not zero before recentering the mouse, otherwise there will
    // be an infinite loop of mouse move events.
    if(m_captureMouse) {
        int deltaX = event->x() - width() / 2;
        int deltaY = event->y() - height() / 2;
        if (!deltaX && !deltaY) return;
        QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));

        // TODO: Handle mouse movements here
        if (event->buttons() & Qt::LeftButton) {
            camera->mouseDragged(event->x(), event->y());
        }
        s_mouse->parse(QString("%1,%2,%3").arg(
                           QString::number(event->x()),
                           QString::number(event->y()),
                           QString::number(mouseDown)));
    }
}

void View::mouseReleaseEvent(QMouseEvent *event) {
    mouseDown = false;
    s_mouse->parse(QString("%1,%2,%3").arg(
                       QString::number(event->x()),
                       QString::number(event->y()),
                       QString::number(mouseDown)));
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

bool View::saveUniforms(QString path)
{
    QList<const UniformVariable *> toSave;
    foreach (const UniformVariable *v, *activeUniforms) {
        toSave += v;
    }
    foreach (const UniformVariable *v, permUniforms) {
        toSave += v;
    }

    return VarsFile::save(path, &toSave);
}

bool View::loadUniforms(QString path)
{
    QList<const UniformVariable*> fromFile;

    foreach (const UniformVariable *v, permUniforms) {
        delete v;
    }
    permUniforms.clear();

    if (!VarsFile::load(path, &fromFile, context()->contextHandle())) return false;

    bool match;
    foreach (const UniformVariable *v, fromFile) {
        match = false;
        foreach (const UniformVariable *u, *activeUniforms) {
            if (!v->getName().compare(u->getName()) && v->getType() == u->getType()) {
                // Really really really bad, but the best option I can think of for now
                // sskybox_shaderetPermanent doesn't really modify the object much, sort of a small
                // flag for how to save it
                UniformVariable *utemp = const_cast<UniformVariable *>(u);
                utemp->setPermanent(v->getPermanent());
                emit(changeUniform(u, v->toString()));
                match = true;
            }
        }

        if (!match && v->getPermanent()) {
            permUniforms += v;
        } else {
            delete v;
        }
    }
    return true;
}

void View::uniformDeleted(const UniformVariable *uniform)
{
    foreach (UniformVariable *sv, *s_staticVars) {
        if (uniform == sv) return;
    }

    foreach (const UniformVariable *v, *activeUniforms) {
        if (uniform == v)
            delete v;
    }
    activeUniforms->removeAll(uniform);
}

void View::uniformAdded(const UniformVariable *uniform)
{
    activeUniforms->append(uniform);
}

void View::viewChanged(const glm::mat4 &modelview)
{
    std::stringstream s;
    const float *data = glm::value_ptr(glm::transpose(modelview));
    for (int i = 0; i < 16; i++) {
        s << data[i];
        if (i < 15)
            s << ",";
    }
    s_view->parse(QString::fromStdString(s.str()));
}

void View::projectionChanged(const glm::mat4 &projection)
{
    std::stringstream s;
    const float *data = glm::value_ptr(glm::transpose(projection));
    for (int i = 0; i < 16; i++) {
        s << data[i];
        if (i < 15)
            s << ",";
    }
    s_projection->parse(QString::fromStdString(s.str()));
}

void View::modelviewProjectionChanged(const glm::mat4 &modelviewProjection)
{
    std::stringstream s;
    const float *data = glm::value_ptr(glm::transpose(modelviewProjection * model));
    for (int i = 0; i < 16; i++) {
        s << data[i];
        if (i < 15)
            s << ",";
    }
    s_mvp->parse(QString::fromStdString(s.str()));
}

void View::modelChanged(const glm::mat4 &modelview)
{
    std::stringstream s;
    const float *data = glm::value_ptr(glm::transpose(modelview));
    for (int i = 0; i < 16; i++) {
        s << data[i];
        if (i < 15)
            s << ",";
    }
    s_model->parse(QString::fromStdString(s.str()));
}

bool View::loadShader(QString vert, QString frag, QString *errors)
{
    QGLShaderProgram *new_shader = ResourceLoader::newShaderProgram(context(), vert, frag, errors);
    if (new_shader == NULL) {
        return false;
    }

//    delete wireframe_shader2;
//    wireframe_shader2 = ResourceLoader::newShaderProgram(context(), vert, ":/shaders/color.frag", errors);

    UniformVariable::s_numTextures = 2;

    UniformVariable::resetTimer();

    // http://stackoverflow.com/questions/440144/in-opengl-is-there-a-way-to-get-a-list-of-all-uniforms-attribs-used-by-a-shade

    std::vector<GLchar> nameData(256);
    GLint numActiveUniforms = 0;
    gl.glGetProgramiv(new_shader->programId(), GL_ACTIVE_UNIFORMS, &numActiveUniforms);

    for (int unif = 0; unif < numActiveUniforms; unif++) {
        GLint arraySize = 0;
        GLenum type = 0;
        GLsizei actualLength = 0;
        gl.glGetActiveUniform(new_shader->programId(), unif, nameData.size(), &actualLength, &arraySize, &type, &nameData[0]);
        std::string name((char*)&nameData[0], actualLength);

        UniformVariable::Type uniformType = UniformVariable::typeFromGLEnum(type);

        QString qname = QString::fromStdString(name);
        if (qname.startsWith("gl_")) continue;
        emit(addUniform(uniformType, qname, true, arraySize));
    }

    delete current_shader;
    current_shader = new_shader;
    camera->mouseScrolled(0);
    camera->updateMats();
    update();
    return true;
}
