#ifndef _IMAGE_MANAGER
#define _IMAGE_MANAGER

#include <imgui.h>
#include <opencv2/opencv.hpp>
#include <string>
#include <filesystem>
#include "image_processing.h"
#include "GLTexture.h"

struct Image
{
    std::string name;
    cv::Mat original;
    cv::Mat processed;
    GLTexture orig_texture;
    GLTexture processed_texture;
};

class ImageManager
{

public:
    void AddImage(const fs::path& path);
    void AddFolder(const fs::path& path);
    void SelectImage(const std::string& name);
    void SelectImage(int idx);

    size_t GetImagesSize() const;
    const Image* GetImage(int idx) const;
    const int GetSelectedIdx() const;

    void ProccessCurrent(std::function<cv::Mat(const cv::Mat&)> f);
    void ResetCurrent();
private:
    std::vector<Image> images_;
    int idx_ = -1;
    void AddImage(const std::string& name, const cv::Mat& matrix);
    void ReloadImageTexture(int idx);
};

#endif
