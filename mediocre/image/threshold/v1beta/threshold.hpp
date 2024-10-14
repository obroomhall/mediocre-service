#ifndef mediocre_image_threshold_v1beta_h
#define mediocre_image_threshold_v1beta_h

#include <mediocre/image/threshold/v1beta/threshold.grpc.pb.h>
#include <opencv2/core/mat.hpp>

namespace mediocre::image::threshold::v1beta {

    using grpc::ServerContext;
    using grpc::Status;
    using mediocre::image::v1beta::Image;

    class ThresholdServiceImpl final : public ThresholdService::Service {
    public:
        Status Threshold(
                ServerContext *context,
                const ThresholdRequest *request,
                ThresholdResponse *response) override;
        static cv::Mat Threshold(const cv::Mat &input, const ThresholdParams &params);

    private:
        static void Threshold(const Image &image, Image *mutable_image, const ThresholdParams &params);
        static cv::Mat ThresholdColour(const cv::Mat &input, const ColourThresholdParams &params);
    };

}// namespace mediocre::image::threshold::v1beta

#endif// mediocre_image_threshold_v1beta_h