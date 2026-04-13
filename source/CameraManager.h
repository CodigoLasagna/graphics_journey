#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>

enum CameraMovement
{
	CAM_FORWARD,
	CAM_BACKWARD,
	CAM_LEFT,
	CAM_RIGHT,
	CAM_UP,
	CAM_DOWN
};

typedef struct Camera
{
	vec3 Position;
	vec3 Front;
	vec3 Up;
	vec3 Right;
	vec3 WorldUp;
	vec3 cameraDir;
	mat4 projection;
	mat4 view;
	float deltaTime;
	float lastFrame;
	float currentFrame;

	float mouseLastX;
	float mouseLastY;
	float yaw;
	float pitch;
	float fov;
	bool firstMouse;

	float MovementSpeed;
	float MouseSensitivity;
}Camera;

void SetupCamera(Camera *camera, vec3 position, vec3 up, vec3 front,float yaw, float pitch);
void SetupBasicCamera(Camera *camera, int window_width, int window_height, float mouseSensitivity, float movementSpeed);
void UpdateCameraVectors(Camera *camera);

void ProcessMouseMovement(Camera *camera, float xoffset, float yoffset, bool constrainPitch);
void ProcessMouseScroll(Camera *camera, float yoffset);
void ProcessKeyboard(Camera *camera, enum CameraMovement direction);


void GetViewMatrix(Camera *camera, mat4 view);
void UpdateViewMatrix(Camera *camera);

#endif
