#pragma once

#include <DirectXMath.h>

#include "transform.h"
#include <memory>


class Camera
{
	
public:
		Camera(
			DirectX::XMFLOAT3 position,
			float fieldOfView,
			float aspectRatio,
			float nearClip = 0.01f,
			float farClip = 100.0f,
			float movementSpeed = 1.0f,
			float mouseLookSpeed = 0.002f);

		~Camera();

		// Updating methods
		void Update(float dt);
		void UpdateViewMatrix();
		void UpdateProjectionMatrix(float aspectRatio);

		// Getters
		DirectX::XMFLOAT4X4 GetViewMatrix();
		DirectX::XMFLOAT4X4 GetProjectionMatrix();
		std::shared_ptr<transform> GetTransform();
		float GetAspectRatio();

		float GetFieldOfView();
		void SetFieldOfView(float fov);

		float GetNearClip();
		void SetNearClip(float distance);

		float GetFarClip();
		void SetFarClip(float distance);

		float GetOrthographicWidth();
		void SetOrthographicWidth(float width);


private:
		// Camera matrices
		DirectX::XMFLOAT4X4 viewMatrix;
		DirectX::XMFLOAT4X4 projMatrix;

		std::shared_ptr<transform> Transform;

		float fieldOfView;
		float aspectRatio;
		float nearClip;
		float farClip;
		float movementSpeed;
		float mouseLookSpeed;
};


