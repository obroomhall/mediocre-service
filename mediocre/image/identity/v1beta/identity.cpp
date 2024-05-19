#include <mediocre/image/v1beta/image.hpp>
#include <mediocre/image/identity/v1beta/identity.hpp>

namespace mediocre::image::identity::v1beta {

    Status IdentityServiceImpl::Protobuf(
            ServerContext *context,
            const GetIdentityRequest *request,
            GetIdentityResponse *response) {
        CopyImageProtobuf(request->image(), response->mutable_image());
        return Status::OK;
    }

    Status IdentityServiceImpl::OpenCV(
            ServerContext *context,
            const GetIdentityRequest *request,
            GetIdentityResponse *response) {
        CopyImageOpenCV(request->image(), response->mutable_image());
        return Status::OK;
    }

    void IdentityServiceImpl::CopyImageProtobuf(const Image& image, Image *mutable_image) {
        mutable_image->CopyFrom(image);
    }

    void IdentityServiceImpl::CopyImageOpenCV(const Image& image, Image *mutable_image) {
        const auto decoded = image::v1beta::Decode(image);
        image::v1beta::Encode(decoded, mutable_image);
    }

}// namespace mediocre::image::ocr::v1beta
