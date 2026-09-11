#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <memory>
#include "Vertex.h"

class Mesh
{
public:

	// Basic OOP setup
	Mesh(const char* _name, int indexCount, int vertexCount, Vertex* vertices, int* indices);
	~Mesh();
	Mesh(const Mesh&) = delete; // Remove copy constructor
	Mesh& operator=(const Mesh&) = delete; // Remove copy-assignment operator

	// Primary functions
	void Draw(float deltaTime, float totalTime);
	int getVertexCount();
	int getIndexCount();
	int getTriCount();
	const char* getName();
	ID3D11Buffer* getIndexBuffer();
	ID3D11Buffer* getVertexBuffer();


private:
	//private fields
	int idxCount;
	int vertCount;
	const char* name;
	

	// Buffers to hold actual geometry data
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	// private help functions
	void createBuffers(Vertex* vertices, int* indices);


};

