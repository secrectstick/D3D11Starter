#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <memory>

class Mesh
{
public:
	// fields for Imgui window
	std::unique_ptr<float[]> bgColor;
	bool isDemoShowing;

	std::unique_ptr<float> customNumber;

	// Basic OOP setup
	Mesh();
	~Mesh();
	Mesh(const Mesh&) = delete; // Remove copy constructor
	Mesh& operator=(const Mesh&) = delete; // Remove copy-assignment operator

	// Primary functions
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);
	void OnResize();

private:
	int idxCount;
	int vertCount;
	

	// Buffers to hold actual geometry data
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

};

