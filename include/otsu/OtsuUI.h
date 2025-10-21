#ifndef _OTSU_UI
#define _OTSU_UI

#include "ImageManager.h"

class OtsuUI
{
public:
    OtsuUI(ImageManager &manager) : manager_(manager) {}
    void Draw();
private:
    ImageManager &manager_;
};

#endif
