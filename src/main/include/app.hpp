#pragma once

#include <AL/alc.h>

#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>

#include <functional>
#include <string>

class application {    
    GLFWwindow * _window;
    ALCdevice * _alcDevice;
    ALCcontext * _alcContext;
    std::function<void()> _onFrame;

public:
    application(int width, int height, const std::string& title);

    virtual ~application();

    void setOnFrame(const std::function<void()>& fn);

    void doFrame() const;

    void start(const std::function<void()>& fn);
};