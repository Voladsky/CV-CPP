#ifndef _LUMA_APPLICATION
#define _LUMA_APPLICATION

#include "BaseApplication.h"
#include <opencv2/opencv.hpp>
#include <imgui.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include "image_processing.h"
#include "ImageManager.h"
#include "ImageSelectorUI.h"
#include <iostream>

class LumaApplication : public BaseApplication
{
public:
    LumaApplication(): BaseApplication() {
        manager_.AddFolder("./images/");
    }
protected:
    void Update(float delta_time) override
    {
    }
    void RenderUI() override
    {
        static float f = 0.0f;
        static int counter = 0;
        static int current_image = 0;
        try
        {
            selector_.Draw();
        }
        catch (std::exception& e) {
            std::cerr << e.what();
            throw;
        }
    }
    void ProcessEvent(const SDL_Event &event) override
    {
    }

private:
    ImageManager manager_;
    ImageSelectorUI selector_{manager_};
};

#endif
