#ifndef _IMAGE_SELECTOR
#define _IMAGE_SELECTOR

#include "ImageManager.h"

class ImageSelectorUI
{
public:
    ImageSelectorUI(ImageManager &manager) : manager_(manager) {}
    void Draw();
private:
    ImageManager &manager_;
    float thumbnail_size_ = 100.0f;
};

#endif
