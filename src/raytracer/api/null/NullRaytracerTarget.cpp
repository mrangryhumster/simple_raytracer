#include "NullRaytracerTarget.h"

API::Null::NullRaytracerTarget::NullRaytracerTarget() :
	m_Data(nullptr),
	m_Size(0),
	m_Width(0),
	m_Height(0)
{
}

API::Null::NullRaytracerTarget::~NullRaytracerTarget()
{
	Destroy();
}

void API::Null::NullRaytracerTarget::Initialize()
{

}

void API::Null::NullRaytracerTarget::Destroy()
{
	if (m_Data != nullptr)
	{
		delete[] m_Data;
		m_Data = nullptr;
	}
}

void API::Null::NullRaytracerTarget::Sync(bool trySync)
{
	//do nothing, there no need in api<->object sync
}

void API::Null::NullRaytracerTarget::Resize(uint32_t width, uint32_t height)
{
	if (m_Width != width || m_Height != height)
	{
		if (m_Data != nullptr)
			delete[] m_Data;

		m_Size = width * height * 3;
		m_Data = new uint8_t[m_Size];
		m_Width = width;
		m_Height = height;
	}
}

void API::Null::NullRaytracerTarget::Clear(color3f clearColor)
{
	memset(m_Data, 0, m_Size);
}


bool API::Null::NullRaytracerTarget::Lock(bool trySync)
{
	//do nothing, there no need in api<->object sync
	return true;
}

void API::Null::NullRaytracerTarget::Unlock()
{
	//do nothing, there no need in api<->object sync
}

uint32_t API::Null::NullRaytracerTarget::getWidth()
{
	return m_Width;
}

uint32_t API::Null::NullRaytracerTarget::getHeight()
{
	return m_Height;
}

void* API::Null::NullRaytracerTarget::getAPIObject()
{
	return m_Data; //there's no api, so api=data
}

size_t API::Null::NullRaytracerTarget::getAPIObjectSize()
{
	return m_Size; //there's no api, so api=data
}

void* API::Null::NullRaytracerTarget::getData()
{
	return m_Data;
}

size_t API::Null::NullRaytracerTarget::getDataSize()
{
	return m_Size;
}
