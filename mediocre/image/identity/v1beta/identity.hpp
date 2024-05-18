#ifndef mediocre_image_identity_v1beta_h
#define mediocre_image_identity_v1beta_h

#include <mediocre/image/identity/v1beta/identity.grpc.pb.h>

namespace mediocre::image::identity::v1beta {

    using grpc::ServerContext;
    using grpc::Status;
    using mediocre::image::v1beta::Image;

    class IdentityServiceImpl final : public IdentityService::Service {
    public:
        Status Identity(
                ServerContext *context,
                const GetIdentityRequest *request,
                GetIdentityResponse *response) override;
    private:
        static void CopyImage(const Image *image, Image *mutableImage);
    };

}// namespace mediocre::image::identity::v1beta

#endif// mediocre_image_identity_v1beta_h