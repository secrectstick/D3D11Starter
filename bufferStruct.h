#pragma once

#include <DirectXMath.h>

struct VertexShaderExternalData {
	DirectX::XMFLOAT4 ColorTint;
	DirectX::XMFLOAT4X4 WorldMatrix;
};