#include <mediocre/image/scale/v1beta/scale.hpp>
#include <mediocre/image/v1beta/image.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>

namespace mediocre::image::scale::v1beta {

    Status ScaleServiceImpl::Scale(
            ServerContext *context,
            const ScaleRequest *request,
            ScaleResponse *response) {
        Scale(request->image(), response->mutable_image(), request->params());
        return Status::OK;
    }

    void ScaleServiceImpl::Scale(const Image &image, Image *mutable_image, const ScaleParams &params) {
        const auto decoded = image::v1beta::Decode(image);
        const auto scaled = Scale(decoded, params);
        image::v1beta::Encode(scaled, mutable_image);
    }

    cv::Mat ScaleServiceImpl::Scale(const cv::Mat &input, const ScaleParams &params) {
        if (!params.has_fixed_aspect()) {
            throw std::invalid_argument("No fixed aspect scale params were supplied");
        }
        return ScaleFixedAspect(input, params.fixed_aspect());
    }

    cv::Mat ScaleServiceImpl::ScaleFixedAspect(const cv::Mat &input, const FixedAspectScaleParams &params) {
        if ((params.has_width() && params.width() <= 0) || (params.has_height() && params.height() <= 0)) {
            throw std::invalid_argument("Invalid scale dimensions");
        }

        cv::Mat output;
        if (params.has_width()) {
            const auto aspect_ratio = static_cast<double>(params.width()) / input.cols;
            const auto new_height = static_cast<int>(input.rows * aspect_ratio);
            cv::resize(input, output, cv::Size(params.width(), new_height));
        } else if (params.has_height()) {
            const auto aspect_ratio = static_cast<double>(params.height()) / input.rows;
            const auto new_width = static_cast<int>(input.cols * aspect_ratio);
            cv::resize(input, output, cv::Size(new_width, params.height()));
        } else {
            throw std::invalid_argument("Unknown params supplied");
        }

        return output;
    }

}// namespace mediocre::image::scale::v1beta
