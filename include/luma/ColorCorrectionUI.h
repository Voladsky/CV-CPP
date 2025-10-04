#ifndef _GRAY_WORLD
#define _GRAY_WORLD

#include "ImageManager.h"

class ColorCorrectionUI
{
public:
    ColorCorrectionUI(ImageManager &manager) : manager_(manager) {}
    void Draw();
private:
    ImageManager &manager_;
};

#endif
