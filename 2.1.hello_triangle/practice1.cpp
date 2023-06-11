#include <glad/glad.h> 
#include <GLFW\glfw3.h>

#include<iostream>


//指定三个顶点，每个顶点都有一个3D位置
float vertices[] = {

    -0.4f,0.0f,0.0f,
    0.0f,0.0f,0.0f,
    -0.2f,0.2f,0.0f,

    0.0f,0.0f,0.0f,
    0.4f,0.0f,0.0f,
    0.2f,0.2f,0.0f
};


unsigned int indices[] = {
    // 注意索引从0开始! 
    // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
    // 这样可以由下标代表顶点组合成矩形

    0, 1, 3, // 第一个三角形
    1, 2, 3  // 第二个三角形
};


//我们暂时将顶点着色器的源代码硬编码在代码文件顶部的C风格字符串中：
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";


const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";


//当用户改变窗口的大小的时候，视口也应该被调整
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //告诉OpenGL渲染窗口的尺寸大小
    glViewport(0, 0, width, height);
}


//这里我们检查用户是否按下了返回键(Esc),如果用户的确按下了返回键,
//把WindowShouldClose属性设置为 true,关闭GLFW。下一次while循环的条件检测将会失败，程序将会关闭。
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}



int main()
{
    //初始化GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);




    //创建一个窗口对象,窗口对象而且会被GLFW的其他函数频繁地用到
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    //通知GLFW将我们窗口的上下文设置为当前线程的主上下文了
    glfwMakeContextCurrent(window);


    //告诉GLFW我们希望每当窗口调整大小的时候调用这个函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);




    //GLAD是用来管理OpenGL的函数指针的,所以在调用任何OpenGL的函数之前我们需要初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    //生成一个VAO对象
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    //ID生成一个VBO对象
    unsigned int VBO;
    glGenBuffers(1, &VBO);


    //生成一个EBO对象
    unsigned int EBO;
    glGenBuffers(1, &EBO);




    //创建一个顶点着色器
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    //把这个着色器源码附加到着色器对象上，然后编译它：
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //检测在调用glCompileShader后编译是否成功了
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }



    //创建并编译片段着色器，过程与顶点着色器类似
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);


    //两个着色器现在都编译了，剩下的事情是把两个着色器对象链接到一个用来渲染的着色器程序(Shader Program)中。

    //创建着色器程序对象
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    //把之前编译的顶点、片段着色器附加到程序对象上，然后用glLinkProgram链接它们：
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //检测在调用glLinkProgram后编译是否成功了
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::shaderProgram::LINK_FAILED\n" << infoLog << std::endl;
    }

    //把着色器对象链接到程序对象以后，记得删除着色器对象，我们不再需要它们了：
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);








    //绑定VAO
    glBindVertexArray(VAO);


    //把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上：
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //把之前定义的顶点数据复制到缓冲的内存中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    //告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）
    //第一个0就是着色器里的layout (location = 0)个属性位置值
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    //绑定EBO然后用glBufferData把索引复制到缓冲里
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    //用线框模式绘制你的三角形
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //渲染循环
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);


        //在每个新的渲染迭代开始的时候我们总是希望清屏
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);



        // 4. 绘制物体
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);//解绑VAO



        glfwPollEvents();
        glfwSwapBuffers(window);

    }



    //当渲染循环结束后我们需要正确释放/删除之前的分配的所有资源
    glfwTerminate();
    return 0;
}