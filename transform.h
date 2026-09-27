#pragma once

#include <DirectXMath.h>


class transform
{
public:
	transform();
	

	// mutators
	void MoveAbsolute(float x, float y, float z);
	void Rotate(float x, float y, float z);
	void Scale(float x, float y, float z);

	
	void MoveAbsolute(DirectX::XMFLOAT3 offset);
	void Rotate(DirectX::XMFLOAT3 rotation);
	void Scale(DirectX::XMFLOAT3 scale);



	// setters 
	void SetPosition(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void setPitchYawRoll(float x, float y, float z);
	void SetPosition(DirectX::XMFLOAT3 position);
	void setPitchYawRoll(DirectX::XMFLOAT3 rotation);
	void SetScale(DirectX::XMFLOAT3 scale);



	// getters
	DirectX::XMFLOAT3 getPosition();
	DirectX::XMFLOAT3 getScale();
	DirectX::XMFLOAT3 getPitchYawRoll();


	DirectX::XMFLOAT4X4 getWorldMatrix();
	DirectX::XMFLOAT4X4 GetWorldInverseTransposeMatrix();

private:
	bool dirty;

	DirectX::XMFLOAT3 position;
	
	DirectX::XMFLOAT3 scale;

	DirectX::XMFLOAT3 pitchYawRoll;

	DirectX::XMFLOAT4X4 worldMatrix;
};

