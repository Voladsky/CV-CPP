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
    // Lab02 - Luma
    cv::Mat GrayWorldCorrection(const cv::Mat &image);
    cv::Mat ReferencePixelCorrection(const cv::Mat &image,
                                     const cv::Scalar &reference_color,
                                     const cv::Scalar &target_color);
    cv::Mat LinearStretching(const cv::Mat &image, float minOffset, float maxOffset);
    cv::Mat GammaCorrection(const cv::Mat &image, float gamma, float gain);
    cv::Mat HistogramEqualization(const cv::Mat &image);
    cv::Mat LinearCorrection(const cv::Mat &image, float alpha, float beta);

    // Lab03 - Otsu
    cv::Mat ToGrayscale(const cv::Mat &image);
    cv::Mat BrightnessQuantization(const cv::Mat& input, int levels);
    cv::Mat GlobalOtsu(const cv::Mat& input);
    cv::Mat LocalOtsu(const cv::Mat& input, int blockSize);
    cv::Mat HierarchicalOtsu(const cv::Mat& input, int minBlockSize);
    cv::Mat OtsuThreshQuantization(const cv::Mat& input, int levels);
    cv::Mat ColorImageQuant(const cv::Mat &image, int k);
}

#endif
