#include "LumaApplication.h"


GLuint matToTexture(const cv::Mat &mat) {
    cv::Mat matRGBA;
    if (mat.channels() == 1)
        cv::cvtColor(mat, matRGBA, cv::COLOR_GRAY2RGBA);
    else if (mat.channels() == 3)
        cv::cvtColor(mat, matRGBA, cv::COLOR_BGR2RGBA);
    else if (mat.channels() == 4)
        matRGBA = mat;
    else
        throw std::runtime_error("Unsupported channels");

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, matRGBA.cols, matRGBA.rows, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, matRGBA.data);

    return tex;
}
