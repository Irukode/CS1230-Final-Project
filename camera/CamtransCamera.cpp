/**
 * @file   CamtransCamera.cpp
 *
 * This is the perspective camera class you will need to fill in for the Camtrans lab.  See the
 * lab handout for more details.
 */

#include "CamtransCamera.h"
#include <Settings.h>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

CamtransCamera::CamtransCamera()
{
    // @TODO: [CAMTRANS] Fill this in...
    m_near = 1.0f;
    m_far = 30.0f;
    m_thetaH = 60.0f;
    m_aspectRatio = 1.0f;
    m_eye = glm::vec4(2.0f,2.0f,2.0f,1.0f);
    m_look = glm::vec4(-2.0f,-2.0f,-2.0f,0.0f);
    m_up = glm::vec4(0.0f,1.0f,0.0f,0.0f);
    orientLook(glm::vec4(2.0f,2.0f,2.0f,1.0f), glm::normalize(glm::vec4(-2.0f,-2.0f,-2.0f,0.0f)), glm::vec4(0.0f,1.0f,0.0f,0.0f));
}

void CamtransCamera::setAspectRatio(float a)
{
    // @TODO: [CAMTRANS] Fill this in...
    m_aspectRatio = a;
    m_thetaW = 2.0f * glm::atan(glm::tan(glm::radians(m_thetaH/2.0f))*m_aspectRatio);
    updateProjectionMatrix();
}

glm::mat4x4 CamtransCamera::getProjectionMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
    glm::mat4x4 projection = m_perspectiveTransformation*m_scaleMatrix;
    return projection;
}

glm::mat4x4 CamtransCamera::getViewMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_rotationMatrix*m_translationMatrix;
}

glm::mat4x4 CamtransCamera::getScaleMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_scaleMatrix;
}

glm::mat4x4 CamtransCamera::getPerspectiveMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_perspectiveTransformation;
}

glm::vec4 CamtransCamera::getPosition() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_eye;
}

glm::vec4 CamtransCamera::getLook() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_look;
}

glm::vec4 CamtransCamera::getUp() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_up;
}

float CamtransCamera::getAspectRatio() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_aspectRatio;
}

float CamtransCamera::getHeightAngle() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_thetaH;
}

void CamtransCamera::orientLook(const glm::vec4 &eye, const glm::vec4 &look, const glm::vec4 &up) {
    // @TODO: [CAMTRANS] Fill this in...
    m_eye = eye;
    m_up = up;
    m_look = look;
    m_w = -glm::normalize(m_look);
    m_v = glm::normalize(m_up-glm::dot(m_up, m_w)*m_w);
    glm::vec3 w = m_w.xyz();
    glm::vec3 v = m_v.xyz();
    glm::vec3 u = glm::cross(v, w);
    m_u = glm::vec4(u,0);
    updateViewMatrix();
    updateProjectionMatrix();

}

void CamtransCamera::setHeightAngle(float h) {
    // @TODO: [CAMTRANS] Fill this in...
    m_thetaH = h;
    m_thetaW = 2.0f * glm::atan(glm::tan(glm::radians(m_thetaH/2.0f))*m_aspectRatio);
    updateProjectionMatrix();

}

void CamtransCamera::translate(const glm::vec4 &v) {
    // @TODO: [CAMTRANS] Fill this in...
    m_eye+=v;
    updateViewMatrix();

}

void CamtransCamera::rotateU(float degrees) {
    // @TODO: [CAMTRANS] Fill this in...
    glm::vec4 v = glm::vec4(0.0f, glm::cos(glm::radians(-degrees)), -glm::sin(glm::radians(-degrees)), 0.0f);
    glm::vec4 w = glm::vec4(0.0f, glm::sin(glm::radians(-degrees)), glm::cos(glm::radians(-degrees)), 0.0f);

    glm::mat4x4 inverse = glm::inverse(getViewMatrix());
    m_v = inverse*v;
    m_w = inverse*w;

    m_look = -m_w;
    m_up = (glm::dot(m_up, m_w)*m_w)+(glm::dot(m_up, m_v)*m_v);
    updateViewMatrix();
}

void CamtransCamera::rotateV(float degrees) {
    // @TODO: [CAMTRANS] Fill this in...
    glm::vec4 u = glm::vec4(glm::cos(glm::radians(-degrees)),0.0f, glm::sin(glm::radians(-degrees)), 0.0f);
    glm::vec4 w = glm::vec4(-glm::sin(glm::radians(-degrees)), 0.0f, glm::cos(glm::radians(-degrees)), 0.0f);

    glm::mat4x4 inverse = glm::inverse(getViewMatrix());
    m_u = inverse*u;
    m_w = inverse*w;

    m_look = -m_w;
    m_up = (glm::dot(m_up, m_w)*m_w)+(glm::dot(m_up, m_v)*m_v);
    updateViewMatrix();

}

void CamtransCamera::rotateW(float degrees) {
    // @TODO: [CAMTRANS] Fill this in...
    glm::vec4 u = glm::vec4(glm::cos(glm::radians(-degrees)), -glm::sin(glm::radians(-degrees)),0.0f, 0.0f);
    glm::vec4 v = glm::vec4(glm::sin(glm::radians(-degrees)), glm::cos(glm::radians(-degrees)),0.0f, 0.0f);

    glm::mat4x4 inverse = glm::inverse(getViewMatrix());
    m_u = inverse*u;
    m_v = inverse*v;

    m_up = (glm::dot(m_up, m_w)*m_w)+(glm::dot(m_up, m_v)*m_v);
    updateViewMatrix();
}

void CamtransCamera::setClip(float nearPlane, float farPlane) {
    // @TODO: [CAMTRANS] Fill this in...
    m_near = nearPlane;
    m_far = farPlane;
    updateProjectionMatrix();

}

void CamtransCamera::updateProjectionMatrix(){
    updateScaleMatrix();
    updatePerspectiveMatrix();
}

void CamtransCamera::updatePerspectiveMatrix(){
    float c = -m_near/m_far;
    m_perspectiveTransformation = glm::transpose(glm::mat4x4(1,0,0,0,
                                                             0,1,0,0,
                                                             0,0,-1.0f/(1.0f+c), c/(1.0f+c),
                                                             0,0,-1,0));
}

void CamtransCamera::updateScaleMatrix(){
    m_scaleMatrix = glm::transpose(glm::mat4x4(1.0f/(m_far*glm::tan(m_thetaW/2.0f)),0,0,0,
                                               0,1.0f/(m_far*glm::tan(glm::radians(m_thetaH/2.0f))),0,0,
                                               0,0,1.0f/m_far, 0,
                                               0,0,0,1));
}

void CamtransCamera::updateViewMatrix(){
    updateRotationMatrix();
    updateTranslationMatrix();
}

void CamtransCamera::updateRotationMatrix(){
    m_rotationMatrix = glm::transpose(glm::mat4x4(m_u.x,m_u.y,m_u.z,0,
                                                  m_v.x,m_v.y,m_v.z,0,
                                                  m_w.x,m_w.y,m_w.z,0,
                                                  0,0,0,1));
}

void CamtransCamera::updateTranslationMatrix(){
    m_translationMatrix = glm::transpose(glm::mat4x4(1,0,0,-m_eye.x,
                                                     0,1,0,-m_eye.y,
                                                     0,0,1,-m_eye.z,
                                                     0,0,0,1));
}

glm::vec4 CamtransCamera::getU() const{
    return m_u;
}

glm::vec4 CamtransCamera::getV() const{
    return m_v;
}

glm::vec4 CamtransCamera::getW() const{
    return m_w;
}
