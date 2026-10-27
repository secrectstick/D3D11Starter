#include "Game.h"
#include "Graphics.h"
#include "Vertex.h"
#include "Input.h"
#include "PathHelpers.h"
#include "Window.h"
#include "Mesh.h"
#include "bufferStruct.h"
#include "WICTextureLoader.h"

#include <DirectXMath.h>

// Needed for a helper function to load pre-compiled shader files
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>
#include <cmath>
#include "Material.h"



// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// The constructor is called after the window and graphics API
// are initialized but before the game loop begins
// --------------------------------------------------------
Game::Game()
{
	this->TransForm = std::make_unique<transform>();

	//init the camera
	{
		std::shared_ptr<Camera> camera = std::make_shared<Camera>(
			XMFLOAT3(0.0f, 0.0f, -5.0f),	// Position
			XM_PIDIV4,				// Field of view
			Window::AspectRatio(),  // Aspect ratio
			0.01f,					// Near clip
			100.0f,					// Far clip
			5.0f,					// Move speed
			0.002f);				// Look speed

		camList.push_back(camera);

		std::shared_ptr<Camera>camera2 = std::make_shared<Camera>(
			XMFLOAT3(0.0f, 0.0f, -1.0f),	// Position
			XM_PIDIV2,				// Field of view
			Window::AspectRatio(),  // Aspect ratio
			0.01f,					// Near clip
			100.0f,					// Far clip
			5.0f,					// Move speed
			0.002f);				// Look speed

		camList.push_back(camera2);

		activeCamIndex = 0;
		this->camera = camList[activeCamIndex];
	}

	// Initialize ImGui itself & platform/renderer backends
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(Window::Handle());
	ImGui_ImplDX11_Init(Graphics::Device.Get(), Graphics::Context.Get());
	
	// Pick a style (uncomment one of these 3)
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();
	//ImGui::StyleColorsClassic();

	
	//initlizing the bgColor
	this->bgColor = std::make_unique<float[]>(4);
	this->bgColor[0] = 0.5f; // R
	this->bgColor[1] = 0.5f; // G
	this->bgColor[2] = 0.5f; // B
	this->bgColor[3] = 1.0f; // A

	//const VS data
	{
		VertexShaderExternalData vsData{};

		XMFLOAT4X4 viewMatrixStorage = camera->GetViewMatrix();
		XMFLOAT4X4 projMatrixStorage = camera->GetProjectionMatrix();

		XMMATRIX ident = XMMatrixIdentity();
		XMMATRIX view = XMLoadFloat4x4(&viewMatrixStorage);
		XMMATRIX proj = XMLoadFloat4x4(&projMatrixStorage);

		XMStoreFloat4x4(&vsData.WorldMatrix, ident);
		XMStoreFloat4x4(&vsData.ViewMatrix, view);
		XMStoreFloat4x4(&vsData.ProjMatrix, proj);

		this->vsConstData = std::make_unique<VertexShaderExternalData>(vsData);

		XMFLOAT3 offsetMulti = XMFLOAT3(0.1f, 0.0f, 0.0f);
		this->constOffsetMulti = std::make_unique<XMFLOAT3>(offsetMulti);


		PixelShaderExternalData psData{};
		psData.colorTint = XMFLOAT3(1, 0.5, 0.5);
		psData.time = 0.0f;
		psData.uvScale = XMFLOAT2(1.0f, 1.0f);	
		psData.uvOffset = XMFLOAT2(0.0f, 0.0f);
		this->psConstData = std::make_unique<PixelShaderExternalData>(psData);
	}

	

	isDemoShowing = false;

	this->customNumber = std::make_unique<float>();
	*(this->customNumber) = 1.0f;


	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	CreateGeometry();

	// Set initial graphics API state
	//  - These settings persist until we change them
	//  - Some of these, like the primitive topology & input layout, probably won't change
	//  - Others, like setting shaders, will need to be moved elsewhere later
	{
		// Tell the input assembler (IA) stage of the pipeline what kind of
		// geometric primitives (points, lines or triangles) we want to draw.  
		// Essentially: "What kind of shape should the GPU draw with our vertices?"
		Graphics::Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Ensure the pipeline knows how to interpret all the numbers stored in
		// the vertex buffer. For this course, all of your vertices will probably
		// have the same layout, so we can just set this once at startup.
		Graphics::Context->IASetInputLayout(inputLayout.Get());

	}

	//set up const buffer heap
	Graphics::ResizeConstBufferHeap(256 * 10000);
	


}


