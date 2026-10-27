#include "Material.h"
#include "Graphics.h"

Material::Material(Microsoft::WRL::ComPtr<ID3D11PixelShader> ps, 
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vs, 
	DirectX::XMFLOAT3 tint,
	DirectX::XMFLOAT2 uvScale,
	DirectX::XMFLOAT2 uvOffset) :
	ps(ps),
	vs(vs),
	colorTint(tint),
	uvScale(uvScale),
	uvOffset(uvOffset)
{
}

Microsoft::WRL::ComPtr<ID3D11PixelShader> Material::GetPixelShader()
{
	return ps;
}

Microsoft::WRL::ComPtr<ID3D11VertexShader> Material::GetVertexShader()
{
	return vs;
}

DirectX::XMFLOAT3 Material::GetColorTint()
{
	return colorTint;
}

DirectX::XMFLOAT2 Material::GetUVScale()
{
	return uvScale;
}

DirectX::XMFLOAT2 Material::GetUVOffset()
{
	return uvOffset;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Material::GetSRV(unsigned int index)
{
	return SRVs[index];
}

Microsoft::WRL::ComPtr<ID3D11SamplerState> Material::GetSampler(unsigned int index)
{
	return samplers[index];
}

std::unordered_map<unsigned int, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& Material::GetSRVs()
{
	return SRVs;
}

std::unordered_map<unsigned int, Microsoft::WRL::ComPtr<ID3D11SamplerState>>& Material::GetSamplers()
{
	return samplers;
}

void Material::SetPixelShader(Microsoft::WRL::ComPtr<ID3D11PixelShader> ps)
{
	this->ps = ps;
}

void Material::SetVertexShader(Microsoft::WRL::ComPtr<ID3D11VertexShader> vs)
{
	this->vs = vs;
}

void Material::SetColorTint(DirectX::XMFLOAT3 tint)
{
	this->colorTint = tint;
}

void Material::SetUVScale(DirectX::XMFLOAT2 scale)
{
	uvScale = scale;
}

void Material::SetUVOffset(DirectX::XMFLOAT2 offset)
{
	uvOffset = offset;
}

void Material::AddSRV(unsigned int index, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv)
{
	SRVs.insert({ index, srv });
}

void Material::AddSampler(unsigned int index, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler)
{
	samplers.insert({ index, sampler });
}

void Material::RemoveSRV(unsigned int idx)
{
	SRVs.erase(idx);
}

void Material::RemoveSampler(unsigned int idx)
{
	samplers.erase(idx);
}

void Material::BindSRVsAndSamplers()
{
	for (auto& srv : SRVs)
	{
		Graphics::Context->PSSetShaderResources(
			srv.first,
			1,
			srv.second.GetAddressOf()
		);
	}

	for (auto& sampler : samplers)
	{
		Graphics::Context->PSSetSamplers(
			sampler.first,
			1,
			sampler.second.GetAddressOf()
		);
	}
}
