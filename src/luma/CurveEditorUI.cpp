#include "CurveEditorUI.h"
#include <imgui.h>

void CurveEditorUI::Draw()
{
    if (ImGui::Button("Apply diagam"))
    {
        auto curveApply = [this](const cv::Mat &mat)
        {
            return this->curve_.ApplyToImage(mat);
        };
        manager_.ProccessCurrent(curveApply);
    }

    if (ImGui::RadioButton("Linear", curve_.GetInterpolationMode() == Curve::Interpolation::LINEAR))
    {
        curve_.SetInterpolationMode(Curve::Interpolation::LINEAR);
    }
    if (ImGui::RadioButton("Quadtratic", curve_.GetInterpolationMode() == Curve::Interpolation::QUADRATIC))
    {
        curve_.SetInterpolationMode(Curve::Interpolation::QUADRATIC);
    }

    ImGui::Spacing();

    canvasSize = ImVec2(300, 300);
    canvasPos = ImGui::GetCursorScreenPos();
    draw_list = ImGui::GetWindowDrawList();

    ImGui::InvisibleButton("#canvas", canvasSize);

    DrawGrid();
    DrawCurve();
    DrawPoints();

    ImGuiIO &io = ImGui::GetIO();

    ImVec2 mouse_pos = io.MousePos;
    cv::Point2f canvas_mouse_pos = ScreenToCanvas(mouse_pos);

    bool is_mouse_in_canvas =
        mouse_pos.x >= canvasPos.x && mouse_pos.x <= canvasPos.x + canvasSize.x &&
        mouse_pos.y >= canvasPos.y && mouse_pos.y <= canvasPos.y + canvasSize.y;

    if (is_mouse_in_canvas && ImGui::IsMouseDoubleClicked(0))
    {
        curve_.AddPoint(canvas_mouse_pos);
        return;
    }

    static int dragged_point_index = -1;

    if ((ImGui::IsMouseClicked(0) || ImGui::IsMouseClicked(1)) && is_mouse_in_canvas)
    {
        const auto &points = curve_.GetPoints();
        for (int i = 0; i < points.size(); ++i)
        {
            ImVec2 screen_pos = CanvasToScreen(points[i]);
            float distance = sqrtf(
                (mouse_pos.x - screen_pos.x) * (mouse_pos.x - screen_pos.x) +
                (mouse_pos.y - screen_pos.y) * (mouse_pos.y - screen_pos.y));

            if (distance <= 10.0f)
            {
                dragged_point_index = i;

                if (ImGui::IsMouseClicked(1))
                {
                    curve_.RemovePoint(i);
                    dragged_point_index = -1;
                }
                break;
            }
        }
    }

    if (dragged_point_index != -1 && ImGui::IsMouseDragging(0))
    {
        canvas_mouse_pos.x = std::max(0.0f, std::min(1.0f, canvas_mouse_pos.x));
        canvas_mouse_pos.y = std::max(0.0f, std::min(1.0f, canvas_mouse_pos.y));
        curve_.MovePoint(dragged_point_index, canvas_mouse_pos);
    }

    if (ImGui::IsMouseClicked(1) && ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
    {
        curve_.ClearPoints();
    }

    if (ImGui::IsMouseReleased(0))
    {
        dragged_point_index = -1;
    }
}

void CurveEditorUI::DrawGrid()
{
    draw_list->AddRect(canvasPos,
                       ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y),
                       IM_COL32(255, 255, 255, 255));

    for (int i = 1; i < 4; ++i)
    {
        float x = canvasPos.x + (canvasSize.x / 4.0f) * i;
        float y = canvasPos.y + (canvasSize.y / 4.0f) * i;

        draw_list->AddLine(ImVec2(x, canvasPos.y), ImVec2(x, canvasPos.y + canvasSize.y),
                           IM_COL32(100, 100, 100, 100));
        draw_list->AddLine(ImVec2(canvasPos.x, y), ImVec2(canvasPos.x + canvasSize.x, y),
                           IM_COL32(100, 100, 100, 100));
    }
}

void CurveEditorUI::DrawCurve()
{
    const int segments = 100;
    const ImU32 curve_color = IM_COL32(0, 255, 255, 255);
    const ImU32 identity_color = IM_COL32(100, 100, 100, 100);

    draw_list->AddLine(
        canvasPos,
        ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y),
        identity_color, 1.0f);

    std::vector<ImVec2> points;
    points.reserve(segments + 1);

    for (int i = 0; i <= segments; ++i)
    {
        float x = i / static_cast<float>(segments);
        float y = curve_.Evaluate(x);

        ImVec2 point = CanvasToScreen(cv::Point2f(x, y));
        points.push_back(point);
    }

    if (points.size() >= 2)
    {
        draw_list->AddPolyline(points.data(), points.size(), curve_color, false, 2.0f);
    }
}

ImVec2 CurveEditorUI::CanvasToScreen(const cv::Point2f &canvas_point) const
{
    return ImVec2(
        canvasPos.x + canvas_point.x * canvasSize.x,
        canvasPos.y + (1.0f - canvas_point.y) * canvasSize.y // Flip Y axis
    );
}

cv::Point2f CurveEditorUI::ScreenToCanvas(const ImVec2 &screen_point) const
{
    return cv::Point2f(
        (screen_point.x - canvasPos.x) / canvasSize.x,
        1.0f - (screen_point.y - canvasPos.y) / canvasSize.y // Flip Y axis
    );
}

void CurveEditorUI::DrawPoints()
{
    const auto &points = curve_.GetPoints();

    for (int i = 0; i < points.size(); ++i)
    {
        ImVec2 screen_pos = CanvasToScreen(points[i]);
        ImU32 color = IM_COL32(255, 0, 0, 255);

        draw_list->AddCircleFilled(screen_pos, 6.0f, color);
        ImVec2 button_size = ImVec2(12.0f, 12.0f);
        ImVec2 button_pos = ImVec2(screen_pos.x - 6.0f, screen_pos.y - 6.0f);

        ImGui::SetCursorScreenPos(button_pos);
        ImGui::PushID(i);
        ImGui::InvisibleButton("circle_button", button_size);

        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("(%.2f, %.2f)", points[i].x, points[i].y);
        }
        ImGui::PopID();
    }
}
