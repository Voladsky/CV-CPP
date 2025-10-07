#ifndef _HISTOGRAM_PLOTTER
#define _HISTOGRAM_PLOTTER

#include "Histogram.h"

class HistogramPlotterUI
{
public:
    HistogramPlotterUI(HistogramManager &hist) : hist_(hist) {}
    void Draw()
    {
        auto original = hist_.GetOriginalHist();
        if (ImGui::CollapsingHeader("Original"))
        {
            ImGui::PushStyleColor(ImGuiCol_PlotHistogram, IM_COL32(255, 0, 0, 255));
            ImGui::PlotHistogram("RedOrig", original.r.data(), original.r.size(), 0, nullptr, 0.0f, 1.0f, ImVec2(0, 80));
            ImGui::PopStyleColor();
            ImGui::PushStyleColor(ImGuiCol_PlotHistogram, IM_COL32(0, 255, 0, 255));
            ImGui::PlotHistogram("GreenOrig", original.g.data(), original.g.size(), 0, nullptr, 0.0f, 1.0f, ImVec2(0, 80));
            ImGui::PopStyleColor();
            ImGui::PushStyleColor(ImGuiCol_PlotHistogram, IM_COL32(0, 0, 255, 255));
            ImGui::PlotHistogram("BlueOrig", original.b.data(), original.b.size(), 0, nullptr, 0.0f, 1.0f, ImVec2(0, 80));
            ImGui::PopStyleColor();
            ImGui::PushStyleColor(ImGuiCol_PlotHistogram, IM_COL32(100, 100, 100, 255));
            ImGui::PlotHistogram("BrightOrig", original.brightness.data(), original.brightness.size(), 0, nullptr, 0.0f, 1.0f, ImVec2(0, 80));
            ImGui::PopStyleColor();
        }
        auto processed = hist_.GetProcessedHist();
        if (ImGui::CollapsingHeader("Processed"))
        {
            ImGui::PushStyleColor(ImGuiCol_PlotHistogram, IM_COL32(255, 0, 0, 255));
            ImGui::PlotHistogram("RedProc", processed.r.data(), processed.r.size(), 0, nullptr, 0.0f, 1.0f, ImVec2(0, 80));
            ImGui::PopStyleColor();
            ImGui::PushStyleColor(ImGuiCol_PlotHistogram, IM_COL32(0, 255, 0, 255));
            ImGui::PlotHistogram("GreenProc", processed.g.data(), processed.g.size(), 0, nullptr, 0.0f, 1.0f, ImVec2(0, 80));
            ImGui::PopStyleColor();
            ImGui::PushStyleColor(ImGuiCol_PlotHistogram, IM_COL32(0, 0, 255, 255));
            ImGui::PlotHistogram("BlueProc", processed.b.data(), processed.b.size(), 0, nullptr, 0.0f, 1.0f, ImVec2(0, 80));
            ImGui::PopStyleColor();
            ImGui::PushStyleColor(ImGuiCol_PlotHistogram, IM_COL32(100, 100, 100, 255));
            ImGui::PlotHistogram("BrightOrig", processed.brightness.data(), processed.brightness.size(), 0, nullptr, 0.0f, 1.0f, ImVec2(0, 80));
            ImGui::PopStyleColor();
        }
    }
    void SetData(const HistogramManager &hist) { hist_ = hist; }

private:
    HistogramManager &hist_;
};

#endif
