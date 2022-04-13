# 索引缓冲
第一天我们实现了三角形，如果调整一下实现矩形(两个三角形) 怎么做呢
```c
    float colors[] = {
        -0.5f, -0.5f, 0.0f,1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f,  0.5f, 0.0f,0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f,1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f
    };
```
这么写虽然也可以但是  有点弱，明显 有两个点是重复的
可以用一个顶点索引
```c
float vertices[] = {
    0.5f, 0.5f, 0.0f,   // 右上角
    0.5f, -0.5f, 0.0f,  // 右下角
    -0.5f, -0.5f, 0.0f, // 左下角
    -0.5f, 0.5f, 0.0f   // 左上角
};

unsigned int indices[] = { // 注意索引从0开始! 
    0, 1, 3, // 第一个三角形
    1, 2, 3  // 第二个三角形
};
```
定义了四个点，但是opengl不支持直接画矩形，需要定义两个三角形，下面的顶点里就定义了  第几个点构成三角形，定义了两个。

这个跟顶点缓冲一样，需要从cpu传到gpu
在GPU创建一个EBO缓冲区
```c++
    unsigned int VBO, VAO, EBO;   // 1
    glGenBuffers(1, &EBO);        // 2
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);  // 3 
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicts), indicts, GL_STATIC_DRAW); // 4
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
```
整体就成了这样，
+ 创建一个ebo缓冲区
+ 绑定GL_ELEMENT_BUFFER到EBO缓冲区
+ 往GL_ELEMENT_BUFFER传数据，传到EBO

最后需要吧glDrawArray换成glDrawElement
```c++
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
```
# glsl 着色器语言
名称|含义|
|--|--|
vecn|包含n个float分量的默认向量
bvecn|包含n个bool分量的向量
ivecn|	包含n个int分量的向量
uvecn|	包含n个unsigned int分量的向量
dvecn|	包含n个double分量的向量
大多数时候我们使用vecn，因为float足够满足大多数要求了。

一个向量的分量可以通过vec.x这种方式获取，这里x是指这个向量的第一个分量。你可以分别使用.x、.y、.z和.w来获取它们的第1、2、3、4个分量。GLSL也允许你对颜色使用rgba，或是对纹理坐标使用stpq访问相同的分量。

向量这一数据类型也允许一些有趣而灵活的分量选择方式，叫做重组(Swizzling)。重组允许这样的语法：
```c
vec2 someVec;
vec4 differentVec = someVec.xyxx;
vec3 anotherVec = differentVec.zyw;
vec4 otherVec = someVec.xxxx + anotherVec.yxzy;
```

```c
vec2 vect = vec2(0.5, 0.7);
vec4 result = vec4(vect, 0.0, 0.0);
vec4 otherResult = vec4(result.xyz, 1.0);
```

## 输入输出
glsl中用 ***in***  ***out*** 表示输出输出
为了定义顶点数据该如何管理，我们使用 ***location*** 这一元数据指定输入变量，这样我们才可以在CPU上配置顶点属性。我们已经在前面的教程看过这个了， ***layout (location = 0)*** 。顶点着色器需要为它的输入提供一个额外的layout标识，这样我们才能把它链接到顶点数据。

```js
你也可以忽略layout (location = 0)标识符，通过在OpenGL代码中使用glGetAttribLocation查询属性位置值(Location)，但是我更喜欢在着色器中设置它们，这样会更容易理解而且节省你（和OpenGL）的工作量。
```

## Uniform
Uniform是一种从CPU中的应用向GPU中的着色器发送数据的方式，但uniform和顶点属性有些不同。首先，uniform是全局的(Global)。全局意味着uniform变量必须在每个着色器程序对象中都是独一无二的，而且它可以被着色器程序的任意着色器在任意阶段访问。第二，无论你把uniform值设置成什么，uniform会一直保存它们的数据，直到它们被重置或更新。




```flow
st=>start: 开始
op=>operation: My Operation
cond=>condition: Yes or No?
e=>end
st->op->cond
cond(yes)->e
cond(no)->op
&```
