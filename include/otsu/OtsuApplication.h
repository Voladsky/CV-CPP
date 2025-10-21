#ifndef _OTSU_APPLICATION
#define _OTSU_APPLICATION

#include "BaseApplication.h"
#include <opencv2/opencv.hpp>
#include <imgui.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <iostream>
#include "image_processing.h"
#include "ImageManager.h"
#include "ImageSelectorUI.h"
#include "ImageViewerUI.h"
#include "OtsuUI.h"

class OtsuApplication : public BaseApplication
{
protected:
    void Start() {
        manager_.AddFolder("./images/");
        // curve_.AddPoint(cv::Point2f(0.3f, 0.7f));
        // curve_.AddPoint(cv::Point2f(0.7f, 0.3f));
    }
    void Update(float delta_time) override {
    }
    void RenderUI() override {
        static float f = 0.0f;
        static int counter = 0;
        static int current_image = 0;
        try
        {
            selector_.Draw();
            viewer_.Draw();
            otsu_.Draw();
        }
        catch (std::exception &e)
        {
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
    ImageViewerUI viewer_{manager_};
    OtsuUI otsu_{manager_};
};

#endif
