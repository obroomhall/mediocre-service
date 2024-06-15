#include <mediocre/image/colour/v1beta/colour.hpp>
#include <mediocre/image/v1beta/image.hpp>
#include <opencv2/core/mat.hpp>

namespace mediocre::image::colour::v1beta {

    Status GetColourServiceImpl::GetColour(
            ServerContext *context,
            const GetColourRequest *request,
            GetColourResponse *response) {
        GetColour(request->image(), response, request->params());
        return Status::OK;
    }

    void GetColourServiceImpl::GetColour(const Image &image, GetColourResponse *response, const GetColourParams &params) {
        const auto decoded = image::v1beta::Decode(image);
        const auto colour = GetColour(decoded, params);
        response->set_blue(colour[0]);
        response->set_green(colour[1]);
        response->set_red(colour[2]);
    }

    cv::Vec3b GetColourServiceImpl::GetColour(const cv::Mat &input, const GetColourParams &params) {
        return input.at<cv::Vec3b>(params.y(), params.x());
    }

}// namespace mediocre::image::colour::v1beta
