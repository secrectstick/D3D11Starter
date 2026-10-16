#pragma once

#include <DirectXMath.h>

struct VertexShaderExternalData {
	DirectX::XMFLOAT4X4 WorldMatrix;
	DirectX::XMFLOAT4X4 ViewMatrix;
	DirectX::XMFLOAT4X4 ProjMatrix;

};

struct PixelShaderExternalData
{
	DirectX::XMFLOAT3 colorTint;
	float time;
};