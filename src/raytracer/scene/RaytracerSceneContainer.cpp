#include "RaytracerSceneContainer.h"

namespace Raytracer
{
	RaytracerSceneContainer::RaytracerSceneContainer()
	{

	}

	RaytracerSceneContainer::~RaytracerSceneContainer()
	{
		Destroy();
	}

	void RaytracerSceneContainer::Initialize()
	{
		this->MakeDefault();
	}

	void RaytracerSceneContainer::Destroy()
	{
		m_ObjectArray.clear();
		m_MeshArray.clear();
		m_MaterialArray.clear();
		m_TextureArray.clear();

		m_ObjectDictionary.clear();
		m_MeshDictionary.clear();
		m_MaterialDictionary.clear();
		m_TextureDictionary.clear();
	}

	void RaytracerSceneContainer::MakeDefault()
	{
		//Clear previous objects
		m_ObjectArray.clear();
		m_MeshArray.clear();
		m_MaterialArray.clear();
		m_TextureArray.clear();
		m_CameraArray.clear();

		m_ObjectDictionary.clear();
		m_MeshDictionary.clear();
		m_MaterialDictionary.clear();
		m_TextureDictionary.clear();
		m_CameraDictionary.clear();
		//Creating default objects with id = 0

		//----------------------------------------------
		this->addCamera(RaytracerCamera::Default());
		//----------------------------------------------
		this->addTexture(RaytracerTexture::Default());
		//----------------------------------------------
		this->addMaterial(RaytracerMaterial::Default());
		//----------------------------------------------
		this->addMesh(RaytracerMeshEmpty::Create(0));
		//----------------------------------------------
		this->addObject(RaytracerObject::Default());
		//----------------------------------------------
		
		this->m_MainCamera = m_CameraArray[0];
	}

	void RaytracerSceneContainer::Validate()
	{
		//TODO : Find broken links and try to do something about

		//for (uint64_t idx = 0; idx < m_TextureArray.size(); idx++)
		//{

		//}

		//for (uint64_t idx = 0; idx < m_MaterialArray.size(); idx++)
		//{

		//}

		//for (uint64_t idx = 0; idx < m_MeshArray.size(); idx++)
		//{

		//}

		//for (uint64_t idx = 0; idx < m_ObjectArray.size(); idx++)
		//{

		//}

		LOG_DEBUG("RaytracerScene : \n\t- m_TextureArray.size() = %d\n\t- m_MaterialArray.size() = %d\n\t- m_MeshArray.size() = %d\n\t- m_ObjectArray.size() = %d\n",
			m_TextureArray.size(), m_MaterialArray.size(), m_MeshArray.size(), m_ObjectArray.size()
		);
	}

	void RaytracerSceneContainer::setMainCamera(uint64_t id)
	{
		m_MainCamera = getCamera(id);
	}

	std::shared_ptr<RaytracerCamera> const& RaytracerSceneContainer::getMainCamera() const
	{
		return m_MainCamera;
	}

	std::vector<std::shared_ptr<RaytracerCamera>> const& RaytracerSceneContainer::getCameraArray() const
	{
		return m_CameraArray;
	}

	std::shared_ptr<RaytracerCamera> const& RaytracerSceneContainer::getCamera(uint64_t id) const
	{
		auto elem = m_CameraDictionary.find(id);
		if (elem == m_CameraDictionary.end())
			return m_CameraArray[0];
		return (elem->second);
	}

	void RaytracerSceneContainer::addCamera(std::shared_ptr<RaytracerCamera> camera)
	{
		m_CameraArray.push_back(camera);
		m_CameraDictionary.insert({ camera->m_Id, m_CameraArray.back() });
	}

	void RaytracerSceneContainer::removeCamera(uint64_t id)
	{
	}

	std::vector<std::shared_ptr<RaytracerTexture>> const& RaytracerSceneContainer::getTextureArray() const
	{
		return m_TextureArray;
	}

	std::shared_ptr<RaytracerTexture> const& RaytracerSceneContainer::getTexture(uint64_t id) const
	{
		auto elem = m_TextureDictionary.find(id);
		if (elem == m_TextureDictionary.end())
			return m_TextureArray[0];
		return (elem->second);
	}

	void RaytracerSceneContainer::addTexture(std::shared_ptr<RaytracerTexture> texture)
	{
		m_TextureArray.push_back(texture);
		m_TextureDictionary.insert({ texture->m_Id , texture });
	}
	void RaytracerSceneContainer::removeTexture(uint64_t id)
	{

	}

	std::vector<std::shared_ptr<RaytracerMaterial>> const& RaytracerSceneContainer::getMaterialArray()
	{
		return m_MaterialArray;
	}

	std::shared_ptr<RaytracerMaterial> const& RaytracerSceneContainer::getMaterial(uint64_t id)
	{
		auto elem = m_MaterialDictionary.find(id);
		if (elem == m_MaterialDictionary.end())
			return m_MaterialArray[0];
		return (elem->second);
	}

	void RaytracerSceneContainer::addMaterial(std::shared_ptr<RaytracerMaterial> material)
	{
		m_MaterialArray.push_back(material);
		m_MaterialDictionary.insert({ material->m_Id , material });
	}
	void RaytracerSceneContainer::removeMaterial(uint64_t id)
	{

	}

	std::vector<std::shared_ptr<RaytracerMesh>> const& RaytracerSceneContainer::getMeshArray() const
	{
		return m_MeshArray;
	}

	std::shared_ptr<RaytracerMesh> const& RaytracerSceneContainer::getMesh(uint64_t id) const
	{
		auto elem = m_MeshDictionary.find(id);
		if (elem == m_MeshDictionary.end())
			return m_MeshArray[0];
		return (elem->second);
	}

	void RaytracerSceneContainer::addMesh(std::shared_ptr<RaytracerMesh> mesh)
	{
		m_MeshArray.push_back(mesh);
		m_MeshDictionary.insert({ mesh->m_Id , mesh });
	}
	void RaytracerSceneContainer::removeMesh(uint64_t id)
	{

	}

	std::vector<std::shared_ptr<RaytracerObject>> const& RaytracerSceneContainer::getObjectArray() const
	{
		return m_ObjectArray;
	}

	std::shared_ptr<RaytracerObject> const& RaytracerSceneContainer::getObject(uint64_t id) const
	{
		auto elem = m_ObjectDictionary.find(id);
		if (elem == m_ObjectDictionary.end())
			return m_ObjectArray[0];
		return (elem->second);
	}

	void RaytracerSceneContainer::addObject(std::shared_ptr<RaytracerObject> object)
	{
		m_ObjectArray.push_back(object);
		m_ObjectDictionary.insert({ object->m_Id , object});
	}
	void RaytracerSceneContainer::removeObject(uint64_t id)
	{

	}
}