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
        try {
            const auto lowerColour = cv::Scalar(params.lower().blue(), params.lower().green(), params.lower().red());
            const auto upperColour = cv::Scalar(params.upper().blue(), params.upper().green(), params.upper().red());
            
            cv::Mat mask;
            cv::inRange(input, lowerColour, upperColour, mask);
            return mask;

            cv::Mat output = cv::Mat::zeros(input.size(), input.type());
            cv::copyTo(input, output, mask);
            return output;
        } catch (std::exception &e) {
            std::cout << "Exception " << e.what() << std::endl;
            throw e;
        }
    }

}// namespace mediocre::image::threshold::v1beta
