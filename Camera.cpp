#include "Camera.h"
#include "Input.h"
#include <algorithm>

Camera::Camera(DirectX::XMFLOAT3 position, 
	float fieldOfView, 
	float aspectRatio, 
	float nearClip, 
	float farClip,
	float movementSpeed,
	float mouseLookSpeed) :
	fieldOfView(fieldOfView),
	aspectRatio(aspectRatio),
	nearClip(nearClip),
	farClip(farClip),
	movementSpeed(movementSpeed),
	mouseLookSpeed(mouseLookSpeed)
{
	Transform = std::make_shared<transform>();
	Transform->SetPosition(position);

	UpdateViewMatrix();
	UpdateProjectionMatrix(aspectRatio);

}

Camera::~Camera()
{
}

void Camera::Update(float dt)
{
	float speed = movementSpeed * dt;

	if (Input::KeyDown('W')) { Transform->MoveRelative(DirectX::XMFLOAT3(0.0f, 0.0f, speed)); }
	if (Input::KeyDown('S')) { Transform->MoveRelative(DirectX::XMFLOAT3(0.0f, 0.0f, -speed)); }

	if (Input::KeyDown('A')) { Transform->MoveRelative(DirectX::XMFLOAT3(-speed, 0.0f, 0.0f)); }
	if (Input::KeyDown('D')) { Transform->MoveRelative(DirectX::XMFLOAT3(speed, 0.0f, 0.0f)); }

	if (Input::KeyDown('X')) { Transform->MoveAbsolute(DirectX::XMFLOAT3(0.0f, -speed, 0.0f)); }
	if (Input::KeyDown(' ')) { Transform->MoveAbsolute(DirectX::XMFLOAT3(0.0f, speed, 0.0f)); }

	if (Input::MouseRightDown())
	{
		float dx = Input::GetMouseXDelta() * mouseLookSpeed;
		float dy = Input::GetMouseYDelta() * mouseLookSpeed;

		

		// Calculate new pitch
		float newPitch = Transform->getPitchYawRoll().x + dy;

		//stop the gimbal lock that happens exactly at 90 degrees
		float limit = DirectX::XM_PIDIV2-0.001f;

		if (newPitch > limit) {
			newPitch = limit;
		}
			
		if (newPitch < -limit) {
			newPitch = -limit;
		}
			

		// Only rotate by the amount that they moved
		float pitchChange = newPitch - Transform->getPitchYawRoll().x;


		Transform->Rotate(pitchChange, dx, 0.0f);



	}

	// this last do input before this
	UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
	DirectX::XMFLOAT3 fwd = Transform->GetForward();
	DirectX::XMFLOAT3 pos = Transform->getPosition();

	// Make the view matrix and save
	DirectX::XMMATRIX view = DirectX::XMMatrixLookToLH(
		DirectX::XMLoadFloat3(&pos),
		DirectX::XMLoadFloat3(&fwd), 
		DirectX::XMVectorSet(0, 1, 0, 0)); // World up axis
	DirectX::XMStoreFloat4x4(&viewMatrix, view);
}

void Camera::UpdateProjectionMatrix(float aspectRatio)
{
	this->aspectRatio = aspectRatio;

	DirectX::XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(
			fieldOfView,		
			aspectRatio,		
			nearClip,			
			farClip);			
	
	

	DirectX::XMStoreFloat4x4(&projMatrix, P);
}


DirectX::XMFLOAT4X4 Camera::GetViewMatrix()
{
	return viewMatrix;
}

DirectX::XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	return projMatrix;
}

std::shared_ptr<transform> Camera::GetTransform()
{
	return Transform;
}

float Camera::GetAspectRatio()
{
	return aspectRatio;
}

float Camera::GetFieldOfView()
{
	return fieldOfView;
}

void Camera::SetFieldOfView(float fov)
{
	fieldOfView = fov;
}

float Camera::GetNearClip()
{
	return nearClip;
}

void Camera::SetNearClip(float distance)
{
	nearClip = distance;
}

float Camera::GetFarClip()
{
	return farClip;
}

void Camera::SetFarClip(float distance)
{
	farClip = distance;
}

