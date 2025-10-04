#ifndef _IMAGE_VIEWER
#define _IMAGE_VIEWER

#include "ImageManager.h"

class ImageViewerUI
{
public:
    ImageViewerUI(ImageManager &manager) : manager_(manager) {}
    void Draw();
private:
    ImageManager &manager_;
    const int size_ = 500;
};

#endif
