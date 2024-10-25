#include "Headers/UIObject.h"

#include <iostream>


void UIObject::InitUI()
{    
	//uiObjects.push_back(std::make_unique<DebugUI>());
}

void UIObject::UpdateUI(float deltaTime)
{
    for(auto& uiObject : uiObjects)
    {
        uiObject->UpdateUI(deltaTime);
    }
}

void UIObject::DrawFps(int posX, int posY)
{
    
    std::cout << "FPS\n" << time.GetFPS() << "\n";
}