#pragma once

#include <cstdint>
#include <string>

#include "RaytracerTypes.h"

namespace Raytracer
{
	class RaytracerTargetInterface
	{
	public:
		virtual ~RaytracerTargetInterface() = default;

		virtual void Initialize() = 0;
		virtual void Destroy() = 0;

		//Update API object from Data
		//	arg : tryLock
		//		false = if object is locked then wait until object will be released
		//		true = if object is locked then skip sync operation
		virtual void Sync(bool trySync = true) = 0;
		
		//Changing size affecting api object and data object
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		//Simply clear internal pixelarray to color (api object will be not affected until sync called, most of the time...)
		virtual void Clear(color3f clearColor) = 0;

		//Do nothing if used NULL api otherwise lock/unlock mutex for data object
		//	arg : trySync
		//		false = if object is already locked then wait until object will be released
		//		true = if object is already locked then skip and dont lock
		//	ret : returns true if object succefully locked	
		virtual bool Lock(bool trySync = false) = 0;
		//Be careful - do not try unlock object if YOU didnt lock it
		virtual void Unlock() = 0; 

		virtual uint32_t getWidth() = 0;
		virtual uint32_t getHeight() = 0;

		//Returns api object
		//	for example, if we using opengl we will return GLuint, so you sould use reinterpret_cast<GLuint>(x->getAPIObject) to recover value
		virtual void* getAPIObject() = 0;
		virtual size_t getAPIObjectSize() = 0;

		//Returns data object
		//	Always return ptr to pixel array
		virtual void* getData() = 0;
		virtual size_t getDataSize() = 0;
	};
}