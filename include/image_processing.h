#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include "opencv2/opencv.hpp"
#include <string>
#include <vector>

struct ImageUnit {
    GLuint texture;
    cv::Mat mat;
    std::string filePath;
};

namespace Processing {
    cv::Mat grayscale(const cv::Mat& input);
}

class ImageProcessor {
public:
    ImageProcessor();
    void selectImage(int idx);
    void loadImage(const std::string& path);
private:
    std::vector<ImageUnit> images;
    int idx = -1;
};
