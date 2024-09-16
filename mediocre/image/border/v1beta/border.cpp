#include <mediocre/image/border/v1beta/border.hpp>
#include <mediocre/image/v1beta/image.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>

namespace mediocre::image::border::v1beta {

    Status BorderServiceImpl::Border(
            ServerContext *context,
            const BorderRequest *request,
            BorderResponse *response) {
        Border(request->image(), response->mutable_image(), request->params());
        return Status::OK;
    }

    void BorderServiceImpl::Border(const Image &image, Image *mutable_image, const BorderParams &params) {
        const auto decoded = image::v1beta::Decode(image);
        const auto bordered = Border(decoded, params);
        image::v1beta::Encode(bordered, mutable_image);
    }

    cv::Mat BorderServiceImpl::Border(const cv::Mat &input, const BorderParams &params) {
        cv::Mat output;
        cv::copyMakeBorder(
                input, output,
                params.size(), params.size(), params.size(), params.size(),
                cv::BORDER_CONSTANT,
                cv::Scalar(params.colour().red(), params.colour().green(), params.colour().blue()));
        return output;
    }

}// namespace mediocre::image::border::v1beta
