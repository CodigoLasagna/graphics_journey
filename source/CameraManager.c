
#include "CameraManager.h"


void SetupBasicCamera(Camera *camera, int window_width, int window_height, float mouseSensitivity)
{
	glm_vec3_copy((vec3){0.0f, 0.0f, 3.0f}, camera->Position);
	glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, camera->Up);
	glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, camera->WorldUp);
	glm_vec3_copy((vec3){0.0f, 0.0f, -1.0f}, camera->Front);

	camera->MouseSensitivity = mouseSensitivity;

	camera->deltaTime = 0.0f;
	camera->lastFrame = 0.0f;

	camera->mouseLastX = (int) (window_width / 2);
	camera->mouseLastY = (int) (window_height / 2);

	camera->yaw = -90;
	camera->pitch = 0;
	camera->fov = 90;
	camera->firstMouse = true;
	UpdateCameraVectors(camera);
}

void ProcessMouseMovement(Camera *camera, float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= camera->MouseSensitivity;
	yoffset *= camera->MouseSensitivity;

	camera->yaw += xoffset;
	camera->pitch += yoffset;

	if (constrainPitch)
	{
		if (camera->pitch > 89.0f)
		{
			camera->pitch = 89.0f;
		}
		if (camera->pitch < -89.0f)
		{
			camera->pitch = -89.0f;
		}
	}

	UpdateCameraVectors(camera);
}

void UpdateCameraVectors(Camera *camera)
{
	vec3 front;
	vec3 temp;
	front[0] = cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
	front[1] = sin(glm_rad(camera->pitch));
	front[2] = sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));

	glm_normalize_to(front, camera->Front);

	glm_cross(camera->Front, camera->WorldUp, temp);
	glm_normalize_to(temp, camera->Right);
	glm_cross(camera->Right, camera->Front, temp);
	glm_normalize_to(temp, camera->Up);
}


void GetViewMatrix(Camera *camera, mat4 view)
{
	vec3 center;
	glm_vec3_add(camera->Position, camera->Front, center);
	glm_lookat(camera->Position, center, camera->Up, view);
}


void UpdateViewMatrix(Camera *camera)
{
	vec3 center;
	glm_vec3_add(camera->Position, camera->Front, center);
	glm_lookat(camera->Position, center, camera->Up, camera->view);
}

