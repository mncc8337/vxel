#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
private:
    glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 right;
    glm::vec3 up;

public:
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    float fovy = 45.0f;
    float aspect = 1.0f;
    float near = 0.1f;
    float far = 100.0f;

    float yaw = -90.0f;
    float pitch = 0.0f;
    // float roll = 0.0f;
    // there is no roll because im lazy

    Camera() {
        lookAt(glm::vec3(0.0f, 0.0f, -1.0f));
    }

    glm::vec3 getLookingDirection() {
        return -direction;
    }
    glm::vec3 getUpDirection() {
        return up;
    }
    glm::vec3 getRightDirection() {
        return right;
    }
    glm::mat4 getProjection() {
        return glm::perspective(glm::radians(fovy), aspect, near, far);
    }
    glm::mat4 getView() {
        return glm::lookAt(position, position - direction, up);
    }

    void applyNewRotation() {
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction *= -1;
        lookAt(position - direction);
    }
    void lookAt(glm::vec3 target) {
        direction = glm::normalize(position - target);
        right = glm::normalize(glm::cross(world_up, direction));
        up = glm::cross(direction, right);
    }
    void translate(glm::vec3 d) {
        position += d;
    }
    void moveForward(float ammount) {
        translate(-direction * ammount);
    }
    void moveForwardXZ(float ammount) {
        glm::vec3 d = -direction * ammount;
        d.y = 0;
        translate(d);
    }
    void moveRight(float ammount) {
        translate(right * ammount);
    }
    void moveRightXZ(float ammount) {
        glm::vec3 d = right * ammount;
        d.y = 0;
        translate(d);
    }
};