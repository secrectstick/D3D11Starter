#pragma once
#include <memory>
#include "Mesh.h"
#include "transform.h"
#include "bufferStruct.h"


class Entity
{
public:
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<transform> TransForm;
	
	std::shared_ptr<Mesh> GetMesh();
	std::shared_ptr<transform> GetTransform(); 
	
	Entity(std::shared_ptr<Mesh> _mesh, std::shared_ptr<transform> _transform);

	void Draw();
	

private:
};

