#ifndef DAY_5
#define DAY_5
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <iostream>
#include "../Shader.h"
#include "../stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Camera.h"

class Day_5
{
public:
    Day_5() {};
    ~Day_5();
    void InitGlWindow();
    void InitGLAD();
    void MainLoop();
    void InitShader();
    void BindData();
    void BindTexture();
    void SetCamera();
    void processInput(GLFWwindow *window);
private:
    Shader *mShader;
    GLFWwindow *mWindow; 
    unsigned int VBO, VAO, EBO;
    unsigned int texture1, texture2;
    float     deltaTime = 0.0f;
    float     lastFrame = 0.0f;

    void ClearColor();
    void ActivityTexture();
    void UpdateColor();
    void UpdateTransform();
    void CreateSomeModel();
};
#endif