#include <stdio.h>

#include <glbinding/Binding.h>
#include <glbinding/gl33/gl.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>

#include "shader.hpp"
#include "image.hpp"
#include "controls.hpp"

static int width = 1024, height = 768;

int main(int argc, char **argv)
{
    if(!glfwInit()) {
        fprintf(stderr, "Failed to initiate GLFW.\n");
        return 1;
    }


    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *mainWindow = glfwCreateWindow(width, height, "Title", NULL, NULL);

    if(mainWindow == nullptr) {
        fprintf( stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return 2;
    }

    glfwMakeContextCurrent(mainWindow);


    glbinding::Binding::initialize();

    // Ensure we can capture the escape y being pressed below
    glfwSetInputMode(mainWindow, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited movement
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(mainWindow, width/2, height/2);

    glfwSetKeyCallback(mainWindow, keyCallback);
    glfwSetMouseButtonCallback(mainWindow, mouseButtonCallback);
    glfwSetScrollCallback(mainWindow, scrollCallback);

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it is closer to the camera then the former one
    glDepthFunc(GL_LESS);
    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

    GLuint VertexArrayID;
    gl::glGenVertexArrays(1, &VertexArrayID);
    gl::glBindVertexArray(VertexArrayID);

    GLuint programID = LoadShaders( "shaders/simplevertex.glsl", "shaders/simplefragment.glsl" );

    // Get a handle for our "MVP" uniform
    GLint MatrixID = glGetUniformLocation(programID, "MVP");

    // texture stuff
    Image* image = new Image(std::string("textures/dirt2.png"), 0);
    if(image->get_image() == nullptr) {
        fprintf(stderr, "Texture dirt2 failed to load.\n");
        return 3;
    }
    // texture 2
    Image* image2 = new Image(std::string("textures/dirt.png"), 0);
    if(image2->get_image() == nullptr) {
        fprintf(stderr, "Texture dirt failed to load.\n");
        return 4;
    }

    GLuint textureDirt;
    glGenTextures(1, &textureDirt);

    GLint textureID = glGetUniformLocation(programID, "sampler");

    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertexBufferData[] {
            // Front side
            -1.0F,-1.0F, 1.0F, // T0:B
             1.0F,-1.0F, 1.0F,
            -1.0F, 1.0F, 1.0F, // T0:E
             1.0F,-1.0F, 1.0F, // T1:B
             1.0F, 1.0F, 1.0F,
            -1.0F, 1.0F, 1.0F, // T1:E
            // Right side
             1.0F,-1.0F, 1.0F, // T2:B
             1.0F,-1.0F,-1.0F,
             1.0F, 1.0F, 1.0F, // T2:E
             1.0F,-1.0F,-1.0F, // T3:B
             1.0F, 1.0F,-1.0F,
             1.0F, 1.0F, 1.0F, // T3:E
            // Back side
             1.0F,-1.0F,-1.0F, // T4:B
            -1.0F,-1.0F,-1.0F,
             1.0F, 1.0F,-1.0F, // T4:E
            -1.0F,-1.0F,-1.0F, // T5:B
            -1.0F, 1.0F,-1.0F,
             1.0F, 1.0F,-1.0F, // T5:E
            // Left side
            -1.0F,-1.0F,-1.0F, // T6:B
            -1.0F,-1.0F, 1.0F,
            -1.0F, 1.0F,-1.0F, // T6:E
            -1.0F,-1.0F, 1.0F, // T7:B
            -1.0F, 1.0F, 1.0F,
            -1.0F, 1.0F,-1.0F, // T7:E
            // Top Side
            -1.0F, 1.0F, 1.0F, // T8:B
             1.0F, 1.0F, 1.0F,
            -1.0F, 1.0F,-1.0F, // T8:E
             1.0F, 1.0F, 1.0F, // T9:B
             1.0F, 1.0F,-1.0F,
            -1.0F, 1.0F,-1.0F, // T9:E
            // Bottom side
            -1.0F,-1.0F,-1.0F, // T10:B
             1.0F,-1.0F,-1.0F,
            -1.0F,-1.0F, 1.0F, // T10:E
             1.0F,-1.0F,-1.0F, // T11:B
             1.0F,-1.0F, 1.0F,
            -1.0F,-1.0F, 1.0F, // T11:E
    };

    // This will identify our vertex buffer
    GLuint vertexBuffer;
    // Generate 1 buffer, put the resulting identifier in vertexBuffer
    glGenBuffers(1, &vertexBuffer);
    // The following commands will talk about our 'vertexBuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);

    // Two UV coordinates for each vertex
    static const GLfloat g_uv_buffer_data[] = {
            0.0F, 1.0F, // T0:B
            1.0F, 1.0F,
            0.0F, 0.0F, // T0:E
            1.0F, 1.0F, // T1:B
            1.0F, 0.0F,
            0.0F, 0.0F, // T1:E
            0.0F, 1.0F, // T2:B
            1.0F, 1.0F,
            0.0F, 0.0F, // T2:E
            1.0F, 1.0F, // T3:B
            1.0F, 0.0F,
            0.0F, 0.0F, // T3:E
            0.0F, 1.0F, // T4:B
            1.0F, 1.0F,
            0.0F, 0.0F, // T4:E
            1.0F, 1.0F, // T5:B
            1.0F, 0.0F,
            0.0F, 0.0F, // T5:E
            0.0F, 1.0F, // T6:B
            1.0F, 1.0F,
            0.0F, 0.0F, // T6:E
            1.0F, 1.0F, // T7:B
            1.0F, 0.0F,
            0.0F, 0.0F, // T7:E
            0.0F, 1.0F, // T8:B
            1.0F, 1.0F,
            0.0F, 0.0F, // T8:E
            1.0F, 1.0F, // T9:B
            1.0F, 0.0F,
            0.0F, 0.0F, // T9:E
            0.0F, 1.0F, // T10:B
            1.0F, 1.0F,
            0.0F, 0.0F, // T10:E
            1.0F, 1.0F, // T11:B
            1.0F, 0.0F,
            0.0F, 0.0F  // T11:E
    };

    GLuint uvBuffer;
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(programID);

        // Compute the mvp matrix from keyboard and mouse input
        computeMatricesFromInputs(mainWindow, width, height);
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();
        glm::mat4 ModelMatrix = glm::mat4(1.0);
        glm::mat4 mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

        // Bind the texture based on if it has an alpha chanel or not
        if (image->get_comp() == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->get_image_width(), image->get_image_height(), 0,
                         GL_RGB, GL_UNSIGNED_BYTE, image->get_image());
        else if (image->get_comp() == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->get_image_width(), image->get_image_height(), 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, image->get_image());
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glUniform1f(textureID, 0);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(
                0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
        );

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        glVertexAttribPointer(
                1,                    // attribute. No particular reason for 1, but must match the layout in the shader.
                2,                    // size
                GL_FLOAT,             // type
                GL_FALSE,             // normalized?
                0,                    // stride
                (void*)0              // array buffer offset
        );

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 12*3); // Starting from vertex 0; 3 vertices total -> 1 triangle

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        glfwSwapBuffers(mainWindow);
        glfwPollEvents();
    } while(glfwGetKey(mainWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(mainWindow));

    // glDeleteBuffers(1, &vertexBuffer);
    glDeleteProgram(programID);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &uvBuffer);
    gl::glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteTextures(1, &textureDirt);
    image->releaseImage();

    delete image;
    delete image2;

    glfwTerminate();
    return 0;
}
