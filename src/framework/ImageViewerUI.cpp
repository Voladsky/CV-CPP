#include "ImageViewerUI.h"
#include "image_processing.h"
#include "GLTexture.h"

void ImageViewerUI::Draw() {
    int idx = -1;

    if ((idx = manager_.GetSelectedIdx()) == -1) return;

    ImGui::Begin("Image Viewer");

    ImGui::Columns(2);

    const Image* image = manager_.GetImage(idx);

    ImGui::Image(image->orig_texture.id(), ImVec2(size_, size_));

    ImGui::NextColumn();

    ImGui::Image(image->processed_texture.id(), ImVec2(size_, size_));

    ImGui::End();

    ImGui::Begin("Histograms");

    HistogramManager hist(*image);
    HistogramPlotterUI histPlotter{hist};
    histPlotter.Draw();

    ImGui::End();
}
