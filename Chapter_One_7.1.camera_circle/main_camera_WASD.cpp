#include <glad/glad.h> 
#include <GLFW\glfw3.h>

#include"Shader.h"
#include"stb_image.h"
#include<iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//���Ǹ�������ȫ�ֱ����������deltaTimeֵ��
float deltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
float lastFrame = 0.0f; // ��һ֡��ʱ��



float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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


unsigned int indices[] = {
    // ע��������0��ʼ! 
    // ����������(0,1,2,3)���Ƕ�������vertices���±꣬
    // �����������±��������ϳɾ���

    0, 1, 3, // ��һ��������
    1, 2, 3  // �ڶ���������
};

//���������
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);






//���û��ı䴰�ڵĴ�С��ʱ���ӿ�ҲӦ�ñ�����
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//�������Ǽ���û��Ƿ����˷��ؼ�(Esc),����û���ȷ�����˷��ؼ�,
//��WindowShouldClose��������Ϊ true,�ر�GLFW����һ��whileѭ����������⽫��ʧ�ܣ����򽫻�رա�
void processInput(GLFWwindow* window);



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


    //����GLFW����ϣ��ÿ�����ڵ�����С��ʱ������������
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);




    //GLAD����������OpenGL�ĺ���ָ���,�����ڵ����κ�OpenGL�ĺ���֮ǰ������Ҫ��ʼ��GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }







    unsigned int texture1, texture2;
    ///////////////texture1/////////////
    //����һ������Ĺ���
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    // ���ز���������
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);//��תy��
    unsigned char* data = stbi_load("textures/5x01891.JPG", &width, &height, &nrChannels, 0);
    if (data)
    {
        //������glTexImage2Dʱ����ǰ�󶨵��������ͻᱻ����������ͼ��
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

    // Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // ���ز���������
    int width1, height1, nrChannels1;
    stbi_set_flip_vertically_on_load(true);//��תy��
    unsigned char* data1 = stbi_load("textures/awesomeface.png", &width1, &height1, &nrChannels1, 0);
    if (data1)
    {
        //������glTexImage2Dʱ����ǰ�󶨵��������ͻᱻ����������ͼ��
        //��ע�⣬Awesomeface.png����͸���ȣ���˾���alphaͨ���������ȷ������OpenGL��������ΪGL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width1, height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data1);









    //����һ��VAO����
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    //ID����һ��VBO����
    unsigned int VBO;
    glGenBuffers(1, &VBO);


    //����һ��EBO����
    unsigned int EBO;
    glGenBuffers(1, &EBO);





    //��VAO
    glBindVertexArray(VAO);

    //���´����Ļ���󶨵�GL_ARRAY_BUFFERĿ���ϣ�
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //��֮ǰ����Ķ������ݸ��Ƶ�������ڴ���
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    //��EBO�����������ݸ��Ƶ�������ڴ���
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



    //����OpenGL����ν����������ݣ�Ӧ�õ�������������ϣ�
    //��һ��0������ɫ�����layout (location = 0)������λ��ֵ

    //λ������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ��ɫ����
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);





    //���߿�ģʽ�������������
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



    Shader myshader("ShaderPath/shader.vs", "ShaderPath/shader.fs");

    //����ÿ���������ķ�ʽ����OpenGLÿ����ɫ�������������ĸ�����Ԫ������ֻ��Ҫ����һ�μ���
    myshader.use();// ��Ҫ����������uniform����֮ǰ������ɫ������
    glUniform1i(glGetUniformLocation(myshader.ID, "texture1"), 0); // �ֶ�����
    myshader.setInt("texture2", 1); // ����ʹ����ɫ��������








    //��Ⱦѭ��
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //��ÿ���µ���Ⱦ������ʼ��ʱ����������ϣ������
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        //������Ȳ���
        glEnable(GL_DEPTH_TEST);

        /*
        ��Ϊ����ʹ������Ȳ��ԣ�����Ҳ��Ҫ��ÿ����Ⱦ����֮ǰ�����Ȼ��壨����ǰһ֡�������Ϣ��Ȼ�����ڻ����У���
        ���������ɫ����һ�������ǿ���ͨ����glClear������ָ��DEPTH_BUFFER_BITλ�������Ȼ��壺
        */

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //��Ҫ��������󵽶�Ӧ������Ԫ
        glActiveTexture(GL_TEXTURE0); // �ڰ�����֮ǰ�ȼ�������Ԫ
        glBindTexture(GL_TEXTURE_2D, texture1);

        glActiveTexture(GL_TEXTURE1); // �ڰ�����֮ǰ�ȼ�������Ԫ
        glBindTexture(GL_TEXTURE_2D, texture2);


        myshader.use();// ��Ҫ����������uniform����֮ǰ������ɫ������


        //   //���ȴ���һ��ģ�;���
        //glm::mat4 model(1.0f);
        //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 1.0f, 0.0f));

        //����һ���۲����,�۲�������õ�����������ռ�
        // ע�⣬���ǽ�����������Ҫ�����ƶ������ķ������ƶ���
        glm::mat4 view(1.0f);
        //��ʵ����ͨ��һ��lookAt����ʵ���ˣ�����������ϵ�����壩ƽ�ƺ���ת��������Ĺ���
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);



        //����һ��ͶӰ��������ϣ���ڳ�����ʹ��͸��ͶӰ����������������һ��ͶӰ����
        glm::mat4 projection(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);



        //���붥����ɫ��
        //myshader.setMat4("model", model);
        myshader.setMat4("view", view);
        myshader.setMat4("projection", projection);





        // 4. ��������
        glBindVertexArray(VAO);

        for (int i = 0; i < 10; i++)
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, cubePositions[i]);
            if ((i % 3 == 2) || (i == 0))
            {
                model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 1.0f, float(i)));
            }

            myshader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }





        glfwPollEvents();
        glfwSwapBuffers(window);

    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);



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
    float cameraSpeed = 5.0f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}