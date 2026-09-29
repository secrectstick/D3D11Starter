#include "transform.h"

using namespace DirectX;

transform::transform() : 
	position(0,0,0),
	pitchYawRoll(0,0,0),
	scale(1,1,1),
	dirty(false)
{
	XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());
}

void transform::MoveAbsolute(float x, float y, float z)
{
	//ading x, y ,z


	//XMVECTOR p = XMVectorSet(x, y, z, 0);
	//XMVECTOR CurrentPos = XMLoadFloat3(&position);
	//XMStoreFloat3(&position, XMLoadFloat3(&position) + XMVectorSet(x, y, z, 0));
	//dirty = true;

	position.x += x;
	position.y += y;
	position.z += z;

	dirty = true;

}


///////////////////////////////////


void transform::Rotate(float x, float y, float z)
{
	pitchYawRoll.x += x;
	pitchYawRoll.y += y;
	pitchYawRoll.z += z;
	dirty = true;

}

void transform::Scale(float x, float y, float z)
{
	scale.x *= x;
	scale.y *= y;
	scale.z *= z;
	dirty = true;

}

void transform::MoveAbsolute(DirectX::XMFLOAT3 offset)
{
	position.x += offset.x;
	position.y += offset.y;
	position.z += offset.z;

	dirty = true;
}

void transform::Rotate(DirectX::XMFLOAT3 rotation)
{
	pitchYawRoll.x += rotation.x;
	pitchYawRoll.y += rotation.y;
	pitchYawRoll.z += rotation.z;
	dirty = true;
}

void transform::Scale(DirectX::XMFLOAT3 scale)
{
	this->scale.x *= scale.x;
	this->scale.y *= scale.y;
	this->scale.z *= scale.z;

	dirty = true;
}


void transform::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
	dirty = true;

}

void transform::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
	dirty = true;

}

void transform::setPitchYawRoll(float x, float y, float z)
{
	pitchYawRoll.x = x;
	pitchYawRoll.y = y;
	pitchYawRoll.z = z;
	dirty = true;

}

void transform::SetPosition(DirectX::XMFLOAT3 position)
{
	this->position = position;
	dirty = true;
}

void transform::setPitchYawRoll(DirectX::XMFLOAT3 rotation)
{
	this->pitchYawRoll = rotation;
	dirty = true;
}

void transform::SetScale(DirectX::XMFLOAT3 scale)
{
	this->scale = scale;
	dirty = true;
}

DirectX::XMFLOAT3 transform::getPosition()
{
	return position;
}

DirectX::XMFLOAT3 transform::getScale()
{
	return scale;
}

DirectX::XMFLOAT3 transform::getPitchYawRoll()
{
	return pitchYawRoll;
}

DirectX::XMFLOAT4X4 transform::getWorldMatrix()
{
	if (dirty) {

		XMMATRIX t = XMMatrixTranslation(position.x, position.y, position.z);
		XMMATRIX r = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&pitchYawRoll));
		XMMATRIX s = XMMatrixScalingFromVector(XMLoadFloat3(&scale));


		XMStoreFloat4x4(&worldMatrix, s * r * t);
		dirty = false;
	}
		return worldMatrix;

}

DirectX::XMFLOAT4X4 transform::GetWorldInverseTransposeMatrix()
{
	XMFLOAT4X4 result;
	
	XMVECTOR determinant = XMMatrixDeterminant(XMLoadFloat4x4(&worldMatrix));

	XMMATRIX inverseMatrix = XMMatrixInverse(&determinant, XMLoadFloat4x4(&worldMatrix));

	XMMATRIX invTransposeMatrix = XMMatrixTranspose(inverseMatrix);

	XMStoreFloat4x4(&result, invTransposeMatrix);

	return result;
}
