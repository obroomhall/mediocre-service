#include <mediocre/image/rotate/v1beta/rotate.hpp>
#include <mediocre/image/v1beta/image.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>

namespace mediocre::image::rotate::v1beta {

    Status RotateServiceImpl::Rotate(
            ServerContext *context,
            const RotateRequest *request,
            RotateResponse *response) {
        Rotate(request->image(), response->mutable_image(), request->params());
        return Status::OK;
    }

    void RotateServiceImpl::Rotate(const Image &image, Image *mutable_image, const RotateParams &params) {
        const auto decoded = image::v1beta::Decode(image);
        const auto rotated = Rotate(decoded, params);
        image::v1beta::Encode(rotated, mutable_image);
    }

    cv::Mat RotateServiceImpl::Rotate(const cv::Mat &input, const RotateParams &params) {
        cv::Point center = cv::Point(input.cols / 2, input.rows / 2);
        cv::Mat rotation = cv::getRotationMatrix2D(center, params.angle(), 1.0);
        cv::Mat output;
        cv::warpAffine(input, output, rotation, input.size());
        return output;
    }

}// namespace mediocre::image::rotate::v1beta
