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
#include <time.h>

struct Block
{
    glm::vec3 position;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    int shininess;
};

glm::vec3 materialAmbients[] = {
    glm::vec3(0.0215f, 0.1745f, 0.0215), // esmeralda
    glm::vec3(0.1745f, 0.01175f, 0.01175f),  //ruby
    glm::vec3(0.2125f, 0.1275f, 0.054f), //bronce
    glm::vec3(0.24725f, 0.1995f, 0.0745f), //oro
    glm::vec3(0.19225f, 0.19225f, 0.19225f) //plata
};

glm::vec3 materialDiffuses[] = {
    glm::vec3(0.07568f, 0.61424f, 0.07568f),
    glm::vec3(0.61424f, 0.04136f, 0.04136f),
    glm::vec3(0.714f, 0.4284f, 0.18144),
    glm::vec3(0.75164f, 0.60648f, 0.22648f),
    glm::vec3(0.50754f, 0.50754f, 0.50754f)
};

glm::vec3 materialSpeculars[] = {
    glm::vec3(0.633f, 0.727811f, 0.633f),
    glm::vec3(0.727811f, 0.626959f, 0.626959),
    glm::vec3(0.393548f, 0.271906f, 0.166721f),
    glm::vec3(0.628281f, 0.555802f, 0.366065),
    glm::vec3(0.508273f, 0.508273f, 0.508273f)
};

int materialShininess[] = {
    77, 77, 26, 51, 51
};

Camera camera(glm::vec3(0.0f, 3.25f, 10.0f));

glm::vec3 pointLightPositions[] = {
    glm::vec3(4.0f, 2.0f, 0.0f),
    glm::vec3(-4.0f, 2.0f, 0.0f)
};

glm::vec3 lightPositions[] = {
    glm::vec3(2.0f, 4.6f, 5.0f),
    glm::vec3(-2.0f, 4.6f, 5.0f)
};

glm::vec3 lifePositions[] = {
    glm::vec3(-5.0f, 7.0f, 0.0f),
    glm::vec3(-4.5f, 7.0f, 0.0f),
    glm::vec3(-4.0f, 7.0f, 0.0f)
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

const float k_shadow_near = 1.0f;
const float k_shadow_far = 37.5f;

float lastFrame = 0.0f;

bool gameStarted = false;
int lifes = 3;

void handleInput(float dt) {
    Input* input = Input::instance();

    //input de flechas para el paddle
    if (input->isKeyPressed(GLFW_KEY_RIGHT) && paddlePos.x < 4.2f)      
        paddlePos += dt * paddleSpeed * glm::vec3(1.0, 0.0, 0.0);

    if (input->isKeyPressed(GLFW_KEY_LEFT) && paddlePos.x > -3.68f)
        paddlePos -= dt * paddleSpeed * glm::vec3(1.0, 0.0, 0.0);

    if (input->isKeyPressed(GLFW_KEY_SPACE)) 
        gameStarted = true;
}

void onKeyPress(int key, int action) 
{
    if (key == GLFW_KEY_E && action == GLFW_PRESS)
        Window::instance()->setCaptureMode(false);
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


void renderScene(const Model& plate, const Shader& shader, const Geometry& quad, const Geometry& cube,
    const Geometry& sphere, const Texture& t_albedo, const Texture& t_specular, const Texture& t_paris) 
{
    //background
    t_paris.use(shader, "material.diffuse", 0);
    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(0.0f, 5.0f, -1.0f));
    model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
    shader.set("model", model);
    glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
    shader.set("normalMat", normalMat);
    quad.render();


    t_albedo.use(shader, "material.diffuse", 0);
    t_specular.use(shader, "material.specular", 1);
    //rightWall
    model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(5.5f, 3.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.5f, 6.5f, 1.0f));
    shader.set("model", model);
    normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
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
    const Texture& t_albedo, const Texture& t_specular,const Texture& t_paris,  Shadow shadow) 
{
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
        renderScene(plate, s_depth, quad, cube, sphere, t_albedo, t_specular, t_paris);
        //glCullFace(GL_BACK);
    }

    //SECOND PASS
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Window::instance()->getWidth(), Window::instance()->getHeight());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 proj = glm::perspective(glm::radians(camera.getFOV()), static_cast<float>(Window::instance()->getWidth()) / Window::instance()->getHeight(), 0.1f, 100.0f); 


    for (int lifeIndex = 0; lifeIndex < lifes; lifeIndex++)
    {
        s_light.use();
        glm::mat4 model = glm::mat4(1.0);
        model = glm::translate(model, lifePositions[lifeIndex]);
        s_light.set("model", model);
        s_light.set("view", view);
        s_light.set("proj", proj);
        s_light.set("lightColor", 0.0f, 1.0f, 0.0f);

        sphere.render();
    }

    for (Block block : gameBlocks)
    {
        glm::mat4 model = glm::mat4(1.0);
        model = glm::translate(model, block.position);
        model = glm::scale(model, blockSize);
        
        
        renderGameBlocks(cube, s_block, model, view, proj, block.ambient, block.diffuse, block.specular, block.shininess);           
    }

    s_phong.use();

    glm::mat4 model = glm::mat4(1.0);

    s_phong.set("model", model);
    s_phong.set("view", view);
    s_phong.set("proj", proj);

    s_phong.set("viewPos", camera.getPosition());

    s_phong.set("dirLight.direction", dirLightDirection);
    s_phong.set("dirLight.ambient", 0.01f, 0.01f, 0.01f);
    s_phong.set("dirLight.diffuse", 0.1f, 0.1f, 0.1f);
    s_phong.set("dirLight.specular", 0.2f, 0.2f, 0.2f);
    
    const std::string prefixPoints = "pointLight[";
    for (uint32_t i = 0; i < pointLightPositions->length(); ++i) 
    {
        const std::string lightName = prefixPoints + std::to_string(i) + "].";

        if (i == 1)
            s_phong.set((lightName + "position").c_str(), ballPos);
        else
            s_phong.set((lightName + "position").c_str(), glm::vec3(0.0f, 4.0f, -1.0f));

       


        s_phong.set((lightName + "ambient").c_str(), 0.00002f, 0.00002f, 0.000002f);
        s_phong.set((lightName + "diffuse").c_str(), glm::vec3(0.5f, 0.5f, 0.5f));
        s_phong.set((lightName + "specular").c_str(), glm::vec3(0.2f, 0.2f, 0.2f));
        s_phong.set((lightName + "constant").c_str(), 1.0f);
        s_phong.set((lightName + "linear").c_str(), 0.09f);
        s_phong.set((lightName + "quadratic").c_str(), 0.032f);
    }

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
    }

    renderScene(plate, s_phong, quad, cube,sphere, t_albedo, t_specular, t_paris);
}

void generateGameBlocks()
{
    srand(time(NULL));

    for (int j = 0; j < rows; j++)
    {
        for (int i = 0; i < blocksInRow; i++)
        {
            int materialPos = rand() % 5;
            glm::vec3 nextBLockPos(-9.5f / 2.0f + (i + 2) * blockSize.x, firstBlockPosition.y - j * blockSize.y, 0.0f);

            gameBlocks.push_back(Block{ nextBLockPos,
                                        materialAmbients[materialPos],
                                        materialDiffuses[materialPos],
                                        materialSpeculars[materialPos],
                                        materialShininess[materialPos] });
        };
    };
}

void startGame()
{
    gameBlocks.clear();
    generateGameBlocks();
    gameStarted = false;
    ballPos = initialBallPos;
    paddlePos = glm::vec3(0.0f, 0.0f, 0.0f);
    lifes = 3;
}

void die()
{
    gameStarted = false;
    ballPos = initialBallPos;
    lifes--;
}

void handleCollisions(float deltaTime)
{
    // paddle collisions
    float ballToPaddleDis = (ballPos.y - 0.1) - (0.5 / 2.0f);
    if (ballToPaddleDis < 0.1f) {
        if (ballPos.x <= paddlePos.x + 1.0f
            && ballPos.x >= paddlePos.x - 1.0f
            && ballPos.y > 0.0f)
        {
            ballDir.y *= -1.0;
        }
    }

    //Up wall collisions
    if (ballPos.y >= 5.7f)
        ballDir.y *= -1.0;

    //Right and left wall collisions
    if (ballPos.x <= -9.7f / 2.0f + 0.1 * 2.0f)
        ballDir.x *= -1.0;

    if (ballPos.x >= 10.7f / 2.0f - 0.1 * 2.0f)
        ballDir.x *= -1.0;

    //blocks collision
    int blockIterator = 0;
    for (Block block : gameBlocks)
    {
        float bottomDistance_BlockBall = glm::abs((block.position.y - blockSize.y / 2.0f) - (ballPos.y + 0.1f));
        float topDistance_BlockBall = glm::abs((ballPos.y - 0.1f) - (block.position.y + blockSize.y / 2.0f));
        float rightDistance_BlockBall = glm::abs((ballPos.x - 0.1f) - (block.position.x + blockSize.x / 2.0f));
        float leftDistance_BlockBall = glm::abs((ballPos.x + 0.1f) - (block.position.x - blockSize.x / 2.0f));


        if (bottomDistance_BlockBall < 0.05f || topDistance_BlockBall < 0.05f)
        {
            if (ballPos.x <= block.position.x + blockSize.x / 2.0f && ballPos.x >= block.position.x - blockSize.x / 2.0f) 
            {
                ballDir.y *= -1.0f;
                gameBlocks.erase(gameBlocks.begin() + blockIterator);
            }

            else if (rightDistance_BlockBall < 0.05f) 
            {
                ballDir.x *= -1.0;
                gameBlocks.erase(gameBlocks.begin() + blockIterator);
            }

            else if (leftDistance_BlockBall < 0.05f) 
            {
                ballDir.x *= -1.0;
                gameBlocks.erase(gameBlocks.begin() + blockIterator);
            }
        }
        blockIterator++;
    }
}

void update(float deltaTime)
{
    if (ballPos.y < -1.0f)
        die();

    if (lifes == 0)
        startGame();

    if (gameBlocks.empty())
        startGame();

    //Ball movement
    ballPos += glm::vec3(deltaTime * ballSpeed * ballDir.x, deltaTime * ballSpeed * ballDir.y, 0.0f);

    handleCollisions(deltaTime);
}


int main(int, char* []) { 
    Window* window = Window::instance();

    glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

    startGame();

    const Shader s_phong("../projects/FINAL/phong.vs", "../projects/FINAL/blinn.fs");
    const Shader s_block("../projects/FINAL/phong2.vs", "../projects/FINAL/blinn2.fs");
    const Shader s_depth("../projects/FINAL/depth.vs", "../projects/FINAL/depth.fs");
    const Shader s_debug("../projects/FINAL/debug.vs", "../projects/FINAL/debug.fs");
    const Shader s_light("../projects/FINAL/light.vs", "../projects/FINAL/light.fs");
    const Texture t_albedo("../assets/textures/bricks_albedo.png", Texture::Format::RGB);
    const Texture t_specular("../assets/textures/bricks_specular.png", Texture::Format::RGB);
    const Texture t_paris("../assets/textures/paris.jpg", Texture::Format::RGB);
    const Quad quad(2.0f);   
    const Cube cube(1.0f);
    const Sphere sphere(0.1f, 25, 25);
    const Model plate("../assets/models/plate/plate.obj");

    Shadow shadow = Shadow::Shadow();

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    Input::instance()->setKeyPressedCallback(onKeyPress);  

    while (window->alive()) 
    {
        const float currentFrame = glfwGetTime();
        const float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        handleInput(deltaTime);

        if (gameStarted)         
            update(deltaTime);           

        render(plate, quad, cube, sphere, s_phong, s_block, s_depth, s_debug, s_light, t_albedo, t_specular, t_paris, shadow);
        window->frame(); 
    }

    shadow.~Shadow();

    return 0;
}