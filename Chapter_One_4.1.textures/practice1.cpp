#include <glad/glad.h> 
#include <GLFW\glfw3.h>

#include"Shader.h"
#include"stb_image.h"
#include<iostream>


float vertices[] = {
    //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
};

unsigned int indices[] = {
    // 注意索引从0开始! 
    // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
    // 这样可以由下标代表顶点组合成矩形

    0, 1, 3, // 第一个三角形
    1, 2, 3  // 第二个三角形
};



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







    unsigned int texture1, texture2;
    ///////////////texture1/////////////
    //生成一个纹理的过程
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    // 加载并生成纹理
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);//反转y轴
    unsigned char* data = stbi_load("textures/5x01891.JPG", &width, &height, &nrChannels, 0);
    if (data)
    {
        //当调用glTexImage2D时，当前绑定的纹理对象就会被附加上纹理图像
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);




    ///////////////texture2/////////////

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 加载并生成纹理
    int width1, height1, nrChannels1;
    stbi_set_flip_vertically_on_load(true);//反转y轴
    unsigned char* data1 = stbi_load("textures/awesomeface.png", &width1, &height1, &nrChannels1, 0);
    if (data1)
    {
        //当调用glTexImage2D时，当前绑定的纹理对象就会被附加上纹理图像
        //请注意，Awesomeface.png具有透明度，因此具有alpha通道，因此请确保告诉OpenGL数据类型为GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width1, height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data1);






    //生成一个VAO对象
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    //ID生成一个VBO对象
    unsigned int VBO;
    glGenBuffers(1, &VBO);


    //生成一个EBO对象
    unsigned int EBO;
    glGenBuffers(1, &EBO);





    //绑定VAO
    glBindVertexArray(VAO);

    //把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上：
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //把之前定义的顶点数据复制到缓冲的内存中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    //绑定EBO并把索引数据复制到缓冲的内存中
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



    //告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）
    //第一个0就是着色器里的layout (location = 0)个属性位置值

    //位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 纹理坐标属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);




    //用线框模式绘制你的三角形
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



    Shader myshader("ShaderPath/practice1/shader.vs", "ShaderPath/practice1/shader.fs");

    //设置每个采样器的方式告诉OpenGL每个着色器采样器属于哪个纹理单元。我们只需要设置一次即可
    myshader.use();// 不要忘记在设置uniform变量之前激活着色器程序！
    glUniform1i(glGetUniformLocation(myshader.ID, "texture1"), 0); // 手动设置
    myshader.setInt("texture2", 1); // 或者使用着色器类设置


    //渲染循环
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);


        //在每个新的渲染迭代开始的时候我们总是希望清屏
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        //需要绑定纹理对象到对应的纹理单元
        glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活纹理单元
        glBindTexture(GL_TEXTURE_2D, texture1);

        glActiveTexture(GL_TEXTURE1); // 在绑定纹理之前先激活纹理单元
        glBindTexture(GL_TEXTURE_2D, texture2);


        myshader.use();// 不要忘记在设置uniform变量之前激活着色器程序！


        // 4. 绘制物体
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);//解绑VAO



        glfwPollEvents();
        glfwSwapBuffers(window);

    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);



    //当渲染循环结束后我们需要正确释放/删除之前的分配的所有资源
    glfwTerminate();
    return 0;
}