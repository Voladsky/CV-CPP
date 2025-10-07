#ifndef _CURVE_H
#define _CURVE_H

#include <vector>
#include <algorithm>
#include <opencv2/opencv.hpp>

class Curve
{
public:
    enum class Interpolation
    {
        LINEAR,
        QUADRATIC
    };
    Curve()
    {
        points_.push_back(cv::Point2f(0.0f, 0.0f));
        points_.push_back(cv::Point2f(1.0f, 1.0f));
    }
    void AddPoint(const cv::Point2f &point)
    {
        points_.push_back(point);
        SortPoints();
        reload_lut_ = true;
    }
    void RemovePoint(int index)
    {
        if (index <= 0 || index >= points_.size() - 1)
            return;

        points_.erase(points_.begin() + index);
        reload_lut_ = true;
    }
    void MovePoint(int index, const cv::Point2f &new_pos)
    {
        if (index <= 0 || index >= points_.size() - 1)
            return;
        points_[index] = new_pos;
        SortPoints();
        reload_lut_ = true;
    }
    void ClearPoints()
    {
        points_.clear();
        points_.push_back(cv::Point2f(0.0f, 0.0f));
        points_.push_back(cv::Point2f(1.0f, 1.0f));
        reload_lut_ = true;
    }
    const std::vector<cv::Point2f> &GetPoints() const
    {
        return points_;
    }
    float Evaluate(float x) const
    {
        for (size_t i = 0; i < points_.size() - 1; ++i)
        {
            if (x >= points_[i].x && x <= points_[i + 1].x)
            {
                return Interpolate(x, i);
            }
        }
        return points_.back().y;
    }
    cv::Mat ApplyToImage(const cv::Mat image) const
    {
        if (image.empty())
            return cv::Mat();

        if (reload_lut_)
            UpdateLUT();

        cv::Mat result;
        cv::LUT(image, lut_, result);
        return result;
    }
    void SetInterpolationMode(Interpolation interp)
    {
        interpolation_ = interp;
        reload_lut_ = true;
    }
    Interpolation GetInterpolationMode() const { return interpolation_; }

private:
    std::vector<cv::Point2f> points_;
    Interpolation interpolation_ = Interpolation::LINEAR;
    mutable std::vector<uint8_t> lut_;
    mutable bool reload_lut_ = true;
    void SortPoints()
    {
        std::sort(points_.begin(), points_.end(),
                  [](const cv::Point2f &a, const cv::Point2f &b)
                  { return a.x < b.x; });
    }
    float Interpolate(float &x, int index) const
    {
        if (interpolation_ == Interpolation::LINEAR)
        {
            return InterpolateLinear(x, index);
        }
        else if (interpolation_ == Interpolation::QUADRATIC)
        {
            return InterpolateQuadratic(x, index);
        }
        throw std::invalid_argument("unknown interpolation mode");
    }
    float InterpolateLinear(float &x, int index) const
    {
        auto p1 = points_[index];
        auto p2 = points_[index + 1];
        float t = (x - p1.x) / (p2.x - p1.x);
        return p1.y + t * (p2.y - p1.y);
    }
    float InterpolateQuadratic(float &x, int index) const
    {
        // Clamp to valid range
        int n = static_cast<int>(points_.size());
        if (n < 3)
            return InterpolateLinear(x, index);

        // Pick 3 points for the local quadratic fit
        int i0, i1, i2;
        if (index == 0)
        {
            i0 = 0;
            i1 = 1;
            i2 = 2;
        }
        else if (index >= n - 2)
        {
            i0 = n - 3;
            i1 = n - 2;
            i2 = n - 1;
        }
        else
        {
            i0 = index - 1;
            i1 = index;
            i2 = index + 1;
        }

        const auto &p0 = points_[i0];
        const auto &p1 = points_[i1];
        const auto &p2 = points_[i2];

        float x0 = p0.x, y0 = p0.y;
        float x1 = p1.x, y1 = p1.y;
        float x2 = p2.x, y2 = p2.y;

        // Lagrange basis form
        float L0 = ((x - x1) * (x - x2)) / ((x0 - x1) * (x0 - x2));
        float L1 = ((x - x0) * (x - x2)) / ((x1 - x0) * (x1 - x2));
        float L2 = ((x - x0) * (x - x1)) / ((x2 - x0) * (x2 - x1));

        return y0 * L0 + y1 * L1 + y2 * L2;
    }

    void UpdateLUT() const
    {
        lut_.resize(256);
        for (int i = 0; i < 256; ++i)
        {
            float x = i / 256.0f;
            float y = Evaluate(x);
            lut_[i] = (uint8_t)(std::clamp(y * 255.0f, 0.0f, 255.0f));
        }
        reload_lut_ = false;
    }
};

#endif
