#include <mediocre/image/crop/v1beta/crop.hpp>
#include <mediocre/image/v1beta/image.hpp>
#include <opencv2/core/mat.hpp>

namespace mediocre::image::crop::v1beta {

    Status CropServiceImpl::Crop(
            ServerContext *context,
            const GetCropRequest *request,
            GetCropResponse *response) {

        const auto decoded = image::v1beta::Decode(request->image());

        const auto &crop_params = request->params();
        const auto crop_region = cv::Rect(crop_params.x(), crop_params.y(), crop_params.width(), crop_params.height());

        const auto cropped = Crop(decoded, crop_region);
        image::v1beta::Encode(cropped, response->mutable_image());

        return Status::OK;
    }

    cv::Mat CropServiceImpl::Crop(const cv::Mat& input, const cv::Rect& region_of_interest) {
        return input(region_of_interest);
    }

} // namespace mediocre::image::ocr::v1beta
