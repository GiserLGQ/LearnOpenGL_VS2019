#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include"stb_image.h"

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
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f,  0.2f,  2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3(0.0f,  0.0f, -3.0f)
};



//当用户改变窗口的大小的时候，视口也应该被调整
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//这里我们检查用户是否按下了返回键(Esc),如果用户的确按下了返回键,
//把WindowShouldClose属性设置为 true,关闭GLFW。下一次while循环的条件检测将会失败，程序将会关闭。
void processInput(GLFWwindow* window);


void mouse_callback(GLFWwindow* window, double xpos, double ypos);


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


unsigned int loadTexture(char const* path);//加载纹理


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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //法线属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //纹理坐标属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);



    ///////////////////lightVAO///////////////////
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    //把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上：
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //把之前定义的顶点数据复制到缓冲的内存中(已经放入过了，直接绑定就行)
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);








    //用线框模式绘制你的三角形
 /*   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/


    Shader objShader("ShaderPath/obj.vs", "ShaderPath/obj.fs");
    Shader lightshader("ShaderPath/light.vs", "ShaderPath/light.fs");


    objShader.use();
    //生成纹理对象
    unsigned int diffuseMap = loadTexture("textures/container2.png");
    //设置每个采样器的方式告诉OpenGL每个着色器采样器属于哪个纹理单元。我们只需要设置一次即可
      //将要用的纹理单元赋值到material.diffuse这个uniform采样器，并绑定箱子的纹理到这个纹理单元：
    objShader.setInt("material.diffuse", 0); // 或者使用着色器类设置


        //生成纹理对象
    unsigned int specularMap = loadTexture("textures/container2_specular.png");
    //设置每个采样器的方式告诉OpenGL每个着色器采样器属于哪个纹理单元。我们只需要设置一次即可
      //将要用的纹理单元赋值到material.diffuse这个uniform采样器，并绑定箱子的纹理到这个纹理单元：
    objShader.setInt("material.specular", 1); // 或者使用着色器类设置




    





    //渲染循环
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        float currentFrame = static_cast<float>(glfwGetTime());
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





        objShader.use();// 不要忘记在设置uniform变量之前激活着色器程序！
        objShader.setVec3("viewPos", camera.Position);
        objShader.setFloat("material.shininess", 32.0f);




       ////定向光参数
        objShader.setVec3("dirLight.direction", 0.f, 0.f, -3.0f);
        objShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        objShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        objShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // 点光源1
        objShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        objShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        objShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        objShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        objShader.setFloat("pointLights[0].constant", 1.0f);
        objShader.setFloat("pointLights[0].linear", 0.09f);
        objShader.setFloat("pointLights[0].quadratic", 0.032f);
        // 点光源 2
        objShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        objShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        objShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        objShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        objShader.setFloat("pointLights[1].constant", 1.0f);
        objShader.setFloat("pointLights[1].linear", 0.09f);
        objShader.setFloat("pointLights[1].quadratic", 0.032f);
        // 点光源 3
        objShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        objShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        objShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        objShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        objShader.setFloat("pointLights[2].constant", 1.0f);
        objShader.setFloat("pointLights[2].linear", 0.09f);
        objShader.setFloat("pointLights[2].quadratic", 0.032f);
        // 点光源 4
        objShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        objShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        objShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        objShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        objShader.setFloat("pointLights[3].constant", 1.0f);
        objShader.setFloat("pointLights[3].linear", 0.09f);
        objShader.setFloat("pointLights[3].quadratic", 0.032f);
        // 聚光灯
        objShader.setVec3("spotLight.position", camera.Position);
        objShader.setVec3("spotLight.direction", camera.Front);
        objShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        objShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        objShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        objShader.setFloat("spotLight.constant", 1.0f);
        objShader.setFloat("spotLight.linear", 0.09f);
        objShader.setFloat("spotLight.quadratic", 0.032f);
        objShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        objShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));



        //其实就是通过一个lookAt矩阵实现了（对世界坐标系中物体）平移和旋转两个矩阵的功能
        glm::mat4 view = camera.GetViewMatrix();
        //定义一个投影矩阵。我们希望在场景中使用透视投影，所以像这样声明一个投影矩阵：
        glm::mat4 projection(1.0f);
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        objShader.setMat4("view", view);
        objShader.setMat4("projection", projection);




        //需要绑定纹理对象到对应的纹理单元
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        //需要绑定纹理对象到对应的纹理单元
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        // 4. 绘制物体
        glBindVertexArray(objVAO);
        for (int i = 0; i < sizeof(cubePositions) / sizeof(cubePositions[0]); i++)
        {
            //   //首先创建一个模型矩阵
            glm::mat4 model(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(i, 1.0f, 0.0f));
            objShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }







        //光源着色器
        lightshader.use();
        lightshader.setMat4("view", view);
        lightshader.setMat4("projection", projection);


        // 4. 绘制物体
        glBindVertexArray(lightVAO);

        for (unsigned int i = 0; i < 4; i++)
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            lightshader.setMat4("model", model);
            //lightshader.setVec3("lightcolor", lightColor);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


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


unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}