#ifndef _CURVE_EDITOR
#define _CURVE_EDITOR

#include "Curve.h"
#include "ImageManager.h"
#include <imgui.h>

class CurveEditorUI {
public:
    CurveEditorUI(Curve& curve, ImageManager& manager): curve_(curve), manager_(manager) {}
    void Draw();
private:
    Curve& curve_;
    ImageManager& manager_;
    ImVec2 canvasPos;
    ImVec2 canvasSize;
    ImDrawList *draw_list;
    void DrawGrid();
    void DrawCurve();
    void DrawPoints();
    ImVec2 CanvasToScreen(const cv::Point2f &canvas_point) const;
    cv::Point2f ScreenToCanvas(const ImVec2 &screen_point) const;
};

#endif
