#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <memory>
// This code assumes files are in "ImGui" subfolder!
// Adjust as necessary for your own folder structure and project setup
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include <vector>
#include "Mesh.h"
#include "bufferStruct.h"
#include "transform.h"
#include "Entity.h"
#include "Camera.h"

class Game
{
public:
	// fields for Imgui window
	std::unique_ptr<float[]> bgColor;
	bool isDemoShowing;

	std::unique_ptr<float> customNumber;

	std::unique_ptr<VertexShaderExternalData> vsConstData;
	std::unique_ptr<DirectX::XMFLOAT3> constOffsetMulti;

	std::vector<std::shared_ptr<Mesh>> meshList;

	std::vector<std::shared_ptr<Entity>> entityList;


	std::shared_ptr<transform> TransForm;

	// Basic OOP setup
	Game();
	~Game();
	Game(const Game&) = delete; // Remove copy constructor
	Game& operator=(const Game&) = delete; // Remove copy-assignment operator

	// Primary functions
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);
	void OnResize();

private:

	// Initialization helper methods - feel free to customize, combine, remove, etc.
	void LoadShaders();
	void CreateGeometry();

	// Note the usage of ComPtr below
	//  - This is a smart pointer for objects that abide by the
	//     Component Object Model, which DirectX objects do
	//  - More info here: https://github.com/Microsoft/DirectXTK/wiki/ComPtr

	// Buffers to hold actual geometry data
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	// Shaders and shader-related constructs
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

	//constant buffer
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;

	std::shared_ptr<Camera> camera;

	std::vector<std::shared_ptr<Camera>> camList;
	int activeCamIndex;
};

