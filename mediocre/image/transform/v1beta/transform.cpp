#include <mediocre/image/crop/v1beta/crop.hpp>
#include <mediocre/image/identity/v1beta/identity.hpp>
#include <mediocre/image/ocr/v1beta/ocr.hpp>
#include <mediocre/image/transform/v1beta/transform.hpp>
#include <mediocre/image/v1beta/image.hpp>
#include <opencv2/core/mat.hpp>

namespace mediocre::image::transform::v1beta {

    using mediocre::image::crop::v1beta::CropServiceImpl;
    using mediocre::image::identity::v1beta::IdentityServiceImpl;
    using mediocre::image::ocr::v1beta::OcrServiceImpl;

    Status TransformServiceImpl::Transform(
            ServerContext *context,
            const TransformRequest *request,
            ServerWriter<TransformResponse> *writer) {

        auto mat = image::v1beta::Decode(request->image());

        for (const TransformToImage &transform: request->image_transformations()) {
            switch (transform.transformation_case()) {
                case TransformToImage::kGetIdentity: {
                    mat = IdentityServiceImpl::GetIdentity(mat, transform.get_identity());
                    TransformResponse response;
                    image::v1beta::Encode(mat, response.mutable_image());
                    writer->Write(response);
                    break;
                }
                case TransformToImage::kCrop: {
                    mat = CropServiceImpl::Crop(mat, transform.crop());
                    TransformResponse response;
                    image::v1beta::Encode(mat, response.mutable_image());
                    writer->Write(response);
                    break;
                }
                default:
                    return {grpc::StatusCode::UNIMPLEMENTED, "Unrecognised transformation request"};
            }
        }

        if (request->has_other_transformation()) {
            auto &other = request->other_transformation();
            TransformResponse response;
            switch (other.transformation_case()) {
                case TransformToOther::kGetCharacters: {
                    auto characters = OcrServiceImpl::GetCharacters(mat, other.get_characters());
                    response.set_characters(characters);
                    writer->Write(response);
                    break;
                }
                default:
                    return {grpc::StatusCode::UNIMPLEMENTED, "Unrecognised transformation request"};
            }
        }

        return Status::OK;
    }

}// namespace mediocre::image::transform::v1beta
