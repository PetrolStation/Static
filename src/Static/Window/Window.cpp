#include "PCH.h"

#include "Window.h"

namespace PetrolEngine {

    double deltaXMousePos = 0.0;
    double deltaYMousePos = 0.0;
    double cursorXPos     = 0.0;
    double cursorYPos     = 0.0;
    double deltaTime      = 0.0;

    WRC* Window::creator;
    WindowApi* Window::window;
    void* Window::i_window;

    Ref<WindowApi> Window::create(int width, int height, const String& title) {
        return Ref<WindowApi>(creator->newWindow(width, height, title));
    }
    
}
