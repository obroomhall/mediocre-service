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

        auto imageTransformations = getImageTransformations(request->image_transformations());

        auto writeImageResponse = [=](const cv::Mat &mat) {
            TransformResponse response;
            image::v1beta::Encode(mat, response.mutable_image());
            writer->Write(response);
        };

        std::function<cv::Mat(cv::Mat)> imageToImageTransformation = &IdentityServiceImpl::GetIdentityDefault;
        for (const std::function<cv::Mat(cv::Mat)> &transformation: imageTransformations) {
            imageToImageTransformation = [=](const cv::Mat &mat) {
                auto transformed = transformation(imageToImageTransformation(mat));
                writeImageResponse(transformed);
                return transformed;
            };
        }

        if (!request->has_other_transformation()) {
            auto mat = image::v1beta::Decode(request->image());
            auto result = imageToImageTransformation(mat);// don't actually need to use the result
            return Status::OK;
        }

        auto charactersTransformation = getCharactersTransformation(request->other_transformation());

        auto writeCharactersResponse = [=](const std::string &characters) {
            TransformResponse response;
            response.set_characters(characters);
            writer->Write(response);
        };

        auto imageToCharactersTransformation = [=](const cv::Mat &mat) {
            auto transformed = charactersTransformation(imageToImageTransformation(mat));
            writeCharactersResponse(transformed);
            return transformed;
        };

        auto mat = image::v1beta::Decode(request->image());
        auto result = imageToCharactersTransformation(mat);// don't actually need to use the result

        return Status::OK;
    }

    std::vector<std::function<cv::Mat(cv::Mat)>>
    TransformServiceImpl::getImageTransformations(const google::protobuf::RepeatedPtrField<TransformToImage> &trs) {
        std::vector<std::function<cv::Mat(cv::Mat)>> transformations;
        for (const auto &transform: trs) {
            auto transformation = getImageTransformation(transform);
            transformations.emplace_back(transformation);
        }
        return transformations;
    }

    std::function<cv::Mat(cv::Mat)>
    TransformServiceImpl::getImageTransformation(const TransformToImage &transform) {
        switch (transform.transformation_case()) {
            case TransformToImage::kGetIdentity: {
                return [&transform](const cv::Mat &mat) {
                    return IdentityServiceImpl::GetIdentity(mat, transform.get_identity());
                };
            }
            case TransformToImage::kCrop: {
                return [&transform](const cv::Mat &mat) {
                    return CropServiceImpl::Crop(mat, transform.crop());
                };
            }
            default:
                throw std::logic_error("Unrecognised transformation request");
        }
    }

    std::function<std::string(cv::Mat)>
    TransformServiceImpl::getCharactersTransformation(const TransformToOther &transform) {
        switch (transform.transformation_case()) {
            case TransformToOther::kGetCharacters: {
                return [&transform](const cv::Mat &mat) {
                    return OcrServiceImpl::GetCharacters(mat, transform.get_characters());
                };
            }
            default:
                throw std::logic_error("Unrecognised transformation request");
        }
    }

}// namespace mediocre::image::transform::v1beta
