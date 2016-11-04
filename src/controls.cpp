#include <detail/type_mat4x4.hpp>
#include <gtc/matrix_transform.hpp>
#include <stdio.h>
#include "controls.hpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix() {
    return ViewMatrix;
}

glm::mat4 getProjectionMatrix() {
    return ProjectionMatrix;
}

// Initilaize position : on +Z
glm::vec3 position = glm::vec3(0, 0, 5);
// Initial horizontal angle : toward -z
float horizontalAngle = 3.14F;
// initial vertical angle : none
float verticalAngle = 0.0F;
// Initial FoV
float initialFoV = 45.0F;

float speed = 3.0F; // 3 units / second
float mouseSpeed = 0.005F;

void computeMatricesFromInputs(GLFWwindow* window, int width, int height) {
    // Called only on first run
    static double lastTime = glfwGetTime();

    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    // Get mouse position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    // Reset mouse positioon for next frame
    glfwSetCursorPos(window, width / 2, height / 2);

    // Compute new orientation
    horizontalAngle += mouseSpeed * float(width / 2 - xpos);
    verticalAngle   += mouseSpeed * float(height / 2 - ypos);

    // Direction : Spherical coordinates to Cartesian coordinates conversion
    glm::vec3 direction(
            cos(verticalAngle) * sin(horizontalAngle),
            sin(verticalAngle),
            cos(verticalAngle) * cos(horizontalAngle)
    );

    // Right vector
    glm::vec3 right = glm::vec3(
            sin(horizontalAngle - 3.14F / 2.0F),
            0,
            cos(horizontalAngle - 3.14F / 2.0F)
    );

    // Up vector
    glm::vec3 up = glm::cross(right, direction);

    // Move forward
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        position += direction * deltaTime * speed;
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        position -= direction * deltaTime * speed;
    }
    // Strafe right
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        position += right * deltaTime * speed;
    }
    // Strafe left
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        position -= right * deltaTime * speed;
    }

    float FoV = initialFoV;

    // Projection matrix : 45 FoV, custom calculated ratio, display range : 0.1 unit <-> 100 units
    ProjectionMatrix = glm::perspective(glm::radians(FoV), float(width / height), 0.1F, 100.0F);
    // Camera matrix
    ViewMatrix = glm::lookAt(
            position,
            position + direction,
            up
    );

    lastTime = currentTime;
}

void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods) {
    const char* keyName = glfwGetKeyName(key, scanCode);
    printf("%s \n", keyName);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    printf("%i %i \n", button, action);
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    printf("%f %f \n", xoffset, yoffset);
}










