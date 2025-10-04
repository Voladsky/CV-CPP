#ifndef _GL_TEXTURE
#define _GL_TEXTURE

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include "opencv2/opencv.hpp"

class GLTexture
{
public:
    GLTexture(const cv::Mat &mat)
    {
        cv::Mat matRGBA;
        if (mat.channels() == 1)
            cv::cvtColor(mat, matRGBA, cv::COLOR_GRAY2RGBA);
        else if (mat.channels() == 3)
            cv::cvtColor(mat, matRGBA, cv::COLOR_BGR2RGBA);
        else if (mat.channels() == 4)
            matRGBA = mat;
        else
            throw std::runtime_error("Unsupported channels");
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, matRGBA.cols, matRGBA.rows, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, matRGBA.data);
    }
    GLTexture(const GLTexture &) = delete;
    GLTexture &operator=(const GLTexture &) = delete;
    GLTexture(GLTexture &&other) noexcept : tex(other.tex) {
        other.tex = 0;
    }
    GLTexture &operator=(GLTexture &&other) noexcept {
        if (this != &other) {
            if (tex != 0) {
                glDeleteTextures(1, &tex);
            }
            tex = other.tex;
            other.tex = 0;
        }
        return *this;
    }
    ~GLTexture() {
        if (tex != 0) {
            glDeleteTextures(1, &tex);
        }
    }
    GLuint id() const { return tex; }
private:
    GLuint tex = 0;
};

#endif
