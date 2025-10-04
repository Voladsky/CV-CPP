#include "ImageSelectorUI.h"
#include <iostream>

void ImageSelectorUI::Draw()
{
    ImGui::Begin("Image Selector");
    for (int i = 0; i < manager_.GetImagesSize(); i++)
    {
        ImGui::PushID(i);
        const Image *image = manager_.GetImage(i);
        ImGui::BeginGroup();
        if (ImGui::ImageButton("test",
                               matToTexture(image->original),
                               ImVec2(100, 100)))
        {
            manager_.SelectImage(i);
        }
        ImGui::Text(image->name.c_str());
        ImGui::EndGroup();
        ImGui::PopID();
        ImGui::SameLine();
    }
    ImGui::End();
}
