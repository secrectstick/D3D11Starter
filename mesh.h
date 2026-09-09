#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <memory>
#include "Vertex.h"

class Mesh
{
public:

	// Basic OOP setup
	Mesh(int indexCount, int vertexCount, Vertex* vertices, int* indices);
	~Mesh();
	Mesh(const Mesh&) = delete; // Remove copy constructor
	Mesh& operator=(const Mesh&) = delete; // Remove copy-assignment operator

	// Primary functions
	void Draw(float deltaTime, float totalTime);
	int getVertexCount();
	int getIndexCount();
	int getTriCount();
	ID3D11Buffer* getIndexBuffer();
	ID3D11Buffer* getVertexBuffer();


private:
	int idxCount;
	int vertCount;
	

	// Buffers to hold actual geometry data
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	// private help functions
	void createBuffers(Vertex* vertices, int* indices);


};

