#include "image_processing.h"

namespace Processing
{
    cv::Mat GrayWorldCorrection(const cv::Mat &image)
    {
        if (image.empty())
            throw std::invalid_argument("Input image is empty");
        cv::Mat result;
        image.convertTo(result, CV_32FC3);
        cv::Scalar channelMeans = cv::mean(result);
        float mean = channelMeans.dot(cv::Scalar(1, 1, 1)) / 3.0f;
        if (channelMeans[0] == 0 || channelMeans[1] == 0 || channelMeans[2] == 0)
        {
            return image.clone();
        }
        float scaleB = mean / channelMeans[0];
        float scaleG = mean / channelMeans[1];
        float scaleR = mean / channelMeans[2];
        std::vector<cv::Mat> channels;
        cv::split(result, channels);
        channels[0] *= scaleB;
        channels[1] *= scaleG;
        channels[2] *= scaleR;
        cv::merge(channels, result);
        result.convertTo(result, image.type());
        return result;
    }

    cv::Mat ReferencePixelCorrection(const cv::Mat &image,
                                     const cv::Scalar &reference_color,
                                     const cv::Scalar &target_color)
    {
        if (image.empty())
            throw std::invalid_argument("Image was empty");
        cv::Mat result;
        image.convertTo(result, CV_32FC3);

        float scaleB = reference_color[0] / target_color[0];
        float scaleG = reference_color[1] / target_color[1];
        float scaleR = reference_color[2] / target_color[2];
        std::vector<cv::Mat> channels;
        cv::split(result, channels);
        channels[0] *= scaleB;
        channels[1] *= scaleG;
        channels[2] *= scaleR;
        cv::merge(channels, result);
        result.convertTo(result, image.type());
        return result;
    }

    cv::Mat LinearStretching(const cv::Mat &image, float minOffset, float maxOffset)
    {
        if (image.empty())
            throw std::invalid_argument("Image was empty");
        cv::Mat result;
        image.convertTo(result, CV_32FC3);

        double minVal, maxVal;
        cv::minMaxLoc(result, &minVal, &maxVal);

        if (maxVal == minVal)
        {
            return image.clone();
        }

        float scale = (maxOffset - minOffset) / (maxVal - minVal);

        result = (result - minVal) * scale;

        result.convertTo(result, image.type());
        return result;
    }
    cv::Mat GammaCorrection(const cv::Mat &image, float gamma, float gain)
    {
        if (image.empty())
            throw std::invalid_argument("Image was empty");
        cv::Mat result;
        image.convertTo(result, CV_32FC3);

        cv::pow(result, gamma, result);
        result = gain * result;

        result.convertTo(result, image.type());
        return result;
    }
    cv::Mat HistogramEqualization(const cv::Mat &image)
    {
        if (image.empty())
        {
            throw std::invalid_argument("Input image is empty");
        }

        cv::Mat result;

        if (image.channels() != 1)
        {
            cv::cvtColor(image, result, cv::COLOR_BGR2GRAY);
        }

        cv::equalizeHist(result, result);

        cv::cvtColor(result, result, cv::COLOR_GRAY2BGR);

        return result;
    }
}
