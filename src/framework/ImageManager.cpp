#include "ImageManager.h"

void ImageManager::AddImage(const fs::path &path) {

    if (!path.has_filename()) throw std::runtime_error("File not found");
    const cv::Mat matrix = cv::imread(path.generic_string());
    AddImage(path.filename().string(), matrix);
}

void ImageManager::AddImage(const std::string& name, const cv::Mat& matrix) {
    images_.push_back({ name, matrix, matrix, GLTexture(matrix), GLTexture(matrix) });
}

void ImageManager::AddFolder(const fs::path &path) {
    if (path.has_filename()) throw std::runtime_error("Expected directory, path was found");
    for (const auto& file: fs::directory_iterator(path)) {
        AddImage(file);
    }
}

void ImageManager::SelectImage(const std::string &name) {
    auto it = std::find_if(images_.begin(), images_.end(), [&name](const Image& x) { return x.name == name; });
    if (it == images_.end()) throw std::out_of_range("No image with the name");
}

void ImageManager::SelectImage(int idx) {
    if (idx < 0 || idx > images_.size()) throw std::out_of_range("Image size less than idx");
    idx_ = idx;
}

size_t ImageManager::GetImagesSize() const {
    return images_.size();
}

const Image* ImageManager::GetImage(int idx) const {
    return &images_.at(idx);
}

const int ImageManager::GetSelectedIdx() const {
    return idx_;
}

void ImageManager::ProccessCurrent(std::function<cv::Mat(const cv::Mat&)> f) {
    images_[idx_].processed = f(images_[idx_].original);
    ReloadImageTexture(idx_);
}

void ImageManager::ResetCurrent() {
    images_[idx_].processed = images_[idx_].original.clone();
    ReloadImageTexture(idx_);
}

void ImageManager::ReloadImageTexture(int idx) {
    GLTexture tex1(images_[idx].original);
    GLTexture tex2(images_[idx].processed);

    images_[idx].orig_texture = std::move(tex1);
    images_[idx].processed_texture = std::move(tex2);
}
