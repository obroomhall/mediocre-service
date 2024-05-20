#include <mediocre/image/crop/v1beta/crop.hpp>
#include <mediocre/image/v1beta/image.hpp>
#include <opencv2/core/mat.hpp>

namespace mediocre::image::crop::v1beta {

    Status CropServiceImpl::Crop(
            ServerContext *context,
            const CropRequest *request,
            CropResponse *response) {
        Crop(request->image(), response->mutable_image(), request->params());
        return Status::OK;
    }

    void CropServiceImpl::Crop(const Image &image, Image *mutable_image, const CropParams &params) {
        const auto decoded = image::v1beta::Decode(image);
        const auto cropped = Crop(decoded, params);
        image::v1beta::Encode(cropped, mutable_image);
    }

    cv::Mat CropServiceImpl::Crop(const cv::Mat &input, const CropParams &params) {
        if (!params.has_fixed()) {
            throw std::invalid_argument("No fixed crop params were supplied");
        }
        return CropFixed(input, params.fixed());
    }

    cv::Mat CropServiceImpl::CropFixed(const cv::Mat &input, const FixedCropParams &params) {
        const auto crop_region = cv::Rect(params.x(), params.y(), params.width(), params.height());
        return input(crop_region);
    }

}// namespace mediocre::image::crop::v1beta
