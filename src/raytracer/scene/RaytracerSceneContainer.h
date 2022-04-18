#pragma once

#include "RaytracerTypes.h"
#include "RaytracerLogger.h"

#include "RaytracerCamera.h"
#include "RaytracerTexture.h"
#include "RaytracerMaterial.h"
#include "RaytracerMesh.h"
#include "RaytracerObject.h"

#include <vector>
#include <list>
#include <map>

namespace Raytracer
{
	class RaytracerSceneContainer
	{
	public:
		RaytracerSceneContainer();
		~RaytracerSceneContainer();

		void Initialize();
		void Destroy();
		void MakeDefault();
		void Validate();
		//----------------------------------------------------------------
		void setMainCamera(uint64_t id);
		std::shared_ptr<RaytracerCamera> const& getMainCamera() const;
		//----------------------------------------------------------------
		std::vector<std::shared_ptr<RaytracerCamera>> const& getCameraArray() const;
		std::shared_ptr<RaytracerCamera> const& getCamera(uint64_t id) const;
		void addCamera(std::shared_ptr<RaytracerCamera> camera);
		void removeCamera(uint64_t id);
		//----------------------------------------------------------------
		std::vector<std::shared_ptr<RaytracerTexture>> const& getTextureArray() const;
		std::shared_ptr<RaytracerTexture> const& getTexture(uint64_t id) const;
		void addTexture(std::shared_ptr<RaytracerTexture> texture);
		void removeTexture(uint64_t id);
		//----------------------------------------------------------------
		std::vector<std::shared_ptr<RaytracerMaterial>> const& getMaterialArray();
		inline std::shared_ptr<RaytracerMaterial> const& getMaterial(uint64_t id);
		void addMaterial(std::shared_ptr<RaytracerMaterial> material);
		void removeMaterial(uint64_t id);
		//----------------------------------------------------------------
		std::vector<std::shared_ptr<RaytracerMesh>> const& getMeshArray() const;
		std::shared_ptr<RaytracerMesh> const& getMesh(uint64_t id) const;
		void addMesh(std::shared_ptr<RaytracerMesh> mesh);
		void removeMesh(uint64_t id);
		//----------------------------------------------------------------
		std::vector<std::shared_ptr<RaytracerObject>> const& getObjectArray() const;
		std::shared_ptr<RaytracerObject> const& getObject(uint64_t id) const;
		void addObject(std::shared_ptr<RaytracerObject> object);
		void removeObject(uint64_t id);
		//----------------------------------------------------------------
		
	private:
		//Scene configuration
		std::shared_ptr<RaytracerCamera>						m_MainCamera;
		//Containers
		std::map<uint64_t, std::shared_ptr<RaytracerCamera>>	m_CameraDictionary;
		std::vector<std::shared_ptr<RaytracerCamera>>			m_CameraArray;

		std::map<uint64_t, std::shared_ptr<RaytracerTexture>>	m_TextureDictionary;
		std::vector<std::shared_ptr<RaytracerTexture>>			m_TextureArray;

		std::map<uint64_t, std::shared_ptr<RaytracerMaterial>>	m_MaterialDictionary;
		std::vector<std::shared_ptr<RaytracerMaterial>>			m_MaterialArray;

		std::map<uint64_t, std::shared_ptr<RaytracerMesh>>		m_MeshDictionary;
		std::vector<std::shared_ptr<RaytracerMesh>>				m_MeshArray;

		std::map<uint64_t, std::shared_ptr<RaytracerObject>>	m_ObjectDictionary;
		std::vector<std::shared_ptr<RaytracerObject>>			m_ObjectArray;
	};
}