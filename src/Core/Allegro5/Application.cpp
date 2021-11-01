// Copyright (c) 2021-present Sparky Studios. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <SparkyStudios/UI/Pixel/Core/Animation/Animation.h>
#include <SparkyStudios/UI/Pixel/Core/Application.h>

#include <Core/Allegro5/Input/InputHandler.h>
#include <Core/Allegro5/Renderer/Renderer.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

namespace SparkyStudios::UI::Pixel
{
    static Application* gApplication = nullptr;

    static ALLEGRO_DISPLAY* gDisplay = nullptr;
    static ALLEGRO_EVENT_QUEUE* gEventQueue = nullptr;
    static ALLEGRO_TIMER* gTimer = nullptr;

    static PiString gAppResourcesDir = "resources";

    Application::~Application()
    {
        delete _mainWindow;

        delete _skin;
        delete _renderer;

        al_destroy_event_queue(gEventQueue);
        al_destroy_timer(gTimer);
    }

    bool Application::Init(MainWindow* mainWindow, const Skin::Data& skinData)
    {
        if (!_initialized)
        {
            if (!al_init())
                return false;

            // Create the main window
            mainWindow->CreateNativeWindow();
            gDisplay = static_cast<ALLEGRO_DISPLAY*>(mainWindow->GetNativeHandle());

            if (!gDisplay)
                return false;

            // Create a time with a fixed FPS of 60
            gTimer = al_create_timer(1.0 / 60.0);

            if (!gTimer)
                return false;

            gEventQueue = al_create_event_queue();

            if (!gEventQueue)
                return false;

            al_init_image_addon();
            al_init_font_addon();
            al_init_primitives_addon();
            al_init_ttf_addon();

            al_install_mouse();
            al_install_keyboard();

            al_register_event_source(gEventQueue, al_get_display_event_source(gDisplay));
            al_register_event_source(gEventQueue, al_get_mouse_event_source());
            al_register_event_source(gEventQueue, al_get_keyboard_event_source());
            al_register_event_source(gEventQueue, al_get_timer_event_source(gTimer));

            al_start_timer(gTimer);

            SetAppResourcesDirectoryPath(skinData.resourcesDir);

            _paths = RelativeToExecutableResourcePaths(gAppResourcesDir);

            _renderer = new Renderer_Allegro(_paths);
            _skin = new Skin(skinData, _renderer);

            mainWindow->CreateRootCanvas(_skin);

            _mainWindow = mainWindow;

            _initialized = true;
        }

        return _initialized;
    }

    int Application::Run()
    {
        if (!_initialized)
            return EXIT_FAILURE;

        InputHandler_Allegro inputHandler{};
        inputHandler.Initialize(_mainWindow->GetRootCanvas().get());

        ALLEGRO_EVENT ev;
        bool shouldQuit = false;
        while (!shouldQuit)
        {
            al_wait_for_event(gEventQueue, &ev);

            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                shouldQuit = true;

            // Process input events
            else if (inputHandler.ProcessMessage(ev))
            {
                // noop
            }

            else if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
                _mainWindow->OnResize(Size(ev.display.width, ev.display.height));

            else if (ev.type == ALLEGRO_EVENT_DISPLAY_EXPOSE)
                _mainWindow->OnExpose();

            else if (ev.type == ALLEGRO_EVENT_TIMER)
            {
#if PI_ENABLE_ANIMATION
                // Update animation frames
                Animation::Tick(ev.timer.timestamp);
#endif // PI_ENABLE_ANIMATION

                // Paint the widgets
                _mainWindow->Paint(_skin);
            }

            al_rest(0.001);
        }

        return EXIT_SUCCESS;
    }

    Application::Application()
        : _initialized(false)
        , _mainWindow(nullptr)
        , _paths()
        , _skin(nullptr)
        , _renderer(nullptr)
    {}

    Application* Application::Instance()
    {
        return gApplication ? gApplication : gApplication = new Application();
    }

    void Application::SetAppResourcesDirectoryPath(const PiString& path)
    {
        gAppResourcesDir = path;
    }

    const PiString& Application::GetAppResourcesDirectoryPath()
    {
        return gAppResourcesDir;
    }
} // namespace SparkyStudios::UI::Pixel
