#pragma once

#include <string>
#include <memory>
#include <utility>

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

        static int  init(int width, int height, const String& title){
            window = creator->newWindow(width, height, title);
            auto err = window->init();
            i_window = window->i_window;
            return err;
        };

		static Ref<WindowApi> create(int width, int height, const String& title);

        NO_DISCARD static int getWidth () { return window->getWidth(); };
		NO_DISCARD static int getHeight() { return window->getHeight(); };

		static void swapBuffers() { return window->swapBuffers(); };
        static bool shouldClose() { return window->shouldClose(); };
		static void close      () { return window->close(); };
        static void pollEvents () { return window->pollEvents(); };
        static void showCursor(bool x) { return window->showCursor(x); };

		static void setVSync(bool   enabled) {return window->setVSync(enabled); };
        static void setIcon (Image* image  ) {return window->setIcon(image); };
        static void setIcon (String path  ) {
            Image* iconImage = Image::create(path);
            window->setIcon(iconImage);
            delete iconImage;
        };

		static bool isPressed(Keys key) { return window->isPressed(key); };

        ~Window() = default;

		// Vulkan specific
		static void createWindowSurface(void* instance, const void* allocation, void* surface) { return window->createWindowSurface(instance, allocation, surface); };

        static float getAspectRatio() {return window->getAspectRatio(); };

        static WRC* creator;
        static WindowApi* window;
        static void* i_window;
	};
}
