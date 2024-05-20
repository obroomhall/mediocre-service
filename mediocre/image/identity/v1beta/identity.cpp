#include <mediocre/image/identity/v1beta/identity.hpp>
#include <mediocre/image/v1beta/image.hpp>

namespace mediocre::image::identity::v1beta {

    Status IdentityServiceImpl::GetIdentity(
            ServerContext *context,
            const GetIdentityRequest *request,
            GetIdentityResponse *response) {
        GetIdentity(request->image(), response->mutable_image(), request->params());
        return Status::OK;
    }

    void IdentityServiceImpl::GetIdentity(const Image &image, Image *mutable_image, const GetIdentityParams &params) {
        const auto decoded = image::v1beta::Decode(image);
        image::v1beta::Encode(decoded, mutable_image);
    }

    cv::Mat IdentityServiceImpl::GetIdentity(const cv::Mat &input, const GetIdentityParams &params) {
        return input;
    }

    cv::Mat IdentityServiceImpl::GetIdentityDefault(const cv::Mat &input) {
        return input;
    }

}// namespace mediocre::image::identity::v1beta
