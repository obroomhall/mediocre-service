#ifndef mediocre_image_crop_v1beta_h
#define mediocre_image_crop_v1beta_h

#include <mediocre/image/crop/v1beta/crop.grpc.pb.h>
#include <opencv2/core/mat.hpp>

namespace mediocre::image::crop::v1beta {

    using grpc::ServerContext;
    using grpc::Status;
    using mediocre::image::v1beta::Image;

    class CropServiceImpl final : public CropService::Service {
    public:
        Status Crop(
                ServerContext *context,
                const CropRequest *request,
                CropResponse *response) override;
        static cv::Mat Crop(const cv::Mat &input, const CropParams &params);

    private:
        static void Crop(const Image &image, Image *mutable_image, const CropParams &params);
        static cv::Mat CropFixed(const cv::Mat &input, const FixedCropParams &params);
    };

}// namespace mediocre::image::crop::v1beta

#endif// mediocre_image_crop_v1beta_h