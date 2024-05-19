#ifndef mediocre_image_identity_v1beta_h
#define mediocre_image_identity_v1beta_h

#include <mediocre/image/identity/v1beta/identity.grpc.pb.h>

namespace mediocre::image::identity::v1beta {

    using grpc::ServerContext;
    using grpc::Status;
    using mediocre::image::v1beta::Image;

    class IdentityServiceImpl final : public IdentityService::Service {
    public:
        Status Protobuf(
                ServerContext *context,
                const ProtobufRequest *request,
                ProtobufResponse *response) override;
        Status OpenCV(
                ServerContext *context,
                const OpenCVRequest *request,
                OpenCVResponse *response) override;

    private:
        static void CopyImageProtobuf(const Image &image, Image *mutable_image);
        static void CopyImageOpenCV(const Image &image, Image *mutable_image);
    };

}// namespace mediocre::image::identity::v1beta

#endif// mediocre_image_identity_v1beta_h