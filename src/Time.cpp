#include "Headers/Time.h"



void Time::UpdateDeltaTime()
{
    float currentTime = glfwGetTime();
    deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;
}


float Time::DeltaTime()
{
    return deltaTime;
}

int Time::GetFPS()
{
    return 1 / deltaTime;
}