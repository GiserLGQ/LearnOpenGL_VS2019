
#include <glad/glad.h> 
#include <GLFW/glfw3.h>


#include<iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"personal_h/Shader.h"
#include"personal_h/camera.h"




// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;



// 创建camera
//Camera camera(glm::vec3(2.0f, 0.0f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f),
//    120, 10);
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

//我们跟踪两个全局变量来计算出deltaTime值：
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

//我们必须先在程序中储存上一帧的鼠标位置
float lastX = SCR_WIDTH / 2.0f, lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//光源在场景的世界空间坐标中的位置
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

//立方体cube顶点
float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};




//当用户改变窗口的大小的时候，视口也应该被调整
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//这里我们检查用户是否按下了返回键(Esc),如果用户的确按下了返回键,
//把WindowShouldClose属性设置为 true,关闭GLFW。下一次while循环的条件检测将会失败，程序将会关闭。
void processInput(GLFWwindow* window);


void mouse_callback(GLFWwindow* window, double xpos, double ypos);


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


int main()
{
    //初始化GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);





    //创建一个窗口对象,窗口对象而且会被GLFW的其他函数频繁地用到
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    //通知GLFW将我们窗口的上下文设置为当前线程的主上下文了
    glfwMakeContextCurrent(window);


    //首先我们要告诉GLFW，它应该隐藏光标，并捕捉(Capture)它
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    //告诉GLFW我们希望每当窗口调整大小的时候调用这个函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    //当我们用GLFW注册了回调函数之后，鼠标一移动mouse_callback函数就会被调用：
    glfwSetCursorPosCallback(window, mouse_callback);

    //注册鼠标滚轮的回调函数：
    glfwSetScrollCallback(window, scroll_callback);


    //GLAD是用来管理OpenGL的函数指针的,所以在调用任何OpenGL的函数之前我们需要初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }







    //生成一个VAO对象
    unsigned int objVAO, VBO;
    glGenVertexArrays(1, &objVAO);
    glGenBuffers(1, &VBO);


    //绑定objVAO
    glBindVertexArray(objVAO);

    //把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上：
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //把之前定义的顶点数据复制到缓冲的内存中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);




    //告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）
    //第一个0就是着色器里的layout (location = 0)个属性位置值

    //位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //法线属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);



    ///////////////////lightVAO///////////////////
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    //把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上：
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //把之前定义的顶点数据复制到缓冲的内存中(已经放入过了，直接绑定就行)
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);





    //用线框模式绘制你的三角形
 /*   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/




    Shader objShader("ShaderPath/obj.vs", "ShaderPath/obj.fs");
    Shader lightshader("ShaderPath/light.vs", "ShaderPath/light.fs");

    objShader.use();
    // light properties
    objShader.setVec3("light.ambient", glm::vec3(1.0f, 1.0f, 1.0f)); // note that all light colors are set at full intensity
    objShader.setVec3("light.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
    objShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    // material properties
    objShader.setVec3("material.ambient", glm::vec3(0.0f, 0.1f, 0.06f));
    objShader.setVec3("material.diffuse", glm::vec3(0.0f, 0.50980392f, 0.50980392f));
    objShader.setVec3("material.specular", glm::vec3(0.50196078f, 0.50196078f, 0.50196078f));
    objShader.setFloat("material.shininess", 32.0f);


    //渲染循环
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //在每个新的渲染迭代开始的时候我们总是希望清屏
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        //启用深度测试
        glEnable(GL_DEPTH_TEST);

        /*
        因为我们使用了深度测试，我们也想要在每次渲染迭代之前清除深度缓冲（否则前一帧的深度信息仍然保存在缓冲中）。
        就像清除颜色缓冲一样，我们可以通过在glClear函数中指定DEPTH_BUFFER_BIT位来清除深度缓冲：
        */

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //lightPos.x = cos(glfwGetTime()) * 3.0f;
        //lightPos.z = sin(glfwGetTime()) * 3.0f;

        objShader.use();// 不要忘记在设置uniform变量之前激活着色器程序！

        objShader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
        objShader.setVec3("lightPos", lightPos);
        objShader.setVec3("viewPos", camera.Position);


        //glm::vec3 lightColor;
        //lightColor.x = sin(glfwGetTime() * 0.5f);
        //lightColor.y = sin(glfwGetTime() * 0.7f);
        //lightColor.z = sin(glfwGetTime() * 1.3f);

        //glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // 降低影响
        //glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // 很低的影响

        //objShader.setVec3("light.ambient", ambientColor);
        //objShader.setVec3("light.diffuse", diffuseColor);




        //   //首先创建一个模型矩阵
        glm::mat4 model(1.0f);
        // model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 1.0f, 0.0f));

         //其实就是通过一个lookAt矩阵实现了（对世界坐标系中物体）平移和旋转两个矩阵的功能
        glm::mat4 view = camera.GetViewMatrix();

        //定义一个投影矩阵。我们希望在场景中使用透视投影，所以像这样声明一个投影矩阵：
        glm::mat4 projection(1.0f);
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


        //传入顶点着色器
        objShader.setMat4("model", model);
        objShader.setMat4("view", view);
        objShader.setMat4("projection", projection);

        // 4. 绘制物体
        glBindVertexArray(objVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);



        lightshader.use();

        //   //首先创建一个模型矩阵
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos) * glm::scale(model, glm::vec3(0.2f));

        //传入顶点着色器
       // lightshader.setVec3("lightcolor", lightColor);
        lightshader.setMat4("model", model);
        lightshader.setMat4("view", view);
        lightshader.setMat4("projection", projection);

        // 4. 绘制物体
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        glfwPollEvents();
        glfwSwapBuffers(window);

    }

    glDeleteVertexArrays(1, &objVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);




    //当渲染循环结束后我们需要正确释放/删除之前的分配的所有资源
    glfwTerminate();
    return 0;
}




//当用户改变窗口的大小的时候，视口也应该被调整
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //告诉OpenGL渲染窗口的尺寸大小
    glViewport(0, 0, width, height);
    std::cout << "change window size" << std::endl;
}


//这里我们检查用户是否按下了返回键(Esc),如果用户的确按下了返回键,
//把WindowShouldClose属性设置为 true,关闭GLFW。下一次while循环的条件检测将会失败，程序将会关闭。
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

    if (firstMouse) // 这个bool变量初始时是设定为true的
    {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMouse = false;
    }

    //偏移量只有单位1
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}