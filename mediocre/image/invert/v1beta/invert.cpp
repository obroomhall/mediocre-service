#include <mediocre/image/invert/v1beta/invert.hpp>
#include <mediocre/image/v1beta/image.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>

namespace mediocre::image::invert::v1beta {

    Status InvertServiceImpl::Invert(
            ServerContext *context,
            const InvertRequest *request,
            InvertResponse *response) {
        Invert(request->image(), response->mutable_image(), request->params());
        return Status::OK;
    }

    void InvertServiceImpl::Invert(const Image &image, Image *mutable_image, const InvertParams &params) {
        const auto decoded = image::v1beta::Decode(image);
        const auto invertd = Invert(decoded, params);
        image::v1beta::Encode(invertd, mutable_image);
    }

    cv::Mat InvertServiceImpl::Invert(const cv::Mat &input, const InvertParams &params) {
        cv::Mat output;
        cv::bitwise_not(input, output);
        return output;
    }

}// namespace mediocre::image::invert::v1beta
