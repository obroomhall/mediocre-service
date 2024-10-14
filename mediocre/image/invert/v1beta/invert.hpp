#ifndef mediocre_image_invert_v1beta_h
#define mediocre_image_invert_v1beta_h

#include <mediocre/image/invert/v1beta/invert.grpc.pb.h>
#include <opencv2/core/mat.hpp>

namespace mediocre::image::invert::v1beta {

    using grpc::ServerContext;
    using grpc::Status;
    using mediocre::image::v1beta::Image;

    class InvertServiceImpl final : public InvertService::Service {
    public:
        Status Invert(
                ServerContext *context,
                const InvertRequest *request,
                InvertResponse *response) override;
        static cv::Mat Invert(const cv::Mat &input, const InvertParams &params);

    private:
        static void Invert(const Image &image, Image *mutable_image, const InvertParams &params);
    };

}// namespace mediocre::image::invert::v1beta

#endif// mediocre_image_invert_v1beta_h