#include "Camera.h"
#include <memory>
#include <wrl/client.h> // Used for ComPtr
#include <d3d11.h>
#include "mesh.h"

class Sky
{
public:

	//public functions
	Sky(
		const wchar_t* right,
		const wchar_t* left,
		const wchar_t* up,
		const wchar_t* down,
		const wchar_t* front,
		const wchar_t* back,
		std::shared_ptr<Mesh> mesh,
		Microsoft::WRL::ComPtr<ID3D11VertexShader> skyVS,
		Microsoft::WRL::ComPtr<ID3D11PixelShader> skyPS,
		Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerOptions
	);

	~Sky();

	void Draw(std::shared_ptr<Camera> camera);

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetSkyTexture();
private:

	//fields
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerOptions;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> skySRV;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> skyDepthState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> skyRasterState;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> skyPS;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> skyVS;
	std::shared_ptr<Mesh> skyMesh;

	// privatefunctions
	void InitRenderers();

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CreateCubemap(
		const wchar_t* right,
		const wchar_t* left,
		const wchar_t* up,
		const wchar_t* down,
		const wchar_t* front,
		const wchar_t* back);

	

	
	
	

	

};

