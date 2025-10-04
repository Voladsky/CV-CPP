#ifndef _IMAGE_PROCESSING
#define _IMAGE_PROCESSING

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include "opencv2/opencv.hpp"
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

namespace Processing
{
    cv::Mat GrayWorldCorrection(const cv::Mat &image);
    cv::Mat ReferencePixelCorrection(const cv::Mat &image,
                                     const cv::Scalar &reference_color,
                                     const cv::Scalar &target_color);
    cv::Mat LinearStretching(const cv::Mat &image, float minOffset, float maxOffset);
    cv::Mat GammaCorrection(const cv::Mat &image, float gamma, float gain);
}

#endif
