#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <memory>

#include "Camera.h"
#include "Transform.h"

class Material
{
public:
	Material(Microsoft::WRL::ComPtr<ID3D11PixelShader> ps, Microsoft::WRL::ComPtr<ID3D11VertexShader> vs, DirectX::XMFLOAT3 tint);

	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetPixelShader();
	Microsoft::WRL::ComPtr<ID3D11VertexShader> GetVertexShader();
	DirectX::XMFLOAT3 GetColorTint();

	void SetPixelShader(Microsoft::WRL::ComPtr<ID3D11PixelShader> ps);
	void SetVertexShader(Microsoft::WRL::ComPtr<ID3D11VertexShader> vs);
	void SetColorTint(DirectX::XMFLOAT3 tint);

private:

	// shaders
	Microsoft::WRL::ComPtr<ID3D11PixelShader> ps;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vs;

	// color
	DirectX::XMFLOAT3 colorTint;
};