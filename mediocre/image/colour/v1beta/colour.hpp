#ifndef mediocre_image_colour_v1beta_h
#define mediocre_image_colour_v1beta_h

#include <grpcpp/server_context.h>
#include <mediocre/image/colour/v1beta/colour.grpc.pb.h>
#include <opencv2/core/mat.hpp>

namespace mediocre::image::colour::v1beta {

    using grpc::ServerContext;
    using grpc::Status;
    using mediocre::image::v1beta::Image;

    class GetColourServiceImpl final : public GetColourService::Service {
    public:
        Status GetColour(
                ServerContext *context,
                const GetColourRequest *request,
                GetColourResponse *response) override;
        static cv::Vec3b GetColour(const cv::Mat &input, const GetColourParams &params);

    private:
        static void GetColour(const Image &image, GetColourResponse *response, const GetColourParams &params);
    };

}// namespace mediocre::image::colour::v1beta

#endif// mediocre_image_colour_v1beta_h