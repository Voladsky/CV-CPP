#ifndef _IMAGE_PROCESSING
#define _IMAGE_PROCESSING

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include "opencv2/opencv.hpp"
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

GLuint matToTexture(const cv::Mat &mat);

#endif
