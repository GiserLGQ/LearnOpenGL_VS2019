#include <glad/glad.h> 
#include <GLFW\glfw3.h>

#include<iostream>


//ָ���������㣬ÿ�����㶼��һ��3Dλ��
float vertices[] = {
    0.5f, 0.5f, 0.0f,   // ���Ͻ�
    0.5f, -0.5f, 0.0f,  // ���½�
    -0.5f, -0.5f, 0.0f, // ���½�
    -0.5f, 0.5f, 0.0f   // ���Ͻ�
};




//������ʱ��������ɫ����Դ����Ӳ�����ڴ����ļ�������C����ַ����У�
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";


const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
"}\0";


//���û��ı䴰�ڵĴ�С��ʱ���ӿ�ҲӦ�ñ�����
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //����OpenGL��Ⱦ���ڵĳߴ��С
    glViewport(0, 0, width, height);
}


//�������Ǽ���û��Ƿ����˷��ؼ�(Esc),����û���ȷ�����˷��ؼ�,
//��WindowShouldClose��������Ϊ true,�ر�GLFW����һ��whileѭ����������⽫��ʧ�ܣ����򽫻�رա�
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}



int main()
{
    //��ʼ��GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);




    //����һ�����ڶ���,���ڶ�����һᱻGLFW����������Ƶ�����õ�
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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


    //����һ��VAO����
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    //ID����һ��VBO����
    unsigned int VBO;
    glGenBuffers(1, &VBO);






    //����һ��������ɫ��
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    //�������ɫ��Դ�븽�ӵ���ɫ�������ϣ�Ȼ���������
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //����ڵ���glCompileShader������Ƿ�ɹ���
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }



    //����������Ƭ����ɫ���������붥����ɫ������
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);


    //������ɫ�����ڶ������ˣ�ʣ�µ������ǰ�������ɫ���������ӵ�һ��������Ⱦ����ɫ������(Shader Program)�С�

    //������ɫ���������
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    //��֮ǰ����Ķ��㡢Ƭ����ɫ�����ӵ���������ϣ�Ȼ����glLinkProgram�������ǣ�
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //����ڵ���glLinkProgram������Ƿ�ɹ���
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::shaderProgram::LINK_FAILED\n" << infoLog << std::endl;
    }

    //����ɫ���������ӵ���������Ժ󣬼ǵ�ɾ����ɫ���������ǲ�����Ҫ�����ˣ�
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);








    //��VAO
    glBindVertexArray(VAO);


    //���´����Ļ���󶨵�GL_ARRAY_BUFFERĿ���ϣ�
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //��֮ǰ����Ķ������ݸ��Ƶ�������ڴ���
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    //����OpenGL����ν����������ݣ�Ӧ�õ�������������ϣ�
    //��һ��0������ɫ�����layout (location = 0)������λ��ֵ
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);





    //���߿�ģʽ�������������
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);





    //��Ⱦѭ��
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);


        //��ÿ���µ���Ⱦ������ʼ��ʱ����������ϣ������
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        glUseProgram(shaderProgram);


        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        

        // 4. ��������
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);//���VAO

   

        glfwPollEvents();
        glfwSwapBuffers(window);
    
    }




    //����Ⱦѭ��������������Ҫ��ȷ�ͷ�/ɾ��֮ǰ�ķ����������Դ
    glfwTerminate();
    return 0;
}