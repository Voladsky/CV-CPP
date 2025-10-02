#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include "opencv2/opencv.hpp"
#include <string>

struct Image {
    GLuint texture;
    cv::Mat mat;
    std::string filePath;
};

namespace Processing {
    cv::Mat grayscale(const cv::Mat& input);
}
