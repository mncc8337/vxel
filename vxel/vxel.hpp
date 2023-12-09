#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.hpp"
#include "camera.hpp"
#include "voxel.hpp"
#include "image_utils.hpp"

#include <map>
#include <vector>

const uint MAX_TEXTURE_NUMBER = 32;

class vxel {
private:
    std::vector<const char*> texture_path;

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if(action == GLFW_PRESS)
            keyhold[key] = true;
        else if(action == GLFW_RELEASE)
            keyhold[key] = false;
    }

public:
    static std::map<int, bool> keyhold;

    GLFWwindow* window;
    uint VBO, VAO;
    uint textures[MAX_TEXTURE_NUMBER];
    Shader shader;
    Camera camera;
    float deltaTime = 0;

    std::vector<Voxel> voxels;

    vxel(int WIDTH, int HEIGHT, const char* TITLE) {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glewInit();

        window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
        glfwMakeContextCurrent(window);

        glfwSetKeyCallback(window, keyCallback);

        glEnable(GL_DEPTH_TEST);
    }
    uint addTexture(const char* path) {
        texture_path.push_back(path);
        return texture_path.size() - 1;
    }
    void loadShader() {
        shader.loadFromFile("./shaders/vertex.glsl",
                            "./shaders/fragment.glsl");

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(voxel_vertices), voxel_vertices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        uint texture_num = texture_path.size();
        if(texture_num > 0) {
            glGenTextures(texture_num - 1, textures);

            for(int i = 0; i < texture_num; i++)
                loadImageTexture(*(textures + i), texture_path[i]);
                texture_path.clear();
        }

        shader.use();
        shader.setInt("tex", 0);
    }
    void start(void (*process)(void)) {
        float lastFrame = glfwGetTime();
        while (!glfwWindowShouldClose(window)) {
            process();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            shader.use();

            shader.setMat4("projection", camera.getProjection());
            shader.setMat4("view", camera.getView());

            glBindVertexArray(VAO);
            for(Voxel vox: voxels) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, *(textures + vox.getTexture()));

                shader.setMat4("model", vox.getModel());
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            glfwSwapBuffers(window);
            glfwPollEvents();

            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
        }

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glfwTerminate();
    }
    void stop() {
        glfwSetWindowShouldClose(window, true);
    }
};

std::map<int, bool> vxel::keyhold;