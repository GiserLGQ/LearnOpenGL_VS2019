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



//���û��ı䴰�ڵĴ�С��ʱ���ӿ�ҲӦ�ñ�����
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//�������Ǽ���û��Ƿ����˷��ؼ�(Esc),����û���ȷ�����˷��ؼ�,
//��WindowShouldClose��������Ϊ true,�ر�GLFW����һ��whileѭ����������⽫��ʧ�ܣ����򽫻�رա�
void processInput(GLFWwindow* window);


void mouse_callback(GLFWwindow* window, double xpos, double ypos);


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


unsigned int loadTexture(char const* path);//��������


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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //��������
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //������������
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);



    ///////////////////lightVAO///////////////////
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    //���´����Ļ���󶨵�GL_ARRAY_BUFFERĿ���ϣ�
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //��֮ǰ����Ķ������ݸ��Ƶ�������ڴ���(�Ѿ�������ˣ�ֱ�Ӱ󶨾���)
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //λ������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);








    //���߿�ģʽ�������������
 /*   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/


    Shader objShader("ShaderPath/obj.vs", "ShaderPath/obj.fs");
    Shader lightshader("ShaderPath/light.vs", "ShaderPath/light.fs");


    objShader.use();
    //�����������
    unsigned int diffuseMap = loadTexture("textures/container2.png");
    //����ÿ���������ķ�ʽ����OpenGLÿ����ɫ�������������ĸ�����Ԫ������ֻ��Ҫ����һ�μ���
      //��Ҫ�õ�����Ԫ��ֵ��material.diffuse���uniform���������������ӵ������������Ԫ��
    objShader.setInt("material.diffuse", 0); // ����ʹ����ɫ��������


        //�����������
    unsigned int specularMap = loadTexture("textures/container2_specular.png");
    //����ÿ���������ķ�ʽ����OpenGLÿ����ɫ�������������ĸ�����Ԫ������ֻ��Ҫ����һ�μ���
      //��Ҫ�õ�����Ԫ��ֵ��material.diffuse���uniform���������������ӵ������������Ԫ��
    objShader.setInt("material.specular", 1); // ����ʹ����ɫ��������




    





    //��Ⱦѭ��
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        float currentFrame = static_cast<float>(glfwGetTime());
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





        objShader.use();// ��Ҫ����������uniform����֮ǰ������ɫ������
        objShader.setVec3("viewPos", camera.Position);
        objShader.setFloat("material.shininess", 32.0f);




       ////��������
        objShader.setVec3("dirLight.direction", 0.f, 0.f, -3.0f);
        objShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        objShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        objShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // ���Դ1
        objShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        objShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        objShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        objShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        objShader.setFloat("pointLights[0].constant", 1.0f);
        objShader.setFloat("pointLights[0].linear", 0.09f);
        objShader.setFloat("pointLights[0].quadratic", 0.032f);
        // ���Դ 2
        objShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        objShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        objShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        objShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        objShader.setFloat("pointLights[1].constant", 1.0f);
        objShader.setFloat("pointLights[1].linear", 0.09f);
        objShader.setFloat("pointLights[1].quadratic", 0.032f);
        // ���Դ 3
        objShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        objShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        objShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        objShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        objShader.setFloat("pointLights[2].constant", 1.0f);
        objShader.setFloat("pointLights[2].linear", 0.09f);
        objShader.setFloat("pointLights[2].quadratic", 0.032f);
        // ���Դ 4
        objShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        objShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        objShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        objShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        objShader.setFloat("pointLights[3].constant", 1.0f);
        objShader.setFloat("pointLights[3].linear", 0.09f);
        objShader.setFloat("pointLights[3].quadratic", 0.032f);
        // �۹��
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



        //��ʵ����ͨ��һ��lookAt����ʵ���ˣ�����������ϵ�����壩ƽ�ƺ���ת��������Ĺ���
        glm::mat4 view = camera.GetViewMatrix();
        //����һ��ͶӰ��������ϣ���ڳ�����ʹ��͸��ͶӰ����������������һ��ͶӰ����
        glm::mat4 projection(1.0f);
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        objShader.setMat4("view", view);
        objShader.setMat4("projection", projection);




        //��Ҫ��������󵽶�Ӧ������Ԫ
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        //��Ҫ��������󵽶�Ӧ������Ԫ
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        // 4. ��������
        glBindVertexArray(objVAO);
        for (int i = 0; i < sizeof(cubePositions) / sizeof(cubePositions[0]); i++)
        {
            //   //���ȴ���һ��ģ�;���
            glm::mat4 model(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(i, 1.0f, 0.0f));
            objShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }







        //��Դ��ɫ��
        lightshader.use();
        lightshader.setMat4("view", view);
        lightshader.setMat4("projection", projection);


        // 4. ��������
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