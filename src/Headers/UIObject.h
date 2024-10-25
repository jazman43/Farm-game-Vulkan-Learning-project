#pragma once

#include <memory>
#include <vector>

#include "Time.h"


class UIObject
{
private:
    Time time;

    std::vector<std::unique_ptr<UIObject>> uiObjects;
public:
    

    void InitUI();
    void UpdateUI(float deltaTime);

    

protected:

    virtual void UIUpdate(float deltaTime) = 0;

    bool DrawButtonEx();
    bool DrawButton();
    void DrawFps(int posX, int posY);

};

