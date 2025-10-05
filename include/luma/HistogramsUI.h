#ifndef _HISTOGRAMS_H
#define _HISTOGRAMS_H

#include "ImageManager.h"

class HistogramsUI
{
public:
    HistogramsUI(ImageManager &manager) : manager_(manager) {}
    void Draw();
private:
    ImageManager &manager_;
};

#endif
