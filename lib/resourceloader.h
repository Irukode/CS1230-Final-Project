#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H

#include "GL/glew.h"

#include <QFile>
#include <QGLShaderProgram>

/**
   A resource loader with code to handle loading models, skyboxes, and shader programs.

  @author: Justin Ardini (jardini)
   **/
class ResourceLoader {
public:
    ResourceLoader();
    static GLuint createShaderProgram(const char * vertex_file_path,const char * fragment_file_path);
    static void initializeGlew();
    // Returns the skybox ID
    GLuint loadSkybox();

    // These return a new QGLShaderProgram.  THIS MUST BE DELETED BY THE CALLER.
    QGLShaderProgram * newVertShaderProgram(const QGLContext *context, QString vertShader);
    QGLShaderProgram * newFragShaderProgram(const QGLContext *context, QString fragShader);
    QGLShaderProgram * newShaderProgram(const QGLContext *context, QString vertShader, QString fragShader, QString *errors = 0);
    // Returns the cubeMap ID
    GLuint loadCubeMap(QList<QFile *> files);


private:
    static GLuint createShader(GLenum shaderType, const char *filepath);
};

#endif // RESOURCELOADER_H
