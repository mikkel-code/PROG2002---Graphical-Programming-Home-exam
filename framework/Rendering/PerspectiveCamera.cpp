#include "PerspectiveCamera.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

PerspectiveCamera::PerspectiveCamera(
    const Frustrum& frustrum,
    const glm::vec3& position,
    const glm::vec3& lookAt,
    const glm::vec3& upVector)
{
    this->CameraFrustrum = frustrum;
    this->Position = position;
    this->LookAt = lookAt;
    this->UpVector = upVector;
    this->RecalculateMatrix();
}

void PerspectiveCamera::RecalculateMatrix()
{
    float aspect = (CameraFrustrum.width <= 0.0f || CameraFrustrum.height <= 0.0f)
        ? 1.0f
        : CameraFrustrum.width / CameraFrustrum.height;

    ProjectionMatrix = glm::perspective(
        glm::radians(CameraFrustrum.angle),
        aspect,
        CameraFrustrum.near,
        CameraFrustrum.far
    );

    ViewMatrix = glm::lookAt(
        Position,
        LookAt,
        UpVector
    );

    ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
}