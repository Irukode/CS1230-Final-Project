#ifndef VIEW_H
#define VIEW_H

#include "GL/glew.h"
#include <qgl.h>
#include <QTime>
#include <QTimer>
#include "shapes/openglshape.h"
#include "camera/Camera.h"
#include "uniforms/uniformvariable.h"

class QGLShaderProgram;


#define colorToArgs(floatArray) floatArray[0], floatArray[1], floatArray[2], floatArray[3]

class View : public QGLWidget {
    Q_OBJECT

public:
    View(QWidget *parent);
    ~View();

    bool saveUniforms(QString path);
    bool loadUniforms(QString path);

    static UniformVariable* s_skybox;
    static UniformVariable* s_model;
    static UniformVariable* s_view;
    static UniformVariable* s_projection;
    static UniformVariable* s_mvp;
    static UniformVariable* s_time;
    static UniformVariable* s_size;
    static UniformVariable* s_mouse;

    static std::vector<UniformVariable*> *s_staticVars;

signals:
    void addUniform(UniformVariable::Type type, const QString &name, bool editable = true, int size = 1);
    void addUniform(UniformVariable *uniform, bool editable = true);
    void changeUniform(const UniformVariable *uniform, const QString &newVal);
    void changeUniform(const QString &name, const QString &newVal);

private:
    QTime m_time;
    QTimer m_timer;
    bool m_captureMouse;

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    //std::unique_ptr<OpenGLShape> m_sphere;
    //std::unique_ptr<OpenGLShape> m_cube;
    OpenGLShape *m_shape;
    Camera *camera;
    std::unique_ptr<OpenGLShape> m_cube;
    std::unique_ptr<OpenGLShape> skybox_cube;
    QGLShaderProgram *skybox_shader;
    QGLShaderProgram *wireframe_shader;
    QGLShaderProgram *wireframe_shader2;
    QGLShaderProgram *current_shader;

    QList<const UniformVariable*> *activeUniforms;
    QList<const UniformVariable*> permUniforms;

    QOpenGLFunctions gl;

    QTimer *timer;

    glm::mat4 model;

    void handleAnimation();

    //RenderType m_renderMode;

    bool mouseDown;

private slots:

    void tick();
    bool loadShader(QString vert, QString frag, QString *errors = 0);
    void uniformDeleted(const UniformVariable *uniform);
    void uniformAdded(const UniformVariable *uniform);
    void viewChanged(const glm::mat4 &modelview);
    void projectionChanged(const glm::mat4 &projection);
    void modelviewProjectionChanged(const glm::mat4 &modelviewProjection);
    void modelChanged(const glm::mat4 &modelview);
    //void setPaused(bool paused);
};

#endif // VIEW_H
