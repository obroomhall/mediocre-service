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

        std::vector<std::function<cv::Mat(cv::Mat)>> transformations;
        for (const TransformToImage &transform: request->image_transformations()) {
            switch (transform.transformation_case()) {
                case TransformToImage::kGetIdentity: {
                    auto transformation = [&transform](const cv::Mat &mat) {
                        return IdentityServiceImpl::GetIdentity(mat, transform.get_identity());
                    };
                    transformations.emplace_back(transformation);
                    break;
                }
                case TransformToImage::kCrop: {
                    auto transformation = [&transform](const cv::Mat &mat) {
                        return CropServiceImpl::Crop(mat, transform.crop());
                    };
                    transformations.emplace_back(transformation);
                    break;
                }
                default:
                    return {grpc::StatusCode::UNIMPLEMENTED, "Unrecognised transformation request"};
            }
        }

        auto mat = image::v1beta::Decode(request->image());
        for (const std::function<cv::Mat(cv::Mat)> &transformation: transformations) {
            mat = transformation(mat);
            TransformResponse response;
            image::v1beta::Encode(mat, response.mutable_image());
            writer->Write(response);
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
