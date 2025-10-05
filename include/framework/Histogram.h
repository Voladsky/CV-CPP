#ifndef _HISTOGRAM
#define _HISTOGRAM

#include "ImageManager.h"
#include <vector>

struct ImageHistogram
{
    std::vector<float> b;
    std::vector<float> g;
    std::vector<float> r;
};

class HistogramManager
{
public:
    HistogramManager(const Image &image)
    {
        BuilHistogram(image.original, original);
        BuilHistogram(image.processed, processed);
    }
    const ImageHistogram& GetOriginalHist() const { return original; }
    const ImageHistogram& GetProcessedHist() const { return processed; }

private:
    void BuilHistogram(cv::Mat mat, ImageHistogram &out)
    {
        const int hist_size = 256;
        const float range[] = {0, 256};
        const float *hist_range = {range};
        std::vector<cv::Mat> channels;
        cv::split(mat, channels);
        cv::Mat b_hist, g_hist, r_hist;
        cv::calcHist(&channels[0], 1, 0, cv::Mat(), b_hist, 1, &hist_size, &hist_range);
        cv::calcHist(&channels[1], 1, 0, cv::Mat(), g_hist, 1, &hist_size, &hist_range);
        cv::calcHist(&channels[2], 1, 0, cv::Mat(), r_hist, 1, &hist_size, &hist_range);
        cv::normalize(b_hist, b_hist, 0, 1, cv::NORM_MINMAX);
        cv::normalize(g_hist, g_hist, 0, 1, cv::NORM_MINMAX);
        cv::normalize(r_hist, r_hist, 0, 1, cv::NORM_MINMAX);
        out.b.resize(hist_size);
        out.g.resize(hist_size);
        out.r.resize(hist_size);
        for (int i = 0; i < hist_size; i++) {
            out.b[i] = b_hist.at<float>(i, 0);
            out.g[i] = g_hist.at<float>(i, 0);
            out.r[i] = r_hist.at<float>(i, 0);
        }
    }
    ImageHistogram original;
    ImageHistogram processed;
};
#endif
