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
    cv::Mat LinearCorrection(const cv::Mat &image, float alpha, float beta)
    {
        if (image.empty())
            throw std::invalid_argument("Image was empty");
        cv::Mat result;
        image.convertTo(result, CV_32FC3);
        result = alpha * result + beta;
        result = cv::max(0.0, cv::min(255.0, result));
        result.convertTo(result, image.type());
        return result;
    }
    cv::Mat GammaCorrection(const cv::Mat &image, float gamma, float gain)
    {
        if (image.empty())
            throw std::invalid_argument("Image was empty");
        cv::Mat result;
        image.convertTo(result, CV_32FC3, 1.0 / 255.0);

        cv::pow(result, gamma, result);
        result = gain * result * 255.0;

        result = cv::max(0.0, cv::min(255.0, result));

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

    cv::Mat ToGrayscale(const cv::Mat &image)
    {
        cv::Mat result;
        cv::cvtColor(image, result, cv::COLOR_BGR2GRAY);
        return result;
    }
    cv::Mat BrightnessQuantization(const cv::Mat &input, int levels)
    {
        cv::Mat gray = ToGrayscale(input);
        cv::Mat quantized = gray.clone();

        int step = 256 / levels;

        for (int i = 0; i < gray.rows; i++)
        {
            for (int j = 0; j < gray.cols; j++)
            {
                uchar pixel = gray.at<uchar>(i, j);
                uchar new_pixel = (pixel / step) * step;
                quantized.at<uchar>(i, j) = new_pixel;
            }
        }

        return quantized;
    }
    cv::Mat GlobalOtsu(const cv::Mat &input)
    {
        cv::Mat gray = ToGrayscale(input);
        cv::Mat binary;
        double thresh = cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
        return binary;
    }

    cv::Mat LocalOtsu(const cv::Mat &input, int blockSize)
    {
        cv::Mat gray = ToGrayscale(input);
        cv::Mat result = cv::Mat::zeros(gray.size(), gray.type());

        for (int i = 0; i < gray.rows; i += blockSize)
        {
            for (int j = 0; j < gray.cols; j += blockSize)
            {
                int endRow = std::min(i + blockSize, gray.rows);
                int endCol = std::min(j + blockSize, gray.cols);
                cv::Rect region(j, i, endCol - j, endRow - i);
                cv::Mat localRegion = gray(region);
                cv::Mat localBinary;
                double localThresh = cv::threshold(localRegion, localBinary, 0, 255,
                                                   cv::THRESH_BINARY | cv::THRESH_OTSU);
                localBinary.copyTo(result(region));
            }
        }
        return result;
    }

    void recursive_otsu(const cv::Mat &img, std::vector<double> &thresholds, int min_size = 64, int max_depth = 4, int current_depth = 0)
    {
        int h = img.rows;
        int w = img.cols;

        if (h < min_size || w < min_size || current_depth > max_depth)
        {
            return;
        }

        cv::Mat binary;
        double threshold_val = cv::threshold(img, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
        thresholds.push_back(threshold_val);

        cv::Mat labels, stats, centroids;
        int num_labels = cv::connectedComponentsWithStats(binary, labels, stats, centroids);

        if (num_labels <= 1)
        {
            return;
        }

        for (int label = 1; label < num_labels; label++)
        {
            // Create mask for current label
            cv::Mat mask = (labels == label);

            // Check if mask has any white pixels
            if (cv::countNonZero(mask) == 0)
            {
                continue;
            }

            // Find bounding rectangle for the connected component
            cv::Rect bbox;
            cv::Mat nonZeroCoords;
            cv::findNonZero(mask, nonZeroCoords);
            bbox = cv::boundingRect(nonZeroCoords);

            // Extract sub-image
            cv::Mat sub_img = img(bbox);

            if (sub_img.rows > min_size && sub_img.cols > min_size)
            {
                recursive_otsu(sub_img, thresholds, min_size, max_depth, current_depth + 1);
            }
        }
    }

    std::vector<double> recursive_otsu_wrapper(const cv::Mat &img, int min_size = 64, int max_depth = 4)
    {
        std::vector<double> thresholds;
        recursive_otsu(img, thresholds, min_size, max_depth, 0);
        return thresholds;
    }

    cv::Mat otsu_thresh_quantize(const cv::Mat &img, const std::vector<double> &thresholds)
    {
        std::vector<double> thresholds_sorted = thresholds;
        std::sort(thresholds_sorted.begin(), thresholds_sorted.end());

        cv::Mat quantized = cv::Mat::zeros(img.size(), img.type());
        double prev_thresh = 0;

        for (size_t i = 0; i < thresholds_sorted.size(); i++)
        {
            double thresh = thresholds_sorted[i];
            cv::Mat mask = (img > prev_thresh) & (img <= thresh);
            quantized.setTo(static_cast<uchar>(thresh), mask);
            prev_thresh = thresh;
        }

        cv::Mat final_mask = (img > thresholds_sorted.back());
        quantized.setTo(255, final_mask);

        return quantized;
    }

    cv::Mat HierarchicalOtsu(const cv::Mat &img, int minBlockSize)
    {
        cv::Mat gray = ToGrayscale(img);
        std::vector<double> thresh = recursive_otsu_wrapper(gray, minBlockSize);
        return otsu_thresh_quantize(gray, thresh);
    }

    cv::Mat ColorImageQuant(const cv::Mat &image, int k)
    {
        if (image.empty())
        {
            throw std::invalid_argument("Input image is empty");
        }

        cv::Mat data = image.reshape(1, image.rows * image.cols);
        data.convertTo(data, CV_32F);

        cv::Mat labels, centers;
        cv::TermCriteria criteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 10, 1.0);
        kmeans(data, k, labels, criteria, 3, cv::KMEANS_PP_CENTERS, centers);

        cv::Mat quantized = data.clone();
        for (int i = 0; i < data.rows; i++)
        {
            int cluster_idx = labels.at<int>(i);
            quantized.at<float>(i, 0) = centers.at<float>(cluster_idx, 0);
            quantized.at<float>(i, 1) = centers.at<float>(cluster_idx, 1);
            quantized.at<float>(i, 2) = centers.at<float>(cluster_idx, 2);
        }

        // Reshape back to original image dimensions and convert back to 8UC3
        quantized = quantized.reshape(3, image.rows);
        quantized.convertTo(quantized, CV_8U);

        return quantized;
    }
}
