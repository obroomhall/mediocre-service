#ifndef mediocre_image_rotate_v1beta_h
#define mediocre_image_rotate_v1beta_h

#include <mediocre/image/rotate/v1beta/rotate.grpc.pb.h>
#include <opencv2/core/mat.hpp>

namespace mediocre::image::rotate::v1beta {

    using grpc::ServerContext;
    using grpc::Status;
    using mediocre::image::v1beta::Image;

    class RotateServiceImpl final : public RotateService::Service {
    public:
        Status Rotate(
                ServerContext *context,
                const RotateRequest *request,
                RotateResponse *response) override;
        static cv::Mat Rotate(const cv::Mat &input, const RotateParams &params);

    private:
        static void Rotate(const Image &image, Image *mutable_image, const RotateParams &params);
    };

}// namespace mediocre::image::rotate::v1beta

#endif// mediocre_image_rotate_v1beta_h