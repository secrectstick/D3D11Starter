#pragma once

#include <DirectXMath.h>
#include "Light.h"

struct VertexShaderExternalData {
	DirectX::XMFLOAT4X4 WorldMatrix;
	DirectX::XMFLOAT4X4 invTransMatrix;
	DirectX::XMFLOAT4X4 ViewMatrix;
	DirectX::XMFLOAT4X4 ProjMatrix;

};



struct PixelShaderExternalData
{
	
	Light lights[LIGHT_COUNT];
	int lightCount;
	DirectX::XMFLOAT3 ambientColor;

	
	DirectX::XMFLOAT3 cameraPosition;
	float pad; 

	
	DirectX::XMFLOAT3 colorTint;
	float time;
	DirectX::XMFLOAT2 uvScale;
	DirectX::XMFLOAT2 uvOffset;
};