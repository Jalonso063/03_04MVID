#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/camera.hpp"
#include "engine/geometry/cube.hpp"
#include "engine/input.hpp"
#include "engine/shader.hpp"
#include "engine/texture.hpp"
#include "engine/window.hpp"
#include "engine/geometry/sphere.hpp"
#include "engine/geometry/quad.hpp"
#include <iostream>
#include <engine\model.hpp>
#include <engine\game\game.hpp>
#include <engine\shadow.hpp>

struct Block
{
    glm::vec3 position;
};

glm::vec3 ambient(0.0215f, 0.1745f, 0.0215);
glm::vec3 diffuse(0.07568f, 0.61424f, 0.07568f);
glm::vec3 specular(0.633f, 0.727811f, 0.633f);
int shininess = 76;


Camera camera(glm::vec3(0.0f, 3.25f, 10.0f));

glm::vec3 lightPositions[] = {
    glm::vec3(1.2f, 3.0f, 2.0f),
    glm::vec3(0.0f, 3.0f, -3.0f)
};

glm::vec3 dirLightDirection(-0.2f, -1.0f, -0.3f);
glm::vec3 lightPos(4.0f, 20.0f, 0.0f);

glm::vec3 initialBallPos(0.0f, 2.0f, 0.0f);
glm::vec3 ballPos(0.0f, 2.0f, 0.0f);
glm::vec3 ballDir(0.2f, -1.f, 0.0f);
glm::vec3 ballSize(glm::vec3(0.5f));
float ballSpeed = 2.0f;

glm::vec3 paddlePos(0.0f, 0.0f, 0.0f);
glm::vec3 paddleSize(10.0f, 10.0f, 2.0f);
float paddleSpeed = 5.0f;

std::vector<Block> gameBlocks;
glm::vec3 firstBlockPosition(0.0f, 4.6f, 0.0f);
glm::vec3 blockSize(0.7f, 0.3f, 1.0f);
const uint32_t rows = 4;
const uint32_t blocksInRow = 11;
glm::vec3 blockPositions[rows][blocksInRow];

const uint32_t k_shadow_height = 1024;
const uint32_t k_shadow_width = 1024;
const float k_shadow_near = 1.0f;
const float k_shadow_far = 37.5f;

float lastFrame = 0.0f;
float lastX, lastY;
bool firstMouse = true;

bool gameStarted = false;

void handleInput(float dt) {
    Input* input = Input::instance();

    if (input->isKeyPressed(GLFW_KEY_W)) {
        camera.handleKeyboard(Camera::Movement::Forward, dt);
    }
    if (input->isKeyPressed(GLFW_KEY_S)) {
        camera.handleKeyboard(Camera::Movement::Backward, dt);
    }
    if (input->isKeyPressed(GLFW_KEY_A)) {
        camera.handleKeyboard(Camera::Movement::Left, dt);
    }
    if (input->isKeyPressed(GLFW_KEY_D)) {
        camera.handleKeyboard(Camera::Movement::Right, dt);
    }


    //input de flechas para el paddle
    if (input->isKeyPressed(GLFW_KEY_RIGHT)) {
        paddlePos += dt * paddleSpeed * glm::vec3(1.0, 0.0, 0.0);
    }
    if (input->isKeyPressed(GLFW_KEY_LEFT)) {
        paddlePos -= dt * paddleSpeed * glm::vec3(1.0, 0.0, 0.0);
    }

    if (input->isKeyPressed(GLFW_KEY_SPACE)) {
        gameStarted = true;
    }


}

void onKeyPress(int key, int action) {
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        Window::instance()->setCaptureMode(true);
    }

    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        Window::instance()->setCaptureMode(false);
    }
}

void onMouseMoved(float x, float y) {
    if (firstMouse) {
        firstMouse = false;
        lastX = x;
        lastY = y;
    }

    const float xoffset = x - lastX;
    const float yoffset = lastY - y;
    lastX = x;
    lastY = y;

    camera.handleMouseMovement(xoffset, yoffset);
}

void onScrollMoved(float x, float y) {
    camera.handleMouseScroll(y);
}