// --------------------------------------------------------
// Clean up memory or objects created by this class
// 
// Note: Using smart pointers means there probably won't
//       be much to manually clean up here!
// --------------------------------------------------------
Game::~Game()
{
	// ImGui clean up
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

}


Microsoft::WRL::ComPtr<ID3D11PixelShader> Game::LoadPixelShader(const wchar_t* shaderPath)
{
	// Read in shader object 
	ID3DBlob* shaderBlob;
	D3DReadFileToBlob(shaderPath, &shaderBlob);

	// Create pixel shader and return it
	Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
	Graphics::Device->CreatePixelShader(
		shaderBlob->GetBufferPointer(),	// Pointer to blob's contents
		shaderBlob->GetBufferSize(),	// How big is that data?
		0,								// No classes in this shader
		shader.GetAddressOf());			// Address of the pixel shader pointer
	return shader;
}



Microsoft::WRL::ComPtr<ID3D11VertexShader> Game::LoadVertexShader(const wchar_t* shaderPath)
{
	// Read in shader object
	ID3DBlob* shaderBlob;
	D3DReadFileToBlob(shaderPath, &shaderBlob);

	// Create vertex shader and return it
	Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
	Graphics::Device->CreateVertexShader(
		shaderBlob->GetBufferPointer(),	// Pointer to blob's contents
		shaderBlob->GetBufferSize(),	// How big is that data?
		0,								// No classes in this shader
		shader.GetAddressOf());			// Address of the vertex shader pointer
	return shader;
}


// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// and also created the Input Layout that describes our 
// vertex data to the rendering pipeline. 
// - Input Layout creation is done here because it must 
//    be verified against vertex shader byte code
// - We'll have that byte code already loaded below
// --------------------------------------------------------
void Game::LoadShaders()
{
	// set up BLOB (or Binary Large OBjects)
	ID3DBlob* vertexShaderBlob;
	D3DReadFileToBlob(FixPath(L"VertexShader.cso").c_str(), &vertexShaderBlob);

		
	

	// Create an input layout 
	//  - This describes the layout of data sent to a vertex shader
	//  - In other words, it describes how to interpret data (numbers) in a vertex buffer
	//  - Doing this NOW because it requires a vertex shader's byte code to verify against!
	//  - Luckily, we already have that loaded (the vertex shader blob above)
	{
		D3D11_INPUT_ELEMENT_DESC inputElements[3] = {};

		// Set up the first element - a position, which is 3 float values
		inputElements[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;				// Most formats are described as color channels; really it just means "Three 32-bit floats"
		inputElements[0].SemanticName = "POSITION";							// This is "POSITION" - needs to match the semantics in our vertex shader input!
		inputElements[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	// How far into the vertex is this?  Assume it's after the previous element

		inputElements[1].Format = DXGI_FORMAT_R32G32_FLOAT;					
		inputElements[1].SemanticName = "TEXCOORD";							
		inputElements[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	

		inputElements[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;				
		inputElements[2].SemanticName = "NORMAL";							
		inputElements[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	

		// Create the input layout, verifying our description against actual shader code
		Graphics::Device->CreateInputLayout(
			inputElements,							// An array of descriptions
			3,										// How many elements in that array?
			vertexShaderBlob->GetBufferPointer(),	// Pointer to the code of a shader that uses this layout
			vertexShaderBlob->GetBufferSize(),		// Size of the shader code that uses this layout
			inputLayout.GetAddressOf());			// Address of the resulting ID3D11InputLayout pointer
	}
}


// --------------------------------------------------------
// Creates the geometry we're going to draw
// --------------------------------------------------------
void Game::CreateGeometry()
{

	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; // What happens outside the 0-1 uv range
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;		// How to handle sampling between pixels
	sampDesc.MaxAnisotropy = 16;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	Graphics::Device->CreateSamplerState(&sampDesc, sampler.GetAddressOf());

	// Load textures
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> bricksSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> woodSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> asphaltSRV;

	CreateWICTextureFromFile(Graphics::Device.Get(), Graphics::Context.Get(), FixPath(L"../../Assets/Textures/Bricks.png").c_str(), 0, bricksSRV.GetAddressOf());
	CreateWICTextureFromFile(Graphics::Device.Get(), Graphics::Context.Get(), FixPath(L"../../Assets/Textures/Wood.png").c_str(), 0, woodSRV.GetAddressOf());
	CreateWICTextureFromFile(Graphics::Device.Get(), Graphics::Context.Get(), FixPath(L"../../Assets/Textures/Asphalt.png").c_str(), 0, asphaltSRV.GetAddressOf());

	




	//make a material
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertShader = LoadVertexShader(FixPath(L"VertexShader.cso").c_str());

	DirectX::XMFLOAT3 tint = DirectX::XMFLOAT3(1.0f, 0.5f, 0.5f);
	std::shared_ptr<Material> uvMat = std::make_shared<Material>(LoadPixelShader(FixPath(L"DebugUVsPS.cso").c_str()), vertShader, tint);
	materialList.push_back(uvMat);
	std::shared_ptr<Material> normMat = std::make_shared<Material>(LoadPixelShader(FixPath(L"DebugNormalsPS.cso").c_str()), vertShader, tint);
	materialList.push_back(normMat);
	std::shared_ptr<Material> customMat = std::make_shared<Material>(LoadPixelShader(FixPath(L"CustomPS.cso").c_str()), vertShader, tint);
	materialList.push_back(customMat);

	DirectX::XMFLOAT3 white = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	std::shared_ptr<Material> combowhiteMat = std::make_shared<Material>(LoadPixelShader(FixPath(L"ComboPixelShader.cso").c_str()), vertShader, white);
	combowhiteMat->AddSampler(0, sampler);
	combowhiteMat->AddSRV(0, bricksSRV);
	combowhiteMat->AddSRV(1, asphaltSRV);
	materialList.push_back(combowhiteMat);

	DirectX::XMFLOAT3 red = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	std::shared_ptr<Material> redMat = std::make_shared<Material>(LoadPixelShader(FixPath(L"PixelShader.cso").c_str()), vertShader, red);
	redMat->AddSampler(0, sampler);
	redMat->AddSRV(0, woodSRV);
	materialList.push_back(redMat);

	DirectX::XMFLOAT3 magenta = DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f);
	std::shared_ptr<Material> magentaMat = std::make_shared<Material>(LoadPixelShader(FixPath(L"PixelShader.cso").c_str()), vertShader, magenta);
	magentaMat->AddSampler(0, sampler);
	magentaMat->AddSRV(0, bricksSRV);
	materialList.push_back(magentaMat);






	//make meshes
	std::shared_ptr<Mesh> mesh1 = std::make_shared<Mesh>("Cube", FixPath("../../Assets/Meshes/cube.obj").c_str());
	std::shared_ptr<Mesh> mesh2 = std::make_shared<Mesh>("Cylinder", FixPath("../../Assets/Meshes/cylinder.obj").c_str());
	std::shared_ptr<Mesh> mesh3 = std::make_shared<Mesh>("Helix", FixPath("../../Assets/Meshes/helix.obj").c_str());
	std::shared_ptr<Mesh> mesh4 = std::make_shared<Mesh>("Sphere", FixPath("../../Assets/Meshes/sphere.obj").c_str());
	std::shared_ptr<Mesh> mesh5 = std::make_shared<Mesh>("Torus", FixPath("../../Assets/Meshes/torus.obj").c_str());
	std::shared_ptr<Mesh> mesh6 = std::make_shared<Mesh>("Quad", FixPath("../../Assets/Meshes/quad.obj").c_str());
	std::shared_ptr<Mesh> mesh7 = std::make_shared<Mesh>("Quad_double", FixPath("../../Assets/Meshes/quad_double_sided.obj").c_str());
	meshList.push_back(mesh1);
	meshList.push_back(mesh2);
	meshList.push_back(mesh3);
	meshList.push_back(mesh4);
	meshList.push_back(mesh5);
	meshList.push_back(mesh6);
	meshList.push_back(mesh7);

	//make first row of entities with normal shader
	{
		for (int i = 0; i < 7; i++)
		{
			std::shared_ptr<transform> tr = std::make_shared<transform>();

			float x = -9.0f + (i * 3.0f);
			tr->MoveAbsolute(x, 4.0f, 10.0f);

			std::shared_ptr<Entity> entity = std::make_shared<Entity>(meshList[i], tr, normMat);

			entityList.push_back(entity);
		}
	}

	//make second row of entities with uv shader
	{
		for (int i = 0; i < 7; i++)
		{
			std::shared_ptr<transform> tr = std::make_shared<transform>();

			float x = -9.0f + (i * 3.0f);
			tr->MoveAbsolute(x, 1.0f, 10.0f);

			std::shared_ptr<Entity> entity = std::make_shared<Entity>(meshList[i], tr, uvMat);

			entityList.push_back(entity);
		}
	}

	//make third row of entities with custom shader & pixel shader
	{


		std::shared_ptr<Material> materialList[] = {
			combowhiteMat, redMat, magentaMat, customMat,
			magentaMat, redMat, combowhiteMat
		};

		for (int i = 0; i < 7; i++)
		{
			std::shared_ptr<transform> tr = std::make_shared<transform>();

			float x = -9.0f + (i * 3.0f);
			tr->MoveAbsolute(x, -2.0f, 10.0f);

			std::shared_ptr<Entity> entity = std::make_shared<Entity>(
				meshList[i], tr, materialList[i]);

			entityList.push_back(entity);
		}
	}




	
}


// --------------------------------------------------------
// Handle resizing to match the new window size
//  - Eventually, we'll want to update our 3D camera
// --------------------------------------------------------
void Game::OnResize()
{

	for(int i = 0; i < camList.size(); i++)
	{
		camList.at(i)->UpdateProjectionMatrix(Window::AspectRatio());
	}
}


// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Example input checking: Quit if the escape key is pressed
	if (Input::KeyDown(VK_ESCAPE))
		Window::Quit();

	// Put this all in a helper method that is called from Game::Update()
	// Feed fresh data to ImGui
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = deltaTime;
	io.DisplaySize.x = (float)Window::Width();
	io.DisplaySize.y = (float)Window::Height();
	// Reset the frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	// Determine new input capture
	Input::SetKeyboardCapture(io.WantCaptureKeyboard);
	Input::SetMouseCapture(io.WantCaptureMouse);

	// Show the UI window
	{
		
		ImGui::Begin("My Window"); // Everything after is part of the window

		if (ImGui::CollapsingHeader("App Details")) {
			ImGui::Text("current framerate: %.2f ", ImGui::GetIO().Framerate);
			ImGui::Text("window size: %dx%d ", Window::Width(), Window::Height());

			ImGui::ColorEdit4("select background color", this->bgColor.get());


			// Create a button and test for a click
			if (ImGui::Button("Press to show demo window"))
			{
				this->isDemoShowing = !this->isDemoShowing;
			}

			if (this->isDemoShowing) {
				ImGui::ShowDemoWindow();
			}
		}

		if (ImGui::CollapsingHeader("Meshes"))
		{

			static int openMaterialIndex = -1;



			for (int i = 0; i < meshList.size();i++) {

				ImGui::SetNextItemOpen(openMaterialIndex == i);

				//make a collapseable header for each mesh's info in the ui window
				if (ImGui::CollapsingHeader(this->meshList.at(i).get()->getName()))
				{
					if (openMaterialIndex != i)
					{
						openMaterialIndex = i;
					}

					ImGui::Text("triangles: %i", this->meshList.at(i).get()->getTriCount());
					ImGui::Text("vertices: %i", this->meshList.at(i).get()->getVertexCount());
					ImGui::Text("indices: %i", this->meshList.at(i).get()->getIndexCount());
				}


			}
		}

		if (ImGui::CollapsingHeader("constant Buffer controls")) {

			ImGui::DragFloat3("select movement", &this->constOffsetMulti.get()->x);


		}

		if (ImGui::CollapsingHeader("Entities")) {

			for (int i = 0; i < entityList.size();i++) {

				static int openMaterialIndex = -1;

				std::string str = "Entity " + std::to_string(i + 1);

				const char* const_str = str.c_str();

				ImGui::SetNextItemOpen(openMaterialIndex == i);

				if (ImGui::CollapsingHeader(const_str))
				{
					if (openMaterialIndex != i)
					{
						openMaterialIndex = i;
					}
					XMFLOAT3 position = entityList.at(i)->GetTransform()->getPosition();
					XMFLOAT3 rotation = entityList.at(i)->GetTransform()->getPitchYawRoll();
					XMFLOAT3 scale = entityList.at(i)->GetTransform()->getScale();

					if (ImGui::DragFloat3("position", &position.x)) {
						entityList.at(i)->GetTransform()->SetPosition(position.x, position.y, position.z);
					}


					if (ImGui::DragFloat3("rotation", &rotation.x)) {
						entityList.at(i)->GetTransform()->setPitchYawRoll(rotation.x, rotation.y, rotation.z);
					}


					if (ImGui::DragFloat3("scale", &scale.x)) {
						entityList.at(i)->GetTransform()->SetScale(scale.x, scale.y, scale.z);
					}

					ImGui::Text("Mesh index count: %i", entityList.at(i)->GetMesh().get()->getIndexCount());


				}


			}
		}

		if (ImGui::CollapsingHeader("cam info & ctrl"))
		{
			if (ImGui::Button("Press to change to next cam"))
			{
				activeCamIndex++;
				if (activeCamIndex >= camList.size()) {
					activeCamIndex = 0;
				}
			}

			// making stack vars so the l-value doesn't freak out
			Camera cam = *camList.at(activeCamIndex).get();
			XMFLOAT3 pos = cam.GetTransform().get()->getPosition();
			XMFLOAT3 rot = cam.GetTransform().get()->getPitchYawRoll();
			XMFLOAT3 fwd = cam.GetTransform().get()->GetForward();
			XMFLOAT3 up = cam.GetTransform().get()->GetUp();
			XMFLOAT3 right = cam.GetTransform().get()->GetRight();

			ImGui::Text("active cam index: %i", activeCamIndex);
			ImGui::Text("active cam fov (radians): %.2f", cam.GetFieldOfView());
			ImGui::InputFloat3("position", &pos.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
			ImGui::InputFloat3("rotation", &rot.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
			ImGui::InputFloat3("forward", &fwd.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
			ImGui::InputFloat3("up", &up.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
			ImGui::InputFloat3("right", &right.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
			


		}

		if (ImGui::CollapsingHeader("material info"))
		{
			static int openMaterialIndex = -1;

			for(int i =0 ; i < materialList.size(); i++)
			{
				std::string str = "Material " + std::to_string(i + 1);
				const char* const_str = str.c_str();

				ImGui::SetNextItemOpen(openMaterialIndex == i);

				if (ImGui::CollapsingHeader(const_str))
				{
					if (openMaterialIndex != i)
					{
						openMaterialIndex = i;
					}
					XMFLOAT3 colorTint = materialList.at(i)->GetColorTint();
					XMFLOAT2 uvScale = materialList.at(i)->GetUVScale();
					XMFLOAT2 uvOffset = materialList.at(i)->GetUVOffset();

					if (ImGui::ColorEdit3("color tint", &colorTint.x)) {
						materialList.at(i)->SetColorTint(colorTint);
					}
					if (ImGui::DragFloat2("uv scale", &uvScale.x)) {
						materialList.at(i)->SetUVScale(uvScale);
					}
					if (ImGui::SliderFloat2("uv offset", &uvOffset.x, 0.0f, 1.0f)) {
						materialList.at(i)->SetUVOffset(uvOffset);
					}

					for (auto imageSRV : materialList.at(i)->GetSRVs())
					{
						std::string str = "SRV slot: " + std::to_string(imageSRV.first);
						const char* const_str = str.c_str();
						ImGui::Text(const_str);

						ImGui::Image(imageSRV.second.Get(), ImVec2(64, 64));
					}

				}
			}
		}

		ImGui::End(); // Ends the current window
	}

	//custom movements for each entity
	for (int i = 0; i < entityList.size();i++) {
		
		entityList.at(i)->GetTransform()->Rotate(1 * deltaTime, -1 * deltaTime, 0);
	}

	camList.at(activeCamIndex)->Update(deltaTime);
}


// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Frame START
	// - These things should happen ONCE PER FRAME
	// - At the beginning of Game::Draw() before drawing *anything*
	{
		// Clear the back buffer (erase what's on screen) and depth buffer
		const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };
		Graphics::Context->ClearRenderTargetView(Graphics::BackBufferRTV.Get(),	this->bgColor.get());
		Graphics::Context->ClearDepthStencilView(Graphics::DepthBufferDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	//Draw Step
	

	for (int i = 0; i < entityList.size();i++) {

		std::shared_ptr<Material> mat = this->entityList.at(i)->GetMaterial();
		mat->BindSRVsAndSamplers();
		
		Graphics::Context->VSSetShader(this->entityList.at(i)->GetMaterial()->GetVertexShader().Get(), 0, 0);
		Graphics::Context->PSSetShader(this->entityList.at(i)->GetMaterial()->GetPixelShader().Get(), 0, 0);


		this->vsConstData->WorldMatrix = this->entityList.at(i)->GetTransform()->getWorldMatrix();
		this->vsConstData->ViewMatrix = camList.at(activeCamIndex)->GetViewMatrix();
		this->vsConstData->ProjMatrix = camList.at(activeCamIndex)->GetProjectionMatrix();
		Graphics::FillAndBindNextConstBuffer(vsConstData.get(), sizeof(VertexShaderExternalData), D3D11_VERTEX_SHADER, 0);


		this->psConstData->time = totalTime;
		this->psConstData->colorTint = this->entityList.at(i)->GetMaterial()->GetColorTint();
		this->psConstData->uvScale = this->entityList.at(i)->GetMaterial()->GetUVScale();
		this->psConstData->uvOffset = this->entityList.at(i)->GetMaterial()->GetUVOffset();
		Graphics::FillAndBindNextConstBuffer(psConstData.get(), sizeof(PixelShaderExternalData), D3D11_PIXEL_SHADER, 0);

		

		this->entityList.at(i).get()->Draw();
	}

	///////

	ImGui::Render(); // Turns this frame’s UI into renderable triangles
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); // Draws it to the scree

	// Frame END
	// - These should happen exactly ONCE PER FRAME
	// - At the very end of the frame (after drawing *everything*)
	{
		// Present at the end of the frame
		bool vsync = Graphics::VsyncState();
		Graphics::SwapChain->Present(
			vsync ? 1 : 0,
			vsync ? 0 : DXGI_PRESENT_ALLOW_TEARING);

		// Re-bind back buffer and depth buffer after presenting
		Graphics::Context->OMSetRenderTargets(
			1,
			Graphics::BackBufferRTV.GetAddressOf(),
			Graphics::DepthBufferDSV.Get());
	}
}



