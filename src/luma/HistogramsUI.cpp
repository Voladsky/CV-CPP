#include "HistogramsUI.h"
#include "image_processing.h"

void HistogramsUI::Draw()
{
    ImGui::Begin("Histograms");

    if (ImGui::Button("Histogram Equalization"))
    {
        manager_.ProccessCurrent(Processing::HistogramEqualization);
    }

    ImGui::End();
}
