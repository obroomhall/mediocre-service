#include <mediocre/image/identity/v1beta/identity.hpp>

namespace mediocre::image::identity::v1beta {

    Status IdentityServiceImpl::Identity(
            ServerContext *context,
            const GetIdentityRequest *request,
            GetIdentityResponse *response) {

        const auto& image = request->image();

        CopyImage(&image, response->mutable_image());

        return Status::OK;
    }

    void IdentityServiceImpl::CopyImage(const Image *image, Image *mutableImage) {
        mutableImage->CopyFrom(*image);
    }

}// namespace mediocre::image::ocr::v1beta
