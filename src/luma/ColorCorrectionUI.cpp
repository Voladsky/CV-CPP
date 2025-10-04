#include "ColorCorrectionUI.h"
#include <functional>

void ColorCorrectionUI::Draw()
{
    static ImVec4 referenceColor;
    static ImVec4 targetColor;

    ImGui::Begin("Color correction");

    if (ImGui::CollapsingHeader("Gray World correction"))
    {
        ImGui::Indent();
        if (ImGui::Button("Apply"))
        {
            manager_.ProccessCurrent(Processing::GrayWorldCorrection);
        }
        ImGui::Unindent();
    }

    if (ImGui::CollapsingHeader("Reference Color correction"))
    {
        ImGui::Indent();
        ImGui::Text("Reference Color (from image):");
        if (ImGui::ColorButton("Reference Color", referenceColor))
        {
            ImGui::OpenPopup("ref_color_picker");
        }

        if (ImGui::BeginPopup("ref_color_picker"))
        {
            ImGui::ColorPicker3("##RefPicker", (float *)&referenceColor);
            if (ImGui::Button("OK"))
            {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        ImGui::SameLine();
        ImGui::Text("RGB: (%.3f, %.3f, %.3f)",
                    referenceColor.x, referenceColor.y, referenceColor.z);

        // Target color picker
        ImGui::Text("Target Color:");
        if (ImGui::ColorButton("Target Color", targetColor))
        {
            ImGui::OpenPopup("target_color_picker");
        }

        if (ImGui::BeginPopup("target_color_picker"))
        {
            ImGui::ColorPicker3("##TargetPicker", (float *)&targetColor);
            if (ImGui::Button("OK"))
            {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        ImGui::SameLine();
        ImGui::Text("RGB: (%.3f, %.3f, %.3f)",
                    targetColor.x, targetColor.y, targetColor.z);

        if (ImGui::Button("Apply"))
        {
            cv::Scalar targetCV(targetColor.x, targetColor.y, targetColor.z);
            cv::Scalar referenceCV(referenceColor.x, referenceColor.y, referenceColor.z);
            auto referenceColorCorrection = std::bind(Processing::ReferencePixelCorrection,
                                                      std::placeholders::_1,
                                                      referenceCV,
                                                      targetCV);
            manager_.ProccessCurrent(referenceColorCorrection);
        }
        ImGui::Unindent();
    }

    if (ImGui::CollapsingHeader("Transformation Functions"))
    {
        ImGui::Indent();

        if (ImGui::Button("Apply linear stretching"))
        {
            auto linearStretching = std::bind(Processing::LinearStretching,
                                              std::placeholders::_1,
                                              0.0f,
                                              255.0f);
            manager_.ProccessCurrent(linearStretching);
        }

        if (ImGui::CollapsingHeader("Gamma Correction"))
        {
            static float gamma = 1.0f;
            static float gain = 1.0f;

            ImGui::InputFloat("Gamma", &gamma);
            ImGui::InputFloat("Gain", &gain);

            if (ImGui::Button("Apply gamma correction"))
            {
                auto gammaCorrection = std::bind(Processing::GammaCorrection,
                                                 std::placeholders::_1,
                                                 gamma,
                                                 gain);
                manager_.ProccessCurrent(gammaCorrection);
            }
        }

        ImGui::Unindent();
    }

    ImGui::End();
}
