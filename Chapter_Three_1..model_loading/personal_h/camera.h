#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <vector>


//�������ö��ֵ
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// һЩĬ��ֵ
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 12.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


class Camera
{
public:

    // �������
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;

    glm::vec3 WorldUp;

    // ŷ����
    float Yaw;
    float Pitch;

    // ���ѡ��
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    //�����ʼ��
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
        float yaw = YAW, float pitch = PITCH)
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)),MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
	}

    glm::mat4 MyLookAt(const glm::vec3 & eyepos, const glm::vec3& center, const glm::vec3& up)
    {
        glm::mat4 posmat(1.0f);
        posmat = glm::translate(posmat, -eyepos);

        glm::vec3 cameraDirection = glm::normalize(eyepos - center);
        glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
        glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);


        glm::mat4 rotation = glm::mat4(1.0f);
        rotation[0][0] = cameraRight.x; // First column, first row
        rotation[1][0] = cameraRight.y;
        rotation[2][0] = cameraRight.z;
        rotation[0][1] = cameraUp.x; // First column, second row
        rotation[1][1] = cameraUp.y;
        rotation[2][1] = cameraUp.z;
        rotation[0][2] = cameraDirection.x; // First column, third row
        rotation[1][2] = cameraDirection.y;
        rotation[2][2] = cameraDirection.z;

        // Return lookAt matrix as combination of translation and rotation matrix
        return rotation * posmat; // Remember to read from right to left (first translation then rotation)
    }

    glm::mat4 GetViewMatrix()
    {
        return MyLookAt(Position, Position + Front, Up);
    }

    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        //����
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += velocity * Front;
        if(direction== BACKWARD)
            Position -= velocity * Front;
        if(direction== LEFT)
            Position -= velocity * Right;
        if (direction == RIGHT)
            Position += velocity * Right;
    }
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }


private:
    //���һ���ƶ����͸����������̬����
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }
};


#endif