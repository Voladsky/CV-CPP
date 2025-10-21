#include "OtsuUI.h"
#include <functional>

void OtsuUI::Draw()
{
    ImGui::Begin("Color correction");

    static int levels = 4;
    static int block_size = 64;
    static int kNeighbors = 8;

    ImGui::SliderInt("Block Size", &block_size, 16, 256);
    ImGui::SliderInt("Levels for Quantization", &levels, 2, 20);
    ImGui::SliderInt("KNeighbors", &kNeighbors, 1, 64);

    if (ImGui::CollapsingHeader("Methods"))
    {
        ImGui::Indent();

        if (ImGui::Button("Brightness Quantization"))
        {
            auto brightnessQuantization = std::bind(Processing::BrightnessQuantization,
                                                    std::placeholders::_1,
                                                    levels);
            manager_.ProccessCurrent(brightnessQuantization);
        }

        if (ImGui::Button("Global Otsu"))
        {
            manager_.ProccessCurrent(Processing::GlobalOtsu);
        }

        if (ImGui::Button("Local Otsu"))
        {
            auto localOtsu = std::bind(Processing::LocalOtsu,
                                       std::placeholders::_1,
                                       block_size);
            manager_.ProccessCurrent(localOtsu);
        }

        if (ImGui::Button("Hierarchy Otsu"))
        {
            auto hierOtsu = std::bind(Processing::HierarchicalOtsu,
                                      std::placeholders::_1,
                                      block_size);
            manager_.ProccessCurrent(hierOtsu);
        }

        if (ImGui::Button("ColorQuant"))
        {
            auto colorQuant = std::bind(Processing::ColorImageQuant,
                                        std::placeholders::_1,
                                        kNeighbors);
            manager_.ProccessCurrent(colorQuant);
        }

        ImGui::Unindent();
    }

    ImGui::End();
}
