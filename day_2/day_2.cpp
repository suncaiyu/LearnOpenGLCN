#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertex_data = "#version 430 core\n"
                          "layout (location = 0) in vec3 aPos;\n"
                          "in vec3 aColor; \n"
                          "out vec3 myColor; \n"
                          "void main()\n"
                          "{\n"
                          "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                          "   myColor = aColor;\n"
                          "}\0";
const char *fragment_data = "#version 430 core\n"
                            "out vec4 FragColor;\n"
                            "in vec3 myColor;\n"
                            "layout (location = 2) uniform vec2 toumingdu;\n"
                            "void main()\n"
                            "{\n"
                            "   FragColor = vec4(myColor.x, toumingdu.x, myColor.z, 1.0f);\n"
                            "}\n\0";

void compileIsSuccess(unsigned int &shader, unsigned int status)
{
    int success;
    char log[512];
    glGetShaderiv(shader, status, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << log << std::endl;
    }
}

unsigned int getvertexShader()
{
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_data, nullptr);
    glCompileShader(vertexShader);
    compileIsSuccess(vertexShader, GL_COMPILE_STATUS);
    return vertexShader;
}

unsigned int getfragmentShader()
{
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_data, nullptr);
    glCompileShader(fragmentShader);
    compileIsSuccess(fragmentShader, GL_COMPILE_STATUS);
    return fragmentShader;
}

unsigned int createProgram(unsigned int &vertex, unsigned int &fragment)
{
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    compileIsSuccess(program, GL_LINK_STATUS);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return program;
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL_DAY_2", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 我的代码
    // 这里是着色器程序
    unsigned int vertexShader = getvertexShader();
    unsigned int fragmentShader = getfragmentShader();
    unsigned int program = createProgram(vertexShader, fragmentShader);
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left
        0.5f, -0.5f, 0.0f,  // right
        0.0f, 0.5f, 0.0f    // top
    };
    // 这样虽然可以  画出矩形，但是明显有两个点重复了
    // float colors[] = {
    //     -0.5f, -0.5f, 0.0f,1.0f, 0.0f, 0.0f,
    //     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    //     0.5f,  0.5f, 0.0f,0.0f, 0.0f, 1.0f,
    //     -0.5f, -0.5f, 0.0f,1.0f, 0.0f, 0.0f,
    //     0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    //     -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f
    // };

    float colors[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f};

    unsigned int indicts[] = {
        2, 1, 3,
        1, 0, 3};

    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    unsigned int colorlocation = glGetAttribLocation(program, "aColor");
    glVertexAttribPointer(colorlocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicts), indicts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        glClearColor(0.1f, 0.6f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        // 更新uniform颜色
        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f;
        // int vertexColorLocation = glGetUniformLocation(program, "toumingdu");
        glUniform2f(2, greenValue, 0.0f);

        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        // glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(program);
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
