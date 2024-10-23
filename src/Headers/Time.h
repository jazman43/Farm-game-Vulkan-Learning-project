#pragma once

#include "window.h"

class Time
{
private:
    float deltaTime = 0.0f;
    
    float lastFrameTime = 0.0f;

public:
    Time()
    {
       
    }


   void UpdateDeltaTime();//call once at start of main loop

   float DeltaTime();

   int GetFPS();
};