void renderGameBlocks(const Geometry& object, const Shader& shader, glm::mat4 model, glm::mat4 view, glm::mat4 proj,
                       glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, int shininess)
{
    shader.use();

    shader.set("model", model);
    shader.set("view", view);
    shader.set("proj", proj);

    glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
    shader.set("normalMat", normalMat);

    shader.set("viewPos", camera.getPosition());

    shader.set("material.ambient", ambient.r, ambient.g, ambient.b);
    shader.set("material.diffuse", diffuse.r, diffuse.g, diffuse.b);
    shader.set("material.specular", specular.r, specular.g, specular.b);
    shader.set("material.shininess", shininess);

    shader.set("light.position", glm::vec3(4.0f, 1.0f, 4.0f));
    shader.set("light.ambient", 0.1f, 0.1f, 0.1f);
    shader.set("light.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.set("light.specular", 1.0f, 1.0f, 1.0f);

    object.render();
}


void renderScene(const Model& plate, const Shader& shader, const Shader& s_block, const Geometry& quad, const Geometry& cube,
    const Geometry& sphere, const Texture& t_albedo, const Texture& t_specular) {
    t_albedo.use(shader, "material.diffuse", 0);
    t_specular.use(shader, "material.specular", 1);

    //glm::mat4 model = glm::mat4(1.0);
    //model = glm::translate(model, glm::vec3(0.0f, 5.0f, -5.0f));
    ////model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    //model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
    //shader.set("model", model);
    //glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
    //shader.set("normalMat", normalMat);
    //quad.render();

    //rightWall
    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(5.5f, 3.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.5f, 6.5f, 1.0f));
    shader.set("model", model);
    glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
    shader.set("normalMat", normalMat);
    cube.render();
    
    //leftWall
    model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(-5.0f, 3.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.5f, 6.5f, 1.0f));
    shader.set("model", model);
    normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
    shader.set("normalMat", normalMat);
    cube.render();
    
    //topWall
    model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3( 0.25f, 6.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(10.0f, 1.0, 0.5f));
    shader.set("model", model);
    normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
    shader.set("normalMat", normalMat);
    cube.render(); 

    /*for (Block block : gameBlocks)
    {
        model = glm::mat4(1.0);
        model = glm::translate(model, block.position);
        model = glm::scale(model, blockSize);
        glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
        shader.set("normalMat", normalMat);

        shader.set("viewPos", camera.getPosition());

        shader.set("material.ambient", ambient.r, ambient.g, ambient.b);
        shader.set("material.diffuse", diffuse.r, diffuse.g, diffuse.b);
        shader.set("material.specular", specular.r, specular.g, specular.b);
        shader.set("material.shininess", shininess);

        shader.set("light.position", glm::vec3(4.0f, 1.0f, 4.0f));
        shader.set("light.ambient", 0.1f, 0.1f, 0.1f);
        shader.set("light.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
        shader.set("light.specular", 1.0f, 1.0f, 1.0f);

        cube.render();
    }*/

    //sphere
    model = glm::mat4(1.0);
    model = glm::translate(model, ballPos);
    //model = glm::scale(model, glm::vec3(0.1f));
    shader.set("model", model);
    normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
    shader.set("normalMat", normalMat);
    sphere.render();

    //paddle
    model = glm::mat4(1.0);
    //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::translate(model, paddlePos);
    model = glm::scale(model, paddleSize);
    shader.set("model", model);
    normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
    shader.set("normalMat", normalMat);
    plate.render(shader);

   
}

void render(const Model& plate, const Geometry& quad, const Geometry& cube, const Geometry& sphere,
    const Shader& s_phong,const Shader& s_block, const Shader& s_depth, const Shader& s_debug, const Shader& s_light,
    const Texture& t_albedo, const Texture& t_specular,  Shadow shadow) {

    //FIRST PASS
    shadow.render();

    std::vector<glm::mat4> lightSpaceMatrixes;

    for (glm::vec3 lightPos : lightPositions)
    {
        const glm::mat4 lightProjection = glm::perspective(glm::radians(45.0f), static_cast<float>(Window::instance()->getWidth()) / Window::instance()->getHeight(), k_shadow_near, k_shadow_far);
        const glm::mat4 lightView = glm::lookAt(lightPos, -lightPos, glm::vec3(0.0f, 1.0f, 0.0f));
        const glm::mat4 lightSpaceMatrix = lightProjection * lightView;
        lightSpaceMatrixes.push_back(lightSpaceMatrix);


        s_depth.use();
        s_depth.set("lightSpaceMatrix", lightSpaceMatrix);
        //glCullFace(GL_FRONT);
        renderScene(plate, s_depth, s_block, quad, cube, sphere, t_albedo, t_specular);
        //glCullFace(GL_BACK);
    }

    //SECOND PASS
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Window::instance()->getWidth(), Window::instance()->getHeight());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 proj = glm::perspective(glm::radians(camera.getFOV()), static_cast<float>(Window::instance()->getWidth()) / Window::instance()->getHeight(), 0.1f, 100.0f);

    for (glm::vec3 lightPos : lightPositions)
    {
        s_light.use();
        glm::mat4 model = glm::mat4(1.0);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        s_light.set("model", model);
        s_light.set("view", view);
        s_light.set("proj", proj);
        s_light.set("lightColor", 1.0f, 1.0f, 1.0f);

        sphere.render();
    }  

    for (Block block : gameBlocks)
    {
        glm::mat4 model = glm::mat4(1.0);
        model = glm::translate(model, block.position);
        model = glm::scale(model, blockSize);
        
        
        renderGameBlocks(cube, s_block, model, view, proj, ambient, diffuse, specular, shininess);
           
    }

    s_phong.use();

    glm::mat4 model = glm::mat4(1.0);

    s_phong.set("model", model);
    s_phong.set("view", view);
    s_phong.set("proj", proj);

    s_phong.set("viewPos", camera.getPosition());

    s_phong.set("dirLight.direction", dirLightDirection);
    s_phong.set("dirLight.ambient", 0.5f, 0.5f, 0.5f);
    s_phong.set("dirLight.diffuse", 0.1f, 0.1f, 0.1f);
    s_phong.set("dirLight.specular", 0.2f, 0.2f, 0.2f);

    const std::string prefixSpotPoints = "spotLight[";
    for (uint32_t i = 0; i < lightPositions->length(); ++i)
    {
        const std::string lightName = prefixSpotPoints + std::to_string(i) + "].";

        s_phong.set((lightName + "position").c_str(), lightPositions[i]);
        s_phong.set((lightName + "direction").c_str(), -lightPositions[i]);
        s_phong.set((lightName + "ambient").c_str(), 0.1f, 0.1f, 0.1f);
        s_phong.set((lightName + "diffuse").c_str(), 0.5f, 0.5f, 0.5f);
        s_phong.set((lightName + "specular").c_str(), 1.0f, 1.0f, 1.0f);
        s_phong.set((lightName + "constant").c_str(), 1.0f);
        s_phong.set((lightName + "linear").c_str(), 0.02f);
        s_phong.set((lightName + "quadratic").c_str(), 0.06f);
        s_phong.set((lightName + "cutOff").c_str(), glm::cos(glm::radians(20.0f)));
        s_phong.set((lightName + "outerCutOff").c_str(), glm::cos(glm::radians(25.0f)));

        t_albedo.use(s_phong, "material.diffuse", 0);
        t_specular.use(s_phong, "material.specular", 1);
        s_phong.set("material.shininess", 32);

        s_phong.set("lightSpaceMatrix[0]", lightSpaceMatrixes[0]);
        s_phong.set("lightSpaceMatrix[1]", lightSpaceMatrixes[1]);

        shadow.bindTexture(s_phong);

        renderScene(plate, s_phong, s_block, quad, cube,sphere, t_albedo, t_specular);
    }
}

