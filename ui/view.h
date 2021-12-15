#ifndef VIEW_H
#define VIEW_H

#include <memory>

#include "GL/glew.h"
#include <qgl.h>
#include <QTime>
#include <QTimer>

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "glm/gtc/type_ptr.hpp" // glm::value_ptr
#include "cs123_lib/sphere.h"
#include "shapes/openglshape.h"

class View : public QGLWidget {
    Q_OBJECT

public:
    View(QWidget *parent);
    ~View();

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

    std::vector<std::shared_ptr<OpenGLShape>> m_spheres;
    /** ID for the shader program. */
    GLuint m_program;
    GLuint m_phongprogram;

    void rebuildMatrices();
    glm::mat4 m_view, m_projection;

    /** For mouse interaction. */
    float m_angleX, m_angleY, m_zoom;
    QPoint m_prevMousePos;

    /** Timer calls tick() 60 times per second. */
    float m_fps;

    float m_tick;
    float m_angle;

    /** Incremented on every call to paintGL. */
    int m_increment;

private slots:
    void tick();
};

#endif // VIEW_H
