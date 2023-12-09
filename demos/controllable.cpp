// rendering 1000000 voxels

#include <vxel/vxel.hpp>

vxel engine(800, 800, "test");

const float speed = 10.0f;

// function to run before rendering each frame
void func() {
    if(engine.keyhold[GLFW_KEY_ESCAPE])
        engine.stop();

    if(engine.keyhold[GLFW_KEY_W])
        engine.camera.moveForwardXZ(speed * engine.deltaTime);
    if(engine.keyhold[GLFW_KEY_S])
        engine.camera.moveForwardXZ(-speed * engine.deltaTime);
    if(engine.keyhold[GLFW_KEY_A])
        engine.camera.moveRightXZ(-speed * engine.deltaTime);
    if(engine.keyhold[GLFW_KEY_D])
        engine.camera.moveRightXZ(speed * engine.deltaTime);

    if(engine.keyhold[GLFW_KEY_E])
        engine.camera.position.y += speed * engine.deltaTime;
    if(engine.keyhold[GLFW_KEY_Q])
        engine.camera.position.y -= speed * engine.deltaTime;
    
    if(engine.keyhold[GLFW_KEY_RIGHT])
        engine.camera.yaw += speed * engine.deltaTime * 3;
    if(engine.keyhold[GLFW_KEY_LEFT])
        engine.camera.yaw -= speed * engine.deltaTime * 3;
    if(engine.keyhold[GLFW_KEY_UP])
        engine.camera.pitch += speed * engine.deltaTime * 3;
    if(engine.keyhold[GLFW_KEY_DOWN])
        engine.camera.pitch -= speed * engine.deltaTime * 3;
    engine.camera.applyNewRotation();
}

// make sure to load textures before loading shader
const uint AVT = engine.addTexture("./textures/avt.png");
const uint IDK = engine.addTexture("./textures/dcm.png");

int main(void) {
    engine.loadShader();

    engine.camera.position = glm::vec3(-100.0f, 50.0f, 300.0f);
    engine.camera.far = 1e6;
    engine.camera.fovy = 45;
    engine.camera.yaw += 25.0f;

    int volume = 100; // ^3
    for(int i = 0; i < volume; i++)
        for(int j = 0; j < volume; j++)
            for(int k = 0; k < volume; k++)
                engine.voxels.push_back(Voxel(glm::vec3(i, j, k),
                (i % 2 == 0 && j % 2 == 0 && k % 2 == 0) ? AVT : IDK));

    engine.start(func);

    return 0;
}