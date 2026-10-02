#include "Entity.h"
#include "Graphics.h"
#include "bufferStruct.h"

std::shared_ptr<Mesh> Entity::GetMesh()
{
	return mesh;
}

std::shared_ptr<transform> Entity::GetTransform()
{
	return TransForm;
}

Entity::Entity(std::shared_ptr<Mesh> _mesh, std::shared_ptr<transform> _transform)
{
	mesh = _mesh;
	TransForm = _transform;
}

void Entity::Draw()
{
	mesh.get()->Draw();
}
