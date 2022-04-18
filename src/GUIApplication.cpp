#include "GUIApplication.h"

int GUIApplication::Run(int argc, char** argv)
{
    int code = 0;
    //------------------------
    code = this->Initialize();
    //------------------------
    if(code == 0)
        code = this->MainLoop();
    //------------------------
    this->Destroy();
    //------------------------
    return 0;
}

int GUIApplication::Initialize()
{
    Raytracer::Logger::Init(0xFFFFFFFF); //Log everything
    //------------------------------------------------------------------------------------------------------------
    // 	glfw, gl3w and Dear ImGui initialization and basic setup 
    //------------------------------------------------------------------------------------------------------------
    const char* error_description;
    //------------------------------------------------------
    if (!glfwInit())
    {
        glfwGetError(&error_description);
        LOG_FATAL("glfw error: %s\n", error_description);
        return 1;
    }
    //------------------------------------------------------
    m_glfwWindow = glfwCreateWindow(GUIAPP_WINDOW_SIZE_WIDTH, GUIAPP_WINDOW_SIZE_HEIGHT, "MainWindow", NULL, NULL);
    if (!m_glfwWindow)
    {
        glfwGetError(&error_description);
        LOG_FATAL("glfw error: %s\n", error_description);
        glfwTerminate();
        return 1;
    }
    //------------------------------------------------------
    glfwMakeContextCurrent(m_glfwWindow);
    //------------------------------------------------------
    if (gl3wInit())
    {
        LOG_FATAL("gl3w error...\n");
        return 2;
    }
    //------------------------------------------------------
    glfwSwapInterval(1);
    //------------------------------------------------------
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //------------------------------------------------------
    ImGui::StyleColorsDark();
    //------------------------------------------------------
    ImGui_ImplGlfw_InitForOpenGL(m_glfwWindow, false);
    ImGui_ImplGlfw_InstallCallbacks(m_glfwWindow);
    ImGui_ImplOpenGL3_Init("#version 140");
    //------------------------------------------------------
    ImGuiIO& guiIO = ImGui::GetIO();
    guiIO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    guiIO.ConfigDockingWithShift = true;
    //------------------------------------------------------
    glfwSetWindowSizeCallback(m_glfwWindow, resize_window_callback);
    //------------------------------------------------------
    m_RaytracerTarget = new API::OpenGL::OpenGLRaytracerTarget();
    m_RaytracerTarget->Initialize();
    //------------------------------------------------------
    m_Raytracer = new Raytracer::Software::SoftwareRaytracer();
    m_Raytracer->Initialzie();
    //------------------------------------------------------
    //Default settings
    m_RaytracerTargetWidth = GUIAPP_DEFAULT_RAYTRACING_TARGET_WIDTH;
    m_RaytracerTargetHeight = GUIAPP_DEFAULT_RAYTRACING_TARGET_HEIGHT;
    
    m_RaytracerConfig.m_SamplesPerPixel = GUIAPP_DEFAULT_SAMPLES;
    m_RaytracerConfig.m_RayMaxBounces = GUIAPP_DEFAULT_BOUNCES;
    m_RaytracerConfig.m_TileWidth = GUIAPP_DEFAULT_RAYTRACING_TILE_WIDTH;
    m_RaytracerConfig.m_TileHeight = GUIAPP_DEFAULT_RAYTRACING_TILE_HEIGHT;
    m_RaytracerConfig.m_Threads = GUIAPP_DEFAULT_THREADS;
    //------------------------------------------------------
    m_RaytracerScene = new Raytracer::RaytracerSceneContainer();
    m_RaytracerScene->Initialize();
    //------------------------------------------------------
    //RenderTab setup
    m_RenderTab_Scale = 1.0f;
    m_RenderTab_Offset = ImVec2(0, 0);
    //------------------------------------------------------
    return 0;
}

void GUIApplication::Destroy()
{
    delete m_RaytracerScene;
    delete m_RaytracerTarget;
    delete m_Raytracer;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    glfwTerminate();
}

int GUIApplication::MainLoop()
{
    int WindowSizeX, WindowSizeY;
    bool isRunning = true;
    while (isRunning)
    {
        glfwPollEvents();
        //-----------------------------------------------------------------------
        if (glfwWindowShouldClose(m_glfwWindow) ||
            glfwGetKey(m_glfwWindow, GLFW_KEY_ESCAPE))
        {
            break;
        }
        //-----------------------------------------------------------------------

        // *** Event processing things goes here *** // 

        //-----------------------------------------------------------------------

        // *** Logic processing things goes here *** // 
        m_Raytracer->Update();

        //-----------------------------------------------------------------------

        glfwGetFramebufferSize(m_glfwWindow, &WindowSizeX, &WindowSizeY);
        glViewport(0, 0, WindowSizeX, WindowSizeY);
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //-----------------------------------------------------------------------

        // *** OpenGL things goes here *** // 
        //-----------------------------------------------------------------------
        this->DrawGui();
        //-----------------------------------------------------------------------
        glfwSwapBuffers(m_glfwWindow);
    }

    //Stopping raytacer if it are in hot, and wait untill it's actually stop
    if (m_Raytracer->IsProcessing())
    {
        m_Raytracer->StopRaytracing();

        while (m_Raytracer->IsProcessing())
        {
            m_Raytracer->Update();
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(100ms);
        }
    }

    glfwTerminate();
    return 0;
}

void GUIApplication::DrawGui()
{
    ImGuiIO& ImGuiIO = ImGui::GetIO();
    //-----------------------------------------------------------------------
    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
    //-----------------------------------------------------------------------
    static bool showWindowMainAsDetached = false;
    static bool showWindowRendrerConfig = false;
    static bool showWindowRendrerControl = false;
    static bool showWindowImguiDebug = false;
    //*****************************************************
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Menu"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Scene"))
        {
            if (ImGui::MenuItem("Load \"scene.json\""))
            {
                bool initial = false;
                if(!initial)
                    initial = Raytracer::FileSystem::SceneFormat::Import(
                    "scene.json",
                    m_RaytracerScene
                );
                if (!initial)
                    initial = Raytracer::FileSystem::SceneFormat::Import(
                    "../../../resources/SceneRTIOW.json",
                    m_RaytracerScene
                );

                
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Render"))
        {
            if (!m_Raytracer->IsProcessing())
            {
                if (ImGui::MenuItem("Render Image"))
                {
                    m_RaytracerScene->Validate();
                    m_RaytracerTarget->Resize(m_RaytracerTargetWidth, m_RaytracerTargetHeight);
                    m_RaytracerTarget->Clear(color3f(0.0f,0.0f,0.4f));
                    m_Raytracer->StartRaytracing(m_RaytracerTarget, m_RaytracerScene, m_RaytracerConfig);
                    m_RenderTab_Scale = 1.0f;
                    m_RenderTab_Offset = ImVec2(0, 0);
                }
            }
            else
            {
                if (ImGui::MenuItem("Stop Rendering"))
                {
                    m_Raytracer->StopRaytracing();
                }
            }

            if (ImGui::MenuItem("Save \"output.png\""))
            {
                Raytracer::FileSystem::ImageFormat::Export(
                    "output.png",
                    Raytracer::FileSystem::ImageFormatType::PNG,
                    m_RaytracerTarget->getWidth(),
                    m_RaytracerTarget->getHeight(),
                    m_RaytracerTarget->getData(),
                    m_RaytracerTarget->getDataSize()
                );
            }

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Windows"))
        {
            ImGui::MenuItem("Detach main window", nullptr, &showWindowMainAsDetached);
            ImGui::Separator();
            ImGui::MenuItem("Rendrer Configuration", nullptr, &showWindowRendrerConfig);
            //ImGui::MenuItem("Rendrer Controls", nullptr, &showWindowRendrerControl);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Debug"))
        {
            ImGui::MenuItem("ImGUI demo window", nullptr, &showWindowImguiDebug);

            if (ImGui::MenuItem("Dump scene"))
            {
                
                Raytracer::FileSystem::SceneFormat::Export(
                    "scene_dump.json",
                    m_RaytracerScene
                );
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    
    //ImGui::GetIO().add
    //*****************************************************
    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    const ImGuiWindowFlags mainWindowFlags =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoScrollWithMouse |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove;

    const ImGuiWindowFlags mainWindowChildsFlags = ImGuiWindowFlags_NoCollapse;
    //*****************************************************
    if (!showWindowMainAsDetached)
    {
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
    }
    if (ImGui::Begin("Main", nullptr, (!showWindowMainAsDetached) ? mainWindowFlags : 0))
    {
        if (ImGui::BeginTabBar("MainTabBar"))
        {
            /*
            if (ImGui::BeginTabItem("ViewportTab"))
            {
                if (ImGui::BeginChild("ViewportWindow"))
                {
                    ImGui::Text("Sorry, not available!");
                    ImGui::EndChild();
                } // ViewportWindow
                ImGui::EndTabItem();
            } // ViewportTab
            */

            if (ImGui::BeginTabItem("RenderTab"))
            {
                if (ImGui::BeginChild("RenderWindow",ImVec2(0,0),false, mainWindowFlags))
                {
                    DrawRenderTab();
                    ImGui::EndChild();
                } // RenderWindow
                ImGui::EndTabItem();
            } // RenderTab
            ImGui::EndTabBar();
        } // MainTabBar
        ImGui::End();
    } // Main
    //*****************************************************
    
    if (showWindowRendrerConfig)
    {
        const bool isRaytracing = m_Raytracer->IsProcessing();
        ImGui::SetNextWindowSizeConstraints(ImVec2(400, 400), ImVec2(400, 400));
        if (ImGui::Begin("Configuration", &showWindowRendrerConfig, mainWindowChildsFlags))
        {
            if (isRaytracing) ImGui::Text("Controls disabled - raytracing in process...");
            if (ImGui::CollapsingHeader("Target", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::Indent(32.f);
                ImGui::BeginDisabled(isRaytracing);
                ImGui::DragInt("Target Width", reinterpret_cast<int*>(&m_RaytracerTargetWidth), 4.0f, GUIAPP_MIN_RAYTRACING_TARGET_WIDTH, GUIAPP_MAX_RAYTRACING_TARGET_WIDTH);
                ImGui::DragInt("Target Height", reinterpret_cast<int*>(&m_RaytracerTargetHeight), 4.0f, GUIAPP_MIN_RAYTRACING_TARGET_HEIGHT, GUIAPP_MAX_RAYTRACING_TARGET_HEIGHT);
                ImGui::DragInt("Samples", reinterpret_cast<int*>(&m_RaytracerConfig.m_SamplesPerPixel), 1.0f, 1, GUIAPP_MAX_SAMPLES);
                ImGui::EndDisabled();
                ImGui::Indent(-32.f);
            }
            if (ImGui::CollapsingHeader("Ray bounces", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::Indent(32.f);
                ImGui::BeginDisabled(isRaytracing);
                ImGui::DragInt("Max Bounces", reinterpret_cast<int*>(&m_RaytracerConfig.m_RayMaxBounces), 1.0f, 0, GUIAPP_MAX_BOUNCES);
                ImGui::DragInt("Diffuse", reinterpret_cast<int*>(&m_RaytracerConfig.m_RayMaxDiffuseBounces), 1.0f, 0, GUIAPP_MAX_BOUNCES);
                ImGui::DragInt("Reflection", reinterpret_cast<int*>(&m_RaytracerConfig.m_RayMaxReflectionBounces), 1.0f, 0, GUIAPP_MAX_BOUNCES);
                ImGui::DragInt("Refraction", reinterpret_cast<int*>(&m_RaytracerConfig.m_RayMaxRefractionBounces), 1.0f, 0, GUIAPP_MAX_BOUNCES);
                ImGui::EndDisabled();
                ImGui::Indent(-32.f);
            }
            if (ImGui::CollapsingHeader("Tile", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::Indent(32.f);
                ImGui::BeginDisabled(isRaytracing);
                ImGui::DragInt("Tile Width", reinterpret_cast<int*>(&m_RaytracerConfig.m_TileWidth), 4.0f, 1, GUIAPP_MAX_RAYTRACING_TARGET_WIDTH);
                ImGui::DragInt("Tile Height", reinterpret_cast<int*>(&m_RaytracerConfig.m_TileHeight), 4.0f, 1, GUIAPP_MAX_RAYTRACING_TARGET_HEIGHT);
                ImGui::EndDisabled();
                ImGui::Indent(-32.f);
            }

            if (ImGui::CollapsingHeader("Threading", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::Indent(32.f);
                ImGui::BeginDisabled(isRaytracing);
                ImGui::DragInt("Threads", reinterpret_cast<int*>(&m_RaytracerConfig.m_Threads), 1.0f, GUIAPP_MIN_THREADS, GUIAPP_MAX_THREADS);
                ImGui::EndDisabled();
                ImGui::Indent(-32.f);
            }
            
            ImGui::End();
        } 
    }
    //*****************************************************
    
    if (showWindowRendrerControl)
    {
        ImGui::SetNextWindowSizeConstraints(ImVec2(300, 300), ImVec2(300, 300));
        if (ImGui::Begin("Controls", nullptr, mainWindowChildsFlags))
        {
            ImGui::End();
        } 
    }
    //*****************************************************
    if (showWindowImguiDebug)
    {
        ImGui::ShowDemoWindow();
    }
    //-----------------------------------------------------------------------
    ImGui::Render();
    glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    //-----------------------------------------------------------------------
}

void GUIApplication::DrawRenderTab()
{
    ImGuiIO& ImGuiIO = ImGui::GetIO();
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();


    //--------------------------------
    //RenderTarget image size (with correct aspect)
    float targetW = static_cast<float>(m_RaytracerTarget->getWidth());
    float targetH = static_cast<float>(m_RaytracerTarget->getHeight());
    float  rAspect = (targetW / targetH);
    float  wAspect = windowSize.x / windowSize.y;
    ImVec2 imageSize;
    if (wAspect > rAspect)
        imageSize = ImVec2(windowSize.y * rAspect, windowSize.y);
    else
        imageSize = ImVec2(windowSize.x, windowSize.x / rAspect);
    //--------------------------------
    //Process RenderTab controls
    if (ImGui::IsWindowHovered())
    {
        const float targetS = std::fmax(targetW, targetH);
        if (ImGuiIO.MouseWheel > 0.01f && m_RenderTab_Scale < targetS)
        {
            m_RenderTab_Scale *= 1.25f;
            //Correct offset

            m_RenderTab_Offset.x *= 1.25f;
            m_RenderTab_Offset.y *= 1.25f;
        }
        else if (ImGuiIO.MouseWheel < -0.01f && m_RenderTab_Scale >(1.f / targetS))
        {
            m_RenderTab_Scale *= 0.75f;
            //Correct offset
            m_RenderTab_Offset.x *= 0.75f;
            m_RenderTab_Offset.y *= 0.75f;
        }

        if (ImGuiIO.MouseDown[ImGuiMouseButton_Left] ||
            ImGuiIO.MouseDown[ImGuiMouseButton_Middle])
        {
            m_RenderTab_Offset.x += ImGuiIO.MouseDelta.x;
            m_RenderTab_Offset.y += ImGuiIO.MouseDelta.y;
        }
    }
    //--------------------------------
    //Apply scale coef
    imageSize.x *= m_RenderTab_Scale; imageSize.y *= m_RenderTab_Scale;
    //--------------------------------
    //Set image position at window center and add offset coef
    const float wCenterX = ((windowSize.x - imageSize.x) / 2) + m_RenderTab_Offset.x;
    const float wCenterY = ((windowSize.y - imageSize.y) / 2) + m_RenderTab_Offset.y;
    ImGui::SetCursorPos(ImVec2(wCenterX, wCenterY));
    //----------------
    ImGui::Image(
        reinterpret_cast<ImTextureID>(m_RaytracerTarget->getAPIObject()),
        imageSize,
        ImVec2(0, 0), ImVec2(1, 1));
    //--------------------------------
    if (m_Raytracer->IsProcessing())
    {
        auto rendererStage = m_Raytracer->getState();
        if (rendererStage->m_Stage == Raytracer::RaytracerStage::Stage::Rendering)
        {
            auto raytracingRegions = reinterpret_cast<const Raytracer::RaytracerRenderingRegionsStage*>(rendererStage);
            static const ImU32 taskBoxColorActive = ImColor(ImVec4(1.0f, 1.0f, 0.4f, 1.0f));
            for (auto i = 0; i < raytracingRegions->m_Regions.size(); i++)
            {
                auto region = raytracingRegions->m_Regions[i];
                float relx1 = (static_cast<float>(region.x / targetW) * imageSize.x) + wCenterX + windowPos.x;
                float rely1 = (static_cast<float>(region.y / targetH) * imageSize.y) + wCenterY + windowPos.y;
                float relx2 = relx1 + static_cast<float>(region.w / targetW) * imageSize.x;
                float rely2 = rely1 + static_cast<float>(region.h / targetH) * imageSize.y;
                
                ImGui::GetWindowDrawList()->AddRect(
                    ImVec2(relx1, rely1),
                    ImVec2(relx2, rely2),
                    taskBoxColorActive, 0, 4);
                    
            }
        }
        
        static int corner = 0;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
        if (corner != -1)
        {
            const float PADX = 25.0f;
            const float PADY = 40.0f;
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImVec2 work_pos = viewport->WorkPos;
            ImVec2 work_size = viewport->WorkSize;
            ImVec2 window_pos, window_pos_pivot;
            window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PADX) : (work_pos.x + PADX);
            window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PADY) : (work_pos.y + PADY);
            window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
            window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
            window_flags |= ImGuiWindowFlags_NoMove;
        }

        ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background

        if (ImGui::Begin("Rendering overlay", nullptr, window_flags))
        {
            ImGui::Text("Rendering...");
            if (rendererStage->m_Stage == Raytracer::RaytracerStage::Stage::Synchronization)
            {
                ImGui::Text("\tPreparing internals");
            }
            else if (rendererStage->m_Stage == Raytracer::RaytracerStage::Stage::Rendering)
            {
                const auto raytracingRegions = reinterpret_cast<const Raytracer::RaytracerRenderingRegionsStage*>(rendererStage);
                ImGui::Text("\t%d tasks queued", raytracingRegions->m_RegionsQueued);
            }
            if (rendererStage->m_Stage == Raytracer::RaytracerStage::Stage::Finalization)
            {
                ImGui::Text("\tCleanup");
            }
        } ImGui::End();

        
    }
}

void GUIApplication::resize_window_callback(GLFWwindow* glfw_window, int x, int y)
{
    if (x == 0 || y == 0)
    {
        return;
    }
    uint32_t m_lastWindowWidth = x;
    uint32_t m_lastWindowHeight = y;
}
