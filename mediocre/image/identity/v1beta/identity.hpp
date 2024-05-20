#ifndef mediocre_image_identity_v1beta_h
#define mediocre_image_identity_v1beta_h

#include <mediocre/image/identity/v1beta/identity.grpc.pb.h>
#include <opencv2/core/mat.hpp>

namespace mediocre::image::identity::v1beta {

    using grpc::ServerContext;
    using grpc::Status;
    using mediocre::image::v1beta::Image;

    class IdentityServiceImpl final : public IdentityService::Service {
    public:
        Status GetIdentity(
                ServerContext *context,
                const GetIdentityRequest *request,
                GetIdentityResponse *response) override;
        static cv::Mat GetIdentity(const cv::Mat &input, const GetIdentityParams &params);

    private:
        static void GetIdentity(const Image &image, Image *mutable_image, const GetIdentityParams &params);
    };

}// namespace mediocre::image::identity::v1beta

#endif// mediocre_image_identity_v1beta_h