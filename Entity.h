#pragma once
#include <memory>
#include "Mesh.h"
#include "transform.h"
#include "bufferStruct.h"
#include "Material.h"



class Entity
{
public:
	std::shared_ptr<Mesh> GetMesh();
	std::shared_ptr<Material> GetMaterial();
	std::shared_ptr<transform> GetTransform(); 

	void SetMesh(std::shared_ptr<Mesh> mesh);
	void SetMaterial(std::shared_ptr<Material> material);
	
	Entity(std::shared_ptr<Mesh> _mesh, std::shared_ptr<transform> _transform, std::shared_ptr<Material> _material);

	void Draw();
	

private:
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;
	std::shared_ptr<transform> TransForm;

};