int main(int, char* []) { 
    Window* window = Window::instance();

    glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

    Game game = Game::Game();

    //game.init();

    

    //Block positions
    for (int j = 0; j < rows; j++) 
    {
        for (int i = 0; i < blocksInRow; i++) 
        {
            glm::vec3 nextBLockPos(-9.5f / 2.0f + (i + 2) * blockSize.x, firstBlockPosition.y - j * blockSize.y, 0.0f);            
            gameBlocks.push_back(Block{ nextBLockPos });
        };
    };


    const Shader s_phong("../projects/FINAL/phong.vs", "../projects/FINAL/blinn.fs");
    const Shader s_block("../projects/FINAL/phong2.vs", "../projects/FINAL/blinn2.fs");
    const Shader s_depth("../projects/FINAL/depth.vs", "../projects/FINAL/depth.fs");
    const Shader s_debug("../projects/FINAL/debug.vs", "../projects/FINAL/debug.fs");
    const Shader s_light("../projects/FINAL/light.vs", "../projects/FINAL/light.fs");
    const Texture t_albedo("../assets/textures/bricks_albedo.png", Texture::Format::RGB);
    const Texture t_specular("../assets/textures/bricks_specular.png", Texture::Format::RGB);
    const Quad quad(2.0f);   
    const Cube cube(1.0f);
    const Sphere sphere(0.1f, 25, 25);
    const Model plate("../assets/models/plate/plate.obj");

    Shadow shadow = Shadow::Shadow();

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    Input::instance()->setKeyPressedCallback(onKeyPress);
    Input::instance()->setMouseMoveCallback(onMouseMoved);
    Input::instance()->setScrollMoveCallback(onScrollMoved);

    while (window->alive()) {
        const float currentFrame = glfwGetTime();
        const float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        handleInput(deltaTime);


        //update block
        if (gameStarted) {

            if (ballPos.y < -1.0f ) {
                gameStarted = false;
                ballPos = initialBallPos;
                //lifes--;
            }

            //Ball movement
            ballPos += glm::vec3(deltaTime * ballSpeed * ballDir.x, deltaTime * ballSpeed * ballDir.y, 0.0f);

            // paddle collisions
            float distance_BarBall = (ballPos.y - 0.1) - (0.5 / 2.0f);
            if (distance_BarBall < 0.1f) {
                if (ballPos.x <= paddlePos.x + paddleSize.x / 2.0f
                    && ballPos.x >= paddlePos.x - paddleSize.x / 2.0f
                    && ballPos.y > 0.0f)
                {
                    ballDir.y = -1.0 * ballDir.y;
                }
            }



            //Up wall collisions
            if (ballPos.y >= 6.0f)
                ballDir.y = -1.0 * ballDir.y;

            //Right and left wall collisions
            if (ballPos.x <= -11.0f / 2.0f + 0.1 * 2.0f)
                ballDir.x = -1.0 * ballDir.x;            

            if (ballPos.x >= 11.0f / 2.0f - 0.1 * 2.0f)
                ballDir.x = -1.0 * ballDir.x;

            //blocks collision
            int blockIterator = 0;
            for (Block block : gameBlocks)
            {
                float bottomDistance_BlockBall = glm::abs((block.position.y - blockSize.y / 2.0f) - (ballPos.y + 0.1f));
                float topDistance_BlockBall = glm::abs((ballPos.y - 0.1f) - (block.position.y + blockSize.y / 2.0f));
                float rightDistance_BlockBall = glm::abs((ballPos.x - 0.1f) - (block.position.x + blockSize.x / 2.0f));
                float leftDistance_BlockBall = glm::abs((ballPos.x + 0.1f) - (block.position.x - blockSize.x / 2.0f));


                if (bottomDistance_BlockBall < 0.05f) {
                    if (ballPos.x <= block.position.x + blockSize.x / 2.0f && ballPos.x >= block.position.x - blockSize.x / 2.0f) {
                        ballDir.y = -1.0f * ballDir.y;                        
                        gameBlocks.erase(gameBlocks.begin() + blockIterator);
                    }

                    else if (rightDistance_BlockBall < 0.05f) {
                        ballDir.x = -1.0 * ballDir.x;                        
                        gameBlocks.erase(gameBlocks.begin() + blockIterator);
                    }

                    else if (leftDistance_BlockBall < 0.05f) {
                        ballDir.x = -1.0 * ballDir.x;                        
                        gameBlocks.erase(gameBlocks.begin() + blockIterator);
                    }
                }
                if (topDistance_BlockBall < 0.05f) {
                    if (ballPos.x <= block.position.x + blockSize.x / 2.0f && ballPos.x >= block.position.x - blockSize.x / 2.0f) {
                        ballDir.y = -1.0f * ballDir.y;                        
                        gameBlocks.erase(gameBlocks.begin() + blockIterator);
                    }

                    else if (rightDistance_BlockBall < 0.05f) {
                        ballDir.x = -1.0f * ballDir.x;                        
                        gameBlocks.erase(gameBlocks.begin() + blockIterator);
                    }

                    else if (leftDistance_BlockBall < 0.05f) {
                        ballDir.x = -1.0 * ballDir.x;                        
                        gameBlocks.erase(gameBlocks.begin() + blockIterator);
                    }
                }
                blockIterator++;
            }

        }


        // FIN Update Block

        render(plate, quad, cube, sphere, s_phong, s_block, s_depth, s_debug, s_light, t_albedo, t_specular, shadow);
        window->frame(); 
        //game.render();
    }

    shadow.~Shadow();

    return 0;
}