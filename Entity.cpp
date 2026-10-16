#include "Entity.h"
#include "Graphics.h"
#include "bufferStruct.h"

std::shared_ptr<Mesh> Entity::GetMesh()
{
	return mesh;
}

std::shared_ptr<Material> Entity::GetMaterial()
{
	return material;
}

std::shared_ptr<transform> Entity::GetTransform()
{
	return TransForm;
}

void Entity::SetMesh(std::shared_ptr<Mesh> mesh)
{
	this->mesh = mesh;
}

void Entity::SetMaterial(std::shared_ptr<Material> material)
{
	this->material = material;
}

Entity::Entity(std::shared_ptr<Mesh> _mesh, std::shared_ptr<transform> _transform, std::shared_ptr<Material> _material)
{
	mesh = _mesh;
	TransForm = _transform;
	material = _material;
}

void Entity::Draw()
{
	mesh.get()->Draw();
}
