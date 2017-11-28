#pragma once

#include <functional>
#include <memory>
#include <string>

namespace engine {
    struct resources {
        virtual ~resources() {}

        virtual void update() const = 0;

        virtual bool isValid() const = 0;
    };

    class application {
        std::unique_ptr<resources> _resources;        
        std::function<void(void *)> _onFrame;

        application(const application&) = delete;

        application& operator=(application&&) = default;

    public:
        std::shared_ptr<void> userData;

        application() :
            _resources(nullptr),
            _onFrame(nullptr),
            userData(nullptr) {}

        application(int width, int height, const std::string& title);        

        application(application&&) = default;

        application& operator=(const application&) = delete;            

        void setOnFrame(const std::function<void(void *)>& fn);

        void doFrame() const;

        void start(const std::function<void(void *)>& fn);
    };
}