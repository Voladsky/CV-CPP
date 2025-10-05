#ifndef _COLOR_CORRECTION
#define _COLOR_CORRECTION

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
