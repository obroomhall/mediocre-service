#ifndef mediocre_image_border_v1beta_h
#define mediocre_image_border_v1beta_h

#include <mediocre/image/border/v1beta/border.grpc.pb.h>
#include <opencv2/core/mat.hpp>

namespace mediocre::image::border::v1beta {

    using grpc::ServerContext;
    using grpc::Status;
    using mediocre::image::v1beta::Image;

    class BorderServiceImpl final : public BorderService::Service {
    public:
        Status Border(
                ServerContext *context,
                const BorderRequest *request,
                BorderResponse *response) override;
        static cv::Mat Border(const cv::Mat &input, const BorderParams &params);

    private:
        static void Border(const Image &image, Image *mutable_image, const BorderParams &params);
    };

}// namespace mediocre::image::border::v1beta

#endif// mediocre_image_border_v1beta_h