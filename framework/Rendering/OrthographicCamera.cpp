#include "OrthographicCamera.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

OrthographicCamera::OrthographicCamera(
    const Frustrum& frustrum,
    const glm::vec3& position,
    float rotation)
{
    this->CameraFrustrum = frustrum;
    this->Position = position;
    this->Rotation = rotation;
    this->RecalculateMatrix();
}

void OrthographicCamera::RecalculateMatrix()
{

    ProjectionMatrix = glm::ortho(
        CameraFrustrum.left,
        CameraFrustrum.right,
        CameraFrustrum.bottom,
        CameraFrustrum.top,
        CameraFrustrum.near,
        CameraFrustrum.far
    );

    // Start with identity, then apply rotation
    glm::mat4 transform = glm::mat4(1.0f);

    // Translate to camera position
    transform = glm::translate(transform, -Position);

    // Apply rotation around Z axis (common for 2D orthographic camera)
    transform = glm::rotate(transform, glm::radians(Rotation), glm::vec3(0.0f, 0.0f, 1.0f));

    ViewMatrix = transform;
    ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
}
