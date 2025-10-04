#include "ImageSelectorUI.h"
#include "GLTexture.h"
#include <iostream>

void ImageSelectorUI::Draw()
{
    ImGui::Begin("Image Selector");
    for (int i = 0; i < manager_.GetImagesSize(); i++)
    {
        ImGui::PushID(i);
        const Image *image = manager_.GetImage(i);
        ImGui::BeginGroup();
        if (ImGui::ImageButton(image->name.c_str(),
                               (void*)(intptr_t)image->orig_texture.id(),
                               ImVec2(thumbnail_size_, thumbnail_size_)))
        {
            std::cout << i;
            manager_.SelectImage(i);
        }
        ImGui::Text(image->name.c_str());
        ImGui::EndGroup();
        ImGui::PopID();
        ImGui::SameLine();
    }
    ImGui::NewLine();
    ImGui::SliderFloat("Thumbnail size", &thumbnail_size_, 10.0f, 200.0f);
    if (ImGui::Button("Reset current image")) {
        manager_.ResetCurrent();
    }
    ImGui::End();
}
