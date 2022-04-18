#include "ConsoleApplication.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>

int ConsoleApplication::Run(int argc, char** argv)
{
	for (size_t argi = 0; argi < argc; argi++)
	{
		if (std::strcmp(argv[argi], "--debug") == 0)
		{
			std::cout << "Debug mode is selected" << std::endl;
			return ProfilerRun(argc, argv);
		}
	}
	return NormalRun(argc,argv);
}

int ConsoleApplication::NormalRun(int argc, char** argv)
{
	Raytracer::RaytracerInterface*			m_Raytracer;
	Raytracer::RaytracerTargetInterface*	m_RaytracerTarget;
	Raytracer::RaytracerSceneContainer*		m_RaytracerScene;
	Raytracer::RaytracerConfig				m_RaytracerConfig;

	uint32_t m_RaytracerTargetWidth, m_RaytracerTargetHeight;

	//Create raytracer
	m_Raytracer = new Raytracer::Software::SoftwareRaytracer();
	m_Raytracer->Initialzie();

	//Create raytracer target 
	//(allways null api, because console doesn't have any output except text =) )
	m_RaytracerTarget = new API::Null::NullRaytracerTarget();
	m_RaytracerTarget->Initialize();

	//Create scene container
	m_RaytracerScene = new Raytracer::RaytracerSceneContainer();
	m_RaytracerScene->Initialize();

	//Load scene container
	Raytracer::FileSystem::SceneFormat::Import(
		"scene.json",
		m_RaytracerScene
	);

	//Default settings
	m_RaytracerTargetWidth = CONSOLEAPP_DEFAULT_RAYTRACING_TARGET_WIDTH;
	m_RaytracerTargetHeight = CONSOLEAPP_DEFAULT_RAYTRACING_TARGET_HEIGHT;

	m_RaytracerConfig.m_SamplesPerPixel = CONSOLEAPP_DEFAULT_SAMPLES;
	m_RaytracerConfig.m_RayMaxBounces = CONSOLEAPP_DEFAULT_BOUNCES;
	m_RaytracerConfig.m_TileWidth = CONSOLEAPP_DEFAULT_RAYTRACING_TILE_WIDTH;
	m_RaytracerConfig.m_TileHeight = CONSOLEAPP_DEFAULT_RAYTRACING_TILE_HEIGHT;
	m_RaytracerConfig.m_Threads = CONSOLEAPP_DEFAULT_THREADS;

	m_RaytracerTarget->Resize(m_RaytracerTargetWidth, m_RaytracerTargetHeight);
	m_Raytracer->StartRaytracing(m_RaytracerTarget, nullptr, m_RaytracerConfig);

	uint32_t regionsQueued = 0;
	do
	{
		m_Raytracer->Update(false); //dont update target pixels

		auto rendererState = m_Raytracer->getState();
		if (rendererState->m_Stage == Raytracer::RaytracerStage::Stage::Rendering)
		{
			auto raytracingRegions = reinterpret_cast<const Raytracer::RaytracerRenderingRegionsStage*>(rendererState);
			if (regionsQueued != raytracingRegions->m_RegionsQueued)
			{
				regionsQueued = raytracingRegions->m_RegionsQueued;
				std::cout << "Rendering : " << raytracingRegions->m_RegionsCount - regionsQueued << "/" << raytracingRegions->m_RegionsCount << " tasks processed\r" << std::flush;
			}
		}

	} while (m_Raytracer->IsProcessing());
	std::cout << std::endl << "Rendering finished in " << std::setprecision(4) << m_Raytracer->getPerformanceInfo().m_StageTime.m_TotalTime << " s." << std::endl;


	m_Raytracer->Update(); //final update to load pixels into target
	Raytracer::FileSystem::ImageFormat::Export(
		"console_output.png",
		Raytracer::FileSystem::ImageFormatType::PNG,
		m_RaytracerTarget->getWidth(),
		m_RaytracerTarget->getHeight(),
		m_RaytracerTarget->getData(),
		m_RaytracerTarget->getDataSize()
	);

	//Cleanup
	m_RaytracerTarget->Destroy();
	delete m_RaytracerTarget;

	m_Raytracer->Destroy();
	delete m_Raytracer;
	
	return 0;
}

int ConsoleApplication::ProfilerRun(int argc, char** argv)
{
	bool argvCorrect = true;
	uint32_t loopsCount = 8; //By default 8 runs for debugprofile mode
	
	uint32_t width = 1280;
	uint32_t height = 720;
	uint32_t samples = 8;
	uint32_t bounces = 8;
	uint32_t threads = 4;
	uint32_t hierarchy = 0;

	std::string scenefile = "scene.json";
	std::string outputfile = "output_";
	std::string outputfile_postfix = "";
	//Very crude args checker for debug/profile propources
	for (size_t argi = 0; argi < argc; argi++)
	{
		if (std::strcmp(argv[argi], "--loops") == 0)
		{
			if (argi + 1 < argc)
			{
				std::istringstream iss(argv[argi + 1]);
				iss >> loopsCount;
				outputfile_postfix.append("l");
				outputfile_postfix.append(argv[argi + 1]);
				std::cout << " - loop count set to " << loopsCount << std::endl;
				argi++;
			}
			else
			{
				std::cout << "Not enough arguments!" << std::endl;
				argvCorrect = false;
			}
		}
		else if (std::strcmp(argv[argi], "--width") == 0)
		{
			if (argi + 1 < argc)
			{
				std::istringstream iss(argv[argi + 1]);
				iss >> width;
				outputfile_postfix.append("w");
				outputfile_postfix.append(argv[argi + 1]);
				std::cout << " - target width set to " << width << std::endl;
				argi++;
			}
			else
			{
				std::cout << "Not enough arguments!" << std::endl;
				argvCorrect = false;
			}
		}
		else if (std::strcmp(argv[argi], "--height") == 0)
		{
			if (argi + 1 < argc)
			{
				std::istringstream iss(argv[argi + 1]);
				iss >> height;
				outputfile_postfix.append("h");
				outputfile_postfix.append(argv[argi + 1]);
				std::cout << " - target height set to " << height << std::endl;
				argi++;
			}
			else
			{
				std::cout << "Not enough arguments!" << std::endl;
				argvCorrect = false;
			}
		}
		else if (std::strcmp(argv[argi], "--samples") == 0)
		{
			if (argi + 1 < argc)
			{
				std::istringstream iss(argv[argi + 1]);
				iss >> samples;
				outputfile_postfix.append("s");
				outputfile_postfix.append(argv[argi + 1]);
				std::cout << " - sample count set to " << samples << std::endl;
				argi++;
			}
			else
			{
				std::cout << "Not enough arguments!" << std::endl;
				argvCorrect = false;
			}
		}
		else if (std::strcmp(argv[argi], "--bounces") == 0)
		{
			if (argi + 1 < argc)
			{
				std::istringstream iss(argv[argi + 1]);
				iss >> bounces;
				outputfile_postfix.append("b");
				outputfile_postfix.append(argv[argi + 1]);
				std::cout << " - bounce count set to " << bounces << std::endl;
				argi++;
			}
			else
			{
				std::cout << "Not enough arguments!" << std::endl;
				argvCorrect = false;
			}
		}
		else if (std::strcmp(argv[argi], "--threads") == 0)
		{
			if (argi + 1 < argc)
			{
				std::istringstream iss(argv[argi + 1]);
				iss >> threads;
				outputfile_postfix.append("t");
				outputfile_postfix.append(argv[argi + 1]);
				std::cout << " - threads count set to " << threads << std::endl;
				argi++;
			}
			else
			{
				std::cout << "Not enough arguments!" << std::endl;
				argvCorrect = false;
			}
		}
		else if (std::strcmp(argv[argi], "--hierarchy") == 0)
		{
			if (argi + 1 < argc)
			{
				std::istringstream iss(argv[argi + 1]);
				iss >> hierarchy;
				std::cout << " - hierarchy type set to " << hierarchy << std::endl;
				argi++;
			}
			else
			{
				std::cout << "Not enough arguments!" << std::endl;
				argvCorrect = false;
			}
		}
		else if (std::strcmp(argv[argi], "--out") == 0)
		{
			if (argi + 1 < argc)
			{
				std::istringstream iss(argv[argi + 1]);
				iss >> outputfile;
				std::cout << " - out count set to " << outputfile << std::endl;
				argi++;
			}
			else
			{
				std::cout << "Not enough arguments!" << std::endl;
				argvCorrect = false;
			}
		}
	}
	outputfile += outputfile_postfix;
	//------------------------------- 

	if(argvCorrect)
	{
		Raytracer::Logger::Init(0xffffffff); //Full logs;

		Raytracer::RaytracerInterface*			m_Raytracer;
		Raytracer::RaytracerTargetInterface*	m_RaytracerTarget;
		Raytracer::RaytracerSceneContainer*		m_RaytracerScene;
		Raytracer::RaytracerConfig				m_RaytracerConfig;

		//Create raytracer
		m_Raytracer = new Raytracer::Software::SoftwareRaytracer();
		m_Raytracer->Initialzie();

		//Create raytracer target 
		//(allways null api, because console doesn't have any output except text =) )
		m_RaytracerTarget = new API::Null::NullRaytracerTarget();
		m_RaytracerTarget->Initialize();

		//Create scene container
		m_RaytracerScene = new Raytracer::RaytracerSceneContainer();
		m_RaytracerScene->Initialize();

		//Load scene container
		Raytracer::FileSystem::SceneFormat::Import(
			scenefile,
			m_RaytracerScene
		);

		//Default settings for target
		m_RaytracerTarget->Resize(width, height);

		//Default settings for rt
		m_RaytracerConfig.m_SamplesPerPixel = samples;
		m_RaytracerConfig.m_RayMaxBounces = bounces;
		m_RaytracerConfig.m_TileWidth = 64;
		m_RaytracerConfig.m_TileHeight = 64;
		m_RaytracerConfig.m_Threads = threads;
		m_RaytracerConfig.m_Internal_SoftwareBVHType = hierarchy;

		float		timeAccumPrep = 0.f;
		float		timeAccumRegs = 0.f;
		float		timeAccumClup = 0.f;
		float		timeAccumFull = 0.f;

		std::ofstream logStream(outputfile + ".log");
		if (logStream.is_open())
		{
			std::cout << std::endl;
			for (uint32_t i = 0; i < loopsCount; i++)
			{
				m_Raytracer->StartRaytracing(m_RaytracerTarget, m_RaytracerScene, m_RaytracerConfig);
				do
				{
					m_Raytracer->Update(false); //dont update target pixels
				} while (m_Raytracer->IsProcessing());

				auto& perfInfo = m_Raytracer->getPerformanceInfo();
				timeAccumPrep += perfInfo.m_StageTime.m_SynchronizationTime;
				timeAccumRegs += perfInfo.m_StageTime.m_RenderingTime;
				timeAccumClup += perfInfo.m_StageTime.m_FinalizationTime;
				timeAccumFull += perfInfo.m_StageTime.m_TotalTime;
				std::cout << "Step #" << i << " finished in " << std::setprecision(4) << perfInfo.m_StageTime.m_TotalTime << " sec." << std::endl;
				logStream << "Step #" << i << " finished in " << std::setprecision(4) << perfInfo.m_StageTime.m_TotalTime << " sec." << std::endl;
			}
			std::cout << std::endl;
			std::cout << "DebugProfile run statistics for " << loopsCount << " runs : " << std::endl;
			std::cout << "\tPrepTime = " << std::setprecision(4) << timeAccumPrep / loopsCount << std::endl;
			std::cout << "\tRegsTime = " << std::setprecision(4) << timeAccumRegs / loopsCount << std::endl;
			std::cout << "\tClupTime = " << std::setprecision(4) << timeAccumClup / loopsCount << std::endl;
			std::cout << "\tFullTime = " << std::setprecision(4) << timeAccumFull / loopsCount << std::endl;
			std::cout << std::endl;

			logStream << std::endl;
			logStream << "DebugProfile run statistics for " << loopsCount << " runs : " << std::endl;
			logStream << "\tPrepTime = " << std::setprecision(4) << timeAccumPrep / loopsCount << std::endl;
			logStream << "\tRegsTime = " << std::setprecision(4) << timeAccumRegs / loopsCount << std::endl;
			logStream << "\tClupTime = " << std::setprecision(4) << timeAccumClup / loopsCount << std::endl;
			logStream << "\tFullTime = " << std::setprecision(4) << timeAccumFull / loopsCount << std::endl;
			logStream << std::endl;

			m_Raytracer->Update(); //final update to load pixels into target
			Raytracer::FileSystem::ImageFormat::Export(
				"console_output.png",
				Raytracer::FileSystem::ImageFormatType::PNG,
				m_RaytracerTarget->getWidth(),
				m_RaytracerTarget->getHeight(),
				m_RaytracerTarget->getData(),
				m_RaytracerTarget->getDataSize()
			);
			logStream.close();
		}
		else
		{
			std::cout << "Unable to create log file " << (outputfile + ".log") << std::endl;
		}

		//Cleanup
		m_RaytracerTarget->Destroy();
		delete m_RaytracerTarget;

		m_Raytracer->Destroy();
		delete m_Raytracer;

		return 0;
	}
	return 1;
}
