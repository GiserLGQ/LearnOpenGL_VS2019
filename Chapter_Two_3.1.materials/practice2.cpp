
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



// ����camera
//Camera camera(glm::vec3(2.0f, 0.0f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f),
//    120, 10);
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

//���Ǹ�������ȫ�ֱ����������deltaTimeֵ��
float deltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
float lastFrame = 0.0f; // ��һ֡��ʱ��

//���Ǳ������ڳ����д�����һ֡�����λ��
float lastX = SCR_WIDTH / 2.0f, lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//��Դ�ڳ���������ռ������е�λ��
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

//������cube����
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




//���û��ı䴰�ڵĴ�С��ʱ���ӿ�ҲӦ�ñ�����
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//�������Ǽ���û��Ƿ����˷��ؼ�(Esc),����û���ȷ�����˷��ؼ�,
//��WindowShouldClose��������Ϊ true,�ر�GLFW����һ��whileѭ����������⽫��ʧ�ܣ����򽫻�رա�
void processInput(GLFWwindow* window);


void mouse_callback(GLFWwindow* window, double xpos, double ypos);


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


int main()
{
    //��ʼ��GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);





    //����һ�����ڶ���,���ڶ�����һᱻGLFW����������Ƶ�����õ�
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    //֪ͨGLFW�����Ǵ��ڵ�����������Ϊ��ǰ�̵߳�����������
    glfwMakeContextCurrent(window);


    //��������Ҫ����GLFW����Ӧ�����ع�꣬����׽(Capture)��
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    //����GLFW����ϣ��ÿ�����ڵ�����С��ʱ������������
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    //��������GLFWע���˻ص�����֮�����һ�ƶ�mouse_callback�����ͻᱻ���ã�
    glfwSetCursorPosCallback(window, mouse_callback);

    //ע�������ֵĻص�������
    glfwSetScrollCallback(window, scroll_callback);


    //GLAD����������OpenGL�ĺ���ָ���,�����ڵ����κ�OpenGL�ĺ���֮ǰ������Ҫ��ʼ��GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }







    //����һ��VAO����
    unsigned int objVAO, VBO;
    glGenVertexArrays(1, &objVAO);
    glGenBuffers(1, &VBO);


    //��objVAO
    glBindVertexArray(objVAO);

    //���´����Ļ���󶨵�GL_ARRAY_BUFFERĿ���ϣ�
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //��֮ǰ����Ķ������ݸ��Ƶ�������ڴ���
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);




    //����OpenGL����ν����������ݣ�Ӧ�õ�������������ϣ�
    //��һ��0������ɫ�����layout (location = 0)������λ��ֵ

    //λ������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //��������
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);



    ///////////////////lightVAO///////////////////
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    //���´����Ļ���󶨵�GL_ARRAY_BUFFERĿ���ϣ�
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //��֮ǰ����Ķ������ݸ��Ƶ�������ڴ���(�Ѿ�������ˣ�ֱ�Ӱ󶨾���)
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //λ������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);





    //���߿�ģʽ�������������
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


    //��Ⱦѭ��
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //��ÿ���µ���Ⱦ������ʼ��ʱ����������ϣ������
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        //������Ȳ���
        glEnable(GL_DEPTH_TEST);

        /*
        ��Ϊ����ʹ������Ȳ��ԣ�����Ҳ��Ҫ��ÿ����Ⱦ����֮ǰ�����Ȼ��壨����ǰһ֡�������Ϣ��Ȼ�����ڻ����У���
        ���������ɫ����һ�������ǿ���ͨ����glClear������ָ��DEPTH_BUFFER_BITλ�������Ȼ��壺
        */

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //lightPos.x = cos(glfwGetTime()) * 3.0f;
        //lightPos.z = sin(glfwGetTime()) * 3.0f;

        objShader.use();// ��Ҫ����������uniform����֮ǰ������ɫ������

        objShader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
        objShader.setVec3("lightPos", lightPos);
        objShader.setVec3("viewPos", camera.Position);


        //glm::vec3 lightColor;
        //lightColor.x = sin(glfwGetTime() * 0.5f);
        //lightColor.y = sin(glfwGetTime() * 0.7f);
        //lightColor.z = sin(glfwGetTime() * 1.3f);

        //glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // ����Ӱ��
        //glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // �ܵ͵�Ӱ��

        //objShader.setVec3("light.ambient", ambientColor);
        //objShader.setVec3("light.diffuse", diffuseColor);




        //   //���ȴ���һ��ģ�;���
        glm::mat4 model(1.0f);
        // model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 1.0f, 0.0f));

         //��ʵ����ͨ��һ��lookAt����ʵ���ˣ�����������ϵ�����壩ƽ�ƺ���ת��������Ĺ���
        glm::mat4 view = camera.GetViewMatrix();

        //����һ��ͶӰ��������ϣ���ڳ�����ʹ��͸��ͶӰ����������������һ��ͶӰ����
        glm::mat4 projection(1.0f);
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


        //���붥����ɫ��
        objShader.setMat4("model", model);
        objShader.setMat4("view", view);
        objShader.setMat4("projection", projection);

        // 4. ��������
        glBindVertexArray(objVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);



        lightshader.use();

        //   //���ȴ���һ��ģ�;���
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos) * glm::scale(model, glm::vec3(0.2f));

        //���붥����ɫ��
       // lightshader.setVec3("lightcolor", lightColor);
        lightshader.setMat4("model", model);
        lightshader.setMat4("view", view);
        lightshader.setMat4("projection", projection);

        // 4. ��������
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        glfwPollEvents();
        glfwSwapBuffers(window);

    }

    glDeleteVertexArrays(1, &objVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);




    //����Ⱦѭ��������������Ҫ��ȷ�ͷ�/ɾ��֮ǰ�ķ����������Դ
    glfwTerminate();
    return 0;
}




//���û��ı䴰�ڵĴ�С��ʱ���ӿ�ҲӦ�ñ�����
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //����OpenGL��Ⱦ���ڵĳߴ��С
    glViewport(0, 0, width, height);
    std::cout << "change window size" << std::endl;
}


//�������Ǽ���û��Ƿ����˷��ؼ�(Esc),����û���ȷ�����˷��ؼ�,
//��WindowShouldClose��������Ϊ true,�ر�GLFW����һ��whileѭ����������⽫��ʧ�ܣ����򽫻�رա�
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

    if (firstMouse) // ���bool������ʼʱ���趨Ϊtrue��
    {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMouse = false;
    }

    //ƫ����ֻ�е�λ1
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // ע���������෴�ģ���Ϊy�����Ǵӵײ����������������

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}