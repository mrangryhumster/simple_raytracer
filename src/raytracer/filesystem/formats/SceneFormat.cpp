#include "SceneFormat.h"

#include <fstream>
#include "ImageFormat.h"
#include "nlohmann/json.hpp"

//Some things for easy serialization/deserialization nmath types
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(nvec3<float>, x, y, z);

namespace Raytracer
{
	namespace FileSystem
	{
		using json = nlohmann::json;

		const static uint32_t SceneJsonVersionMin = 0x0001;
		const static uint32_t SceneJsonVersionMax = 0x0001;
		const static uint32_t SceneJsonVersionCurrent = SceneJsonVersionMax;

		//Declare functions
		inline static void ExportSceneCameras(json& sceneJson, RaytracerSceneContainer* scene);
		inline static void ExportSceneTextures(json& sceneJson, RaytracerSceneContainer* scene);
		inline static void ExportSceneMaterials(json& sceneJson, RaytracerSceneContainer* scene);
		inline static void ExportSceneMeshes(json& sceneJson, RaytracerSceneContainer* scene);
		inline static void ExportSceneObjects(json& sceneJson, RaytracerSceneContainer* scene);
		inline static void ExportSceneConfiguration(json& sceneJson, RaytracerSceneContainer* scene);

		inline static void ImportSceneCameras(json& sceneJson, RaytracerSceneContainer* scene);
		inline static void ImportSceneTextures(json& sceneJson, RaytracerSceneContainer* scene);
		inline static void ImportSceneMaterials(json& sceneJson, RaytracerSceneContainer* scene);
		inline static void ImportSceneMeshes(json& sceneJson, RaytracerSceneContainer* scene);
		inline static void ImportSceneObjects(json& sceneJson, RaytracerSceneContainer* scene);
		inline static void ImportSceneConfiguration(json& sceneJson, RaytracerSceneContainer* scene);

		//Lets rock...
		bool SceneFormat::Export(std::string file_path, RaytracerSceneContainer* scene)
		{
			bool exportResult = false;
			try //Json lib notifies about errors via exceptions so we have to use try/catch to capture it
			{
				LOG_LDEBUG("SceneFormat::Export(\"%s\") started\n", file_path.c_str());

				json sceneJson;
				std::ofstream file(file_path);
				if (file.is_open())
				{
					sceneJson["format"] = "internal";
					sceneJson["version"] = SceneJsonVersionCurrent;

					//Export things
					ExportSceneCameras(sceneJson, scene);
					ExportSceneTextures(sceneJson, scene);
					ExportSceneMaterials(sceneJson, scene);
					ExportSceneMeshes(sceneJson, scene);
					ExportSceneObjects(sceneJson, scene);

					ExportSceneConfiguration(sceneJson, scene);

					file << std::setw(4) << sceneJson;

					LOG_LDEBUG("SceneFormat::Export successed\n");
					exportResult = true;
				}
				else
				{
					LOG_ERROR("Unable to open file \"%s\"\n", file_path.c_str());
				}
			}
			catch (json::exception jsonException)
			{
				LOG_ERROR("Unable to export scene \"%s\" :\n\t %s\n", file_path.c_str(), jsonException.what());
			}
			catch (std::exception stdException)
			{
				LOG_ERROR("Unable to export scene \"%s\" :\n\t %s\n", file_path.c_str(), stdException.what());
			}
			return exportResult;
		}

		bool SceneFormat::Import(std::string file_path, RaytracerSceneContainer* scene)
		{
			bool importResult = false;
			try //Json lib notifies about errors via exceptions so we have to use try/catch to capture it
			{
				LOG_LDEBUG("SceneFormat::Import(\"%s\") started\n", file_path.c_str());

				json sceneJson;
				std::ifstream file(file_path);
				if (file.is_open())
				{
					file >> sceneJson;

					std::string format = sceneJson.at("format").get<std::string>();
					if (format == "internal") 
					{
						//Load data using 'internal' scheme 
						uint32_t version = sceneJson.at("version").get<uint32_t>();
						if (version >= SceneJsonVersionMin && version <= SceneJsonVersionMax)
						{
							//Clear scene before loading, tmp solution to id conflicts
							scene->MakeDefault();

							//Import things 
							ImportSceneCameras(sceneJson, scene);
							ImportSceneTextures(sceneJson, scene);
							ImportSceneMaterials(sceneJson, scene);
							ImportSceneMeshes(sceneJson, scene);
							ImportSceneObjects(sceneJson, scene);

							ImportSceneConfiguration(sceneJson, scene);

							LOG_LDEBUG("SceneFormat::Import successed\n");
							importResult = true;
						}
						else
						{
							LOG_ERROR("Unable to import scene \"%s\" :\n\t Version missmatch, expected between \'0x%x\' and \'0x%x\' - got 0x%x",
								file_path.c_str(), SceneJsonVersionMin, SceneJsonVersionMax, version);
							importResult = false;
						}
					}
					else
					{
						LOG_ERROR("Uknown scene representation format \"%s\" in file \"%s\"\n", format.c_str() , file_path.c_str());
					}
				}
				else
				{
					LOG_ERROR("Unable to open file \"%s\"\n", file_path.c_str());
				}
			}
			catch (json::exception jsonException)
			{
				LOG_ERROR("Unable to import scene \"%s\" :\n\t %s\n", file_path.c_str(), jsonException.what());
			}
			catch (std::exception stdException)
			{
				LOG_ERROR("Unable to import scene \"%s\" :\n\t %s\n", file_path.c_str(), stdException.what());
			}
			return importResult;
		}

		//****************************************************************

		inline static void ExportSceneCameras(json& sceneJson, RaytracerSceneContainer* scene)
		{
			auto array = scene->getCameraArray();
			for (size_t idx = 1; idx < array.size(); idx++)
			{
				json node;
				auto camera = array[idx].get();

				node["id"] = camera->m_Id;
				node["position"] = camera->m_Position;
				node["target"] = camera->m_Target;
				node["up"] = camera->m_Up;
				node["fov"] = camera->m_FOV;
				node["use_depth_of_field"] = camera->m_UseDepthOfDield;

				sceneJson["descriptors"]["cameras"].push_back(node);
			}
			LOG_LDEBUG("Exported %d cameras\n", sceneJson["descriptors"]["cameras"].size());
		}

		inline static void ExportSceneTextures(json& sceneJson, RaytracerSceneContainer* scene)
		{
			LOG_LDEBUG("Exported %d textures\n", sceneJson["descriptors"]["textures"].size());
		}

		inline static void ExportSceneMaterials(json& sceneJson, RaytracerSceneContainer* scene)
		{
			//This macro will reduce material importing code
			#define MATERIAL_SAVE_FIELD(JSON_TOKEN,FIELD_TOKEN)																		\
			if (material->m_Is##FIELD_TOKEN##Texture) materialJson[JSON_TOKEN"_texture"] = material->m_##FIELD_TOKEN##.TextureId;	\
			else materialJson[JSON_TOKEN] = material->m_##FIELD_TOKEN##.Value;					\
			//-----------------------------------------------
			auto array = scene->getMaterialArray();
			for (size_t idx = 1; idx < array.size(); idx++)
			{
				json materialJson;
				auto material = array[idx].get();

				materialJson["id"] = material->m_Id;
				materialJson["type"] = "default";

				//Write material data
				MATERIAL_SAVE_FIELD("albedo",				Albedo);
				MATERIAL_SAVE_FIELD("metallic",				Metallic);
				MATERIAL_SAVE_FIELD("roughness",			Roughness);
				MATERIAL_SAVE_FIELD("ior",					IOR);
				MATERIAL_SAVE_FIELD("transmission",			Transmission);
				MATERIAL_SAVE_FIELD("emission_strength",	EmissionStrength);
				MATERIAL_SAVE_FIELD("emission_color",		EmissionColor);
				//----------------

				sceneJson["descriptors"]["materials"].push_back(materialJson);
			}
			//-----------------------------------------------
			#undef MATERIAL_SAVE_FIELD
			LOG_LDEBUG("Exported %d materials\n", sceneJson["descriptors"]["materials"].size());
		}

		inline static void ExportSceneMeshes(json& sceneJson, RaytracerSceneContainer* scene)
		{
			auto array = scene->getMeshArray();
			for (size_t idx = 1; idx < array.size(); idx++)
			{
				json node;
				auto mesh = array[idx].get();

				node["id"] = mesh->m_Id;
				switch (mesh->m_Type)
				{
				case RaytracerMeshType::Sphere:
					node["type"] = "sphere";
					node["radius"] = static_cast<RaytracerMeshSphere*>(mesh)->m_Radius;
					break;
				default:
					break;
				}

				sceneJson["descriptors"]["meshes"].push_back(node);
			}
			LOG_LDEBUG("Exported %d meshes\n", sceneJson["descriptors"]["meshes"].size());
		}

		inline static void ExportSceneObjects(json& sceneJson, RaytracerSceneContainer* scene)
		{
			auto array = scene->getObjectArray();
			for (size_t idx = 1; idx < array.size(); idx++)
			{
				json node;
				auto object = array[idx].get();

				node["id"] = object->m_Id;
				node["mesh_id"] = object->m_MeshId;
				node["material_id"] = object->m_MaterialId;
				node["position"] = object->m_Position;

				sceneJson["descriptors"]["objects"].push_back(node);
			}
			LOG_LDEBUG("Exported %d objects\n", sceneJson["descriptors"]["objects"].size());

		}

		inline static void ExportSceneConfiguration(json& sceneJson, RaytracerSceneContainer* scene)
		{
			sceneJson["configuration"]["main_camera"] = scene->getMainCamera()->m_Id;
		}

		//****************************************************************
		inline static void ImportSceneCameras(json& sceneJson, RaytracerSceneContainer* scene)
		{
			uint32_t version = sceneJson["version"].get<uint32_t>();
			if (version == SceneJsonVersionCurrent)
			{
				auto jsonArray = sceneJson.at("descriptors").at("cameras");
				LOG_LDEBUG("Scene json contains %d cameras\n", jsonArray.size());
				for (size_t idx = 0; idx < jsonArray.size(); idx++)
				{
					auto cameraJson = jsonArray.at(idx);
					auto camera = RaytracerCamera::Create(cameraJson.at("id").get<uint64_t>());
					camera->m_Position = cameraJson.at("position").get<vec3>();
					camera->m_Target = cameraJson.at("target").get<vec3>();
					camera->m_Up = cameraJson.at("up").get<vec3>();
					camera->m_FOV = cameraJson.at("fov").get<float>();
					camera->m_UseDepthOfDield = cameraJson.at("use_depth_of_field").get<bool>();
					scene->addCamera(camera);
				}
			}
		}

		inline static void ImportSceneTextures(json& sceneJson, RaytracerSceneContainer* scene)
		{
			uint32_t version = sceneJson["version"].get<uint32_t>();
			if (version == SceneJsonVersionCurrent)
			{
				auto jsonArray = sceneJson.at("descriptors").at("textures");
				LOG_LDEBUG("Scene json contains %d textures\n", jsonArray.size());
				for (size_t idx = 0; idx < jsonArray.size(); idx++)
				{
					auto textureJson = jsonArray.at(idx);

					auto srcString = textureJson.at("source").get<std::string>();
					if (srcString == "file")
					{
						srcString = textureJson.at("file_path").get<std::string>();

						auto texture = RaytracerTexture::Create(textureJson.at("id").get<uint64_t>());

						bool loadResult = 
							ImageFormat::Import(
							srcString.c_str(),
							texture->m_Width,
							texture->m_Height,
							texture->m_Components,
							texture->m_Data,
							texture->m_DataSize
						);

						if (loadResult == false)
						{
							LOG_ERROR("Failed to load texture '%s'\n",srcString.c_str());
						}

						scene->addTexture(texture);
					}
					else
					{
						LOG_ERROR("Texture descriptor contain unknown source\n");
					}
				}
			}
		}

		inline static void ImportSceneMaterials(json& sceneJson, RaytracerSceneContainer* scene)
		{
			//This macro will reduce material importing code
			#define MATERIAL_LOAD_FIELD(JSON_TOKEN,FIELD_TOKEN,FIELD_TYPE)																					\
			material->m_Is##FIELD_TOKEN##Texture = materialJson.contains(JSON_TOKEN "_texture");															\
			if (material->m_Is##FIELD_TOKEN##Texture) material->m_##FIELD_TOKEN##.TextureId = materialJson.at(JSON_TOKEN"_texture").get<uint64_t>();		\
			else if(materialJson.contains(JSON_TOKEN)) material->m_##FIELD_TOKEN##.Value = materialJson.at(JSON_TOKEN).get<FIELD_TYPE>();					\
			//-----------------------------------------------
			uint32_t version = sceneJson["version"].get<uint32_t>();
			if (version == SceneJsonVersionCurrent)
			{
				auto jsonArray = sceneJson.at("descriptors").at("materials");
				LOG_LDEBUG("Scene json contains %d materials\n", jsonArray.size());
				for (size_t idx = 0; idx < jsonArray.size(); idx++)
				{
					auto materialJson = jsonArray.at(idx);
					auto material = RaytracerMaterial::Create(materialJson.at("id").get<uint64_t>());

					//Read material data
					MATERIAL_LOAD_FIELD("albedo",			Albedo,				nvec3<float>);
					MATERIAL_LOAD_FIELD("metallic",			Metallic,			float);
					MATERIAL_LOAD_FIELD("roughness",		Roughness,			float);
					MATERIAL_LOAD_FIELD("ior",				IOR,				float);
					MATERIAL_LOAD_FIELD("transmission",		Transmission,		float);
					MATERIAL_LOAD_FIELD("emission_strength",EmissionStrength,	float);
					MATERIAL_LOAD_FIELD("emission_color",	EmissionColor,		nvec3<float>);
					//----------------

					scene->addMaterial(material);
				}
			}

			#undef MATERIAL_LOAD_FIELD
		}

		inline static void ImportSceneMeshes(json& sceneJson, RaytracerSceneContainer* scene)
		{
			uint32_t version = sceneJson["version"].get<uint32_t>();
			if (version == SceneJsonVersionCurrent)
			{
				auto jsonArray = sceneJson.at("descriptors").at("meshes");
				LOG_LDEBUG("Scene json contains %d meshes\n", jsonArray.size());
				for (size_t idx = 0; idx < jsonArray.size(); idx++)
				{
					auto meshJson = jsonArray.at(idx);
					if (meshJson.at("type") == "sphere")
					{
						auto meshSphere = RaytracerMeshSphere::Create(meshJson.at("id").get<uint64_t>());
						meshSphere->m_Radius = meshJson.at("radius").get<float>();
						scene->addMesh(meshSphere);
					}
				}
			}
		}

		inline static void ImportSceneObjects(json& sceneJson, RaytracerSceneContainer* scene)
		{
			
			uint32_t version = sceneJson["version"].get<uint32_t>();
			if (version == SceneJsonVersionCurrent)
			{
				auto jsonArray = sceneJson.at("descriptors").at("objects");
				LOG_LDEBUG("Scene json contains %d objects\n", jsonArray.size());
				for (size_t idx = 0; idx < jsonArray.size(); idx++)
				{
					auto objectJson = jsonArray.at(idx);
					auto object = RaytracerObject::Create(objectJson.at("id").get<uint64_t>());
					object->m_MeshId = objectJson.at("mesh_id").get<uint32_t>();
					object->m_MaterialId = objectJson.at("material_id").get<uint32_t>();
					object->m_Position = objectJson.at("position").get<nvec3<float>>();
					scene->addObject(object);
				}
			}
		}

		inline static void ImportSceneConfiguration(json& sceneJson, RaytracerSceneContainer* scene)
		{
			uint32_t version = sceneJson["version"].get<uint32_t>();
			if (version == SceneJsonVersionCurrent)
			{
				auto configurationJson = sceneJson.at("configuration");
				if (configurationJson.contains("main_camera"))
					scene->setMainCamera(configurationJson.at("main_camera").get<uint64_t>());
			}
		}

		//****************************************************************
	}
}