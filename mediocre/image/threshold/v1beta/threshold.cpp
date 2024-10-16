#include <mediocre/image/threshold/v1beta/threshold.hpp>
#include <mediocre/image/v1beta/image.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>

namespace mediocre::image::threshold::v1beta {

    Status ThresholdServiceImpl::Threshold(
            ServerContext *context,
            const ThresholdRequest *request,
            ThresholdResponse *response) {
        Threshold(request->image(), response->mutable_image(), request->params());
        return Status::OK;
    }

    void ThresholdServiceImpl::Threshold(const Image &image, Image *mutable_image, const ThresholdParams &params) {
        const auto decoded = image::v1beta::Decode(image);
        const auto thresholded = Threshold(decoded, params);
        image::v1beta::Encode(thresholded, mutable_image);
    }

    cv::Mat ThresholdServiceImpl::Threshold(const cv::Mat &input, const ThresholdParams &params) {
        if (!params.has_colour()) {
            throw std::invalid_argument("No colour threshold params were supplied");
        }
        return ThresholdColour(input, params.colour());
    }

    cv::Mat ThresholdServiceImpl::ThresholdColour(const cv::Mat &input, const ColourThresholdParams &params) {
        if (params.blue().size() != 2 || params.green().size() != 2 || params.red().size() != 2) {
            throw std::invalid_argument("Colour threshold params must have two values for each channel");
        }

        cv::Mat bgrMat;
        cv::cvtColor(input, bgrMat, cv::COLOR_BGRA2BGR);

        const auto lowerColour = cv::Scalar(params.blue().at(0), params.green().at(0), params.red().at(0));
        const auto upperColour = cv::Scalar(params.blue().at(1), params.green().at(1), params.red().at(1));

        cv::Mat mask;
        cv::inRange(bgrMat, lowerColour, upperColour, mask);
        return mask;
    }

}// namespace mediocre::image::threshold::v1beta
