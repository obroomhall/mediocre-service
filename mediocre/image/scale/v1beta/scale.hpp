#ifndef mediocre_image_scale_v1beta_h
#define mediocre_image_scale_v1beta_h

#include <mediocre/image/scale/v1beta/scale.grpc.pb.h>
#include <opencv2/core/mat.hpp>

namespace mediocre::image::scale::v1beta {

    using grpc::ServerContext;
    using grpc::Status;
    using mediocre::image::v1beta::Image;

    class ScaleServiceImpl final : public ScaleService::Service {
    public:
        Status Scale(
                ServerContext *context,
                const ScaleRequest *request,
                ScaleResponse *response) override;
        static cv::Mat Scale(const cv::Mat &input, const ScaleParams &params);

    private:
        static void Scale(const Image &image, Image *mutable_image, const ScaleParams &params);
        static cv::Mat ScaleFixedAspect(const cv::Mat &input, const FixedAspectScaleParams &params);
    };

}// namespace mediocre::image::scale::v1beta

#endif// mediocre_image_scale_v1beta_h