# opengl第一天

## opengl学习使用的库介绍
- glfw 方便在各个平台上生成窗口的
+ glad 方便在运行时动态获取opengl函数的指针入口的
```c++
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
```

## 1. 一个概念VBO
名称|大概作用
--|--
顶点缓冲对象|在GPU测开辟的一段内存空间
着色器直接调GPU测的数据会比较快

1. 如何生成缓冲对象  glGenBuffers
```c++
unsigned int VBO;
glGenBuffers(1, &VBO);
```
2. 如何把CPU数据送到GPU
   1. 把一个特定的buffer绑定到VBO
   顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER
   ```c++
   glBindBuffer(GL_ARRAY_BUFFER, VBO);  
   ```
   2. 调用函数 glBufferData 传数据
   ```c++
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
   ```
   GL_STATIC_DRAW 表示不会改变的
    - GL_STATIC_DRAW ：数据不会或几乎不会改变。
    + GL_DYNAMIC_DRAW：数据会被改变很多。
    - GL_STREAM_DRAW ：数据每次绘制时都会改变。

## 完成渲染的最低条件
- 至少一个顶点着色器和一个片段着色器

## 2. 着色器的大概顺序
1. 顶点着色器输入
```c
e.g.
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
```
2. 编译顶点着色器
```c++
unsigned int vertexShader;
vertexShader = glCreateShader(GL_VERTEX_SHADER);
glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
glCompileShader(vertexShader);
int  success;
char infoLog[512];
glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
if(!success)
{
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
}
```
3. 片段着色器输入
```c
e.g.
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
} 
```
4. 编译片段着色器
5. 生成着色器程序
6. 把顶点和片段着色器连接到着色器程序上
7. 删除顶点 片段着色器
- 这样之后就可以使用glUseProgram激活启用这个着色器程序

## 3. 连接1 和 2
 - 1中说明了如何把我们的数据送到GPU
 - 2中说明了如果我们得到了GPU的数据，我们怎么给shaderprograme使用
 - 所以结合一下，就成了

 给shaderprogeam 解释如何读取理解送到GPU的数据
 ```c++
 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
 ```
 glVertexAttribPointer函数的参数非常多，所以我会逐一介绍它们：

- 第一个参数指定我们要配置的顶点属性。还记得我们在顶点着色器中使用layout(location = 0)定义了position顶点属性的位置值(Location)吗？它可以把顶点属性的位置值设置为0。因为我们希望把数据传递到这一个顶点属性中，所以这里我们传入0。
- 第二个参数指定顶点属性的大小。顶点属性是一个vec3，它由3个值组成，所以大小是3。
- 第三个参数指定数据的类型，这里是GL_FLOAT(GLSL中vec*都是由浮点数值组成的)。
- 下个参数定义我们是否希望数据被标准化(Normalize)。如果我们设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间。我们把它设置为GL_FALSE。
- 第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。由于下个组位置数据在3个float之后，我们把步长设置为3 * sizeof(float)。要注意的是由于我们知道这个数组是紧密排列的（在两个顶点属性之间没有空隙）我们也可以设置为0来让OpenGL决定具体步长是多少（只有当数值是紧密排列时才可用）。一旦我们有更多的顶点属性，我们就必须更小心地定义每个顶点属性之间的间隔，我们在后面会看到更多的例子（译注: 这个参数的意思简单说就是从这个属性第二次出现的地方到整个数组0位置之间有多少字节）。
- 最后一个参数的类型是void*，所以需要我们进行这个奇怪的强制类型转换。它表示位置数据在缓冲中起始位置的偏移量(Offset)。由于位置数据在数组的开头，所以这里是0。我们会在后面详细解释这个参数。

使用glEnableVertexAttribArray，以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的

总体过程大概是这样
``` java
// 0. 复制顶点数组到缓冲中供OpenGL使用
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
// 1. 设置顶点属性指针
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
// 2. 当我们渲染一个物体时要使用着色器程序
glUseProgram(shaderProgram);
// 3. 绘制物体
someOpenGLFunctionThatDrawsOurTriangle();
```


## 4 一个概念VAO
目前理解的还不是很透彻，我自己理解就是对于VBO的一些操作会被记录在VAO中，方便后面重复使用
一个顶点数组对象会储存以下这些内容：

+ glEnableVertexAttribArray和glDisableVertexAttribArray的调用。
+ 通过glVertexAttribPointer设置的顶点属性配置。
+ 通过glVertexAttribPointer调用与顶点属性关联的顶点缓冲对象。

![avatar](https://learnopengl-cn.github.io/img/01/04/vertex_array_objects.png)
```c++
// ..:: 初始化代码（只运行一次 (除非你的物体频繁改变)） :: ..
unsigned int VAO;
glGenVertexArrays(1, &VAO);
// 1. 绑定VAO
glBindVertexArray(VAO);
// 2. 把顶点数组复制到缓冲中供OpenGL使用
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
// 3. 设置顶点属性指针
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

[...]

// ..:: 绘制代码（渲染循环中） :: ..
// 4. 绘制物体
glUseProgram(shaderProgram);
glBindVertexArray(VAO);
someOpenGLFunctionThatDrawsOurTriangle();
```

***踩过的坑***
- 在用 visual studio 编译项目的时候很顺利。
- 但是在用vscode + mingw64 编译的时候遇到了一个坑，当在task.json中把编译的命令 -g "文件"  -I “引用头文件路径” -L “引用库文件路径” -l "库名称" 搞完玩 编译时，如果发现报一堆libglfw3.a(win32_monitor.c.obj):win32_monitor.c:(.text+0x172): undefined reference to `__imp_CreateDCW’ 一系列此等undefined reference to的错误   不要慌，不是你的错
- 网上的回答【问题：理论上，教程到这就完成了，GLFW环境配置搞定了，但是当你编译时却发现libglfw3.a(win32_monitor.c.obj):win32_monitor.c:(.text+0x172): undefined reference to `__imp_CreateDCW’ 一系列此等undefined reference to的错误】【处理过程：可以发现其实当libglfw3.a不链接的时候，我们的glfwInit();直接无法识别，那么其实我链接的其实没啥问题，是libglfw3.a库中的函数找不到定义，说明可能缺少其他链接文件，我查询CreateDCW函数的库是什么，在网上找到是libgdi32.a，确实也有相似的错误是未添加此静态库导致的，但是并处理后并没有解决问题；发现函数中有__imp_前缀，再次找解决问题的库，网上有加libwinmm.a解决问题的，说是libglfw3.a在编译后会添上这个函数前缀导致找不到定义，VS默认解决此问题，我一度认为这就是最终解决方案了，但是在加入这个静态库后还是同样的错误】
【问题的解决：在Stack Overflow网站上浏览相同错误，找到一个错误相同的帖子（链接），但此贴楼主是同时使用了动态库和静态库造成错误，解决方法是干掉静态库。我也看了许多外网贴，在解决不了静态库情况下，我寻求动态库，如何生成动态库？此坑回到最开始的教程，cmake的生成文件即一排红色的那些，不要和教程一样默认，勾选share… IDE 进行build生成动态库，编译器设置链接，解决问题】我用的就是不用静态库，使用动态库就好了