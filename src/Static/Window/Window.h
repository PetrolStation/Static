#pragma once

#include <string>
#include <memory>
#include <utility>
#include <Core/DebugTools.h>

#include "Core/Aliases.h"

#include "Core/Image.h"
#include "Core/EventStack.h"
#include "Core/Window/WindowResourceCreator.h"

namespace PetrolEngine {

    extern double deltaXMousePos;
    extern double deltaYMousePos;
    extern double cursorXPos;
    extern double cursorYPos;

    extern double deltaTime;

	class Window {
	public:
        static void setContext(WRC& creator){
            Window::creator = &creator;
        }

        static int  init(int width, int height, const String& title){ LOG_FUNCTION();
            window = creator->newWindow(width, height, title);
            auto err = window->init();
            i_window = window->i_window;
            return err;
        };

		static Ref<WindowApi> create(int width, int height, const String& title);

        NO_DISCARD static int getWidth () { LOG_FUNCTION(); return window->getWidth(); };
		NO_DISCARD static int getHeight() { LOG_FUNCTION(); return window->getHeight(); };

		static void swapBuffers() { LOG_FUNCTION(); return window->swapBuffers(); };
        static bool shouldClose() { LOG_FUNCTION(); return window->shouldClose(); };
		static void close      () { LOG_FUNCTION(); return window->close(); };
        static void pollEvents () { LOG_FUNCTION(); return window->pollEvents(); };
        static void showCursor(bool x) { LOG_FUNCTION(); return window->showCursor(x); };

		static void setVSync(bool   enabled) { LOG_FUNCTION(); return window->setVSync(enabled); };
        static void setIcon (Image* image  ) { LOG_FUNCTION(); return window->setIcon(image); };
        static void setIcon (String path  ) { LOG_FUNCTION();
            Image* iconImage = Image::create(path);
            window->setIcon(iconImage);
            delete iconImage;
        };

		static bool isPressed(Keys key) { LOG_FUNCTION(); return window->isPressed(key); };

        ~Window() = default;

		// Vulkan specific
		static void createWindowSurface(void* instance, const void* allocation, void* surface) { return window->createWindowSurface(instance, allocation, surface); };

        static float getAspectRatio() { LOG_FUNCTION(); return window->getAspectRatio(); };

        static WRC* creator;
        static WindowApi* window;
        static void* i_window;
	};
}
