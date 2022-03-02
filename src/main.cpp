#include <iostream>
#include "window.hpp"
#include "renderer.hpp"

bool isRunning;

int main()
{
    isRunning = true;

    Window window(500, 500, "title");

    Renderer vulkanRenderer(window.getHWND(), window.getHINSTANCE());
    
    while(isRunning){

        window.GetMessages();

        vulkanRenderer.Update();

        vulkanRenderer.Draw();
    }


    return 0;
}