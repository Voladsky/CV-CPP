#ifndef _LUMA_APPLICATION
#define _LUMA_APPLICATION

#include "BaseApplication.h"
#include <opencv2/opencv.hpp>
#include <imgui.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

GLuint matToTexture(const cv::Mat &mat);

class LumaApplication : public BaseApplication
{
protected:
    void Update(float delta_time) override
    {
    }
    void RenderUI() override
    {
        static float f = 0.0f;
        static int counter = 0;
        static int current_image = 0;
        ImGui::Begin("Image Selector");
        cv::Mat img = cv::imread("images/miku.jpg");
        cv::Mat img2 = cv::imread("images/teto.jpg");

        ImGui::BeginGroup();
        if (ImGui::ImageButton("Image", matToTexture(img), ImVec2(100, 100)))
        {
            current_image = 0;
        };
        ImGui::Text("Hatsune Miku");
        ImGui::EndGroup();

        ImGui::SameLine();

        ImGui::BeginGroup();
        if (ImGui::ImageButton("Image2", matToTexture(img2), ImVec2(100, 100)))
        {
            current_image = 1;
        };
        ImGui::Text("Kasane Teto");
        ImGui::EndGroup();

        ImGui::Image(matToTexture(current_image == 0 ? img : img2), ImVec2(500, 500));

        ImGui::End();
    }
    void ProcessEvent(const SDL_Event &event) override
    {
    }
};

#endif
