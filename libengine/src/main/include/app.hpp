#pragma once

#include <AL/alc.h>

#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>

#include <functional>
#include <memory>
#include <string>

class application {    
    GLFWwindow * _window;
    ALCdevice * _alcDevice;
    ALCcontext * _alcContext;
    std::function<void(void *)> _onFrame;

public:
    std::shared_ptr<void> userData;

    application() :
        _window(nullptr),
        _alcDevice(nullptr),
        _alcContext(nullptr),
        _onFrame(nullptr),
        userData(nullptr) {}

    application(int width, int height, const std::string& title);

    application(const application&) = delete;

    application(application&&) = default;

    application& operator=(const application&) = delete;

    application& operator=(application&&) = default;    

    virtual ~application();

    void setOnFrame(const std::function<void(void *)>& fn);

    void doFrame() const;

    void start(const std::function<void(void *)>& fn);
};