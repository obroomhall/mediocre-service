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

        // This is now completely over-engineered, and almost certainly doesn't provide any benefit
        // over a standard iterative approach.

        auto imageTransformations = getImageTransformations(request->image_transformations());

        auto writeImageResponse = [=](const cv::Mat &mat, double elapsed) {
            TransformResponse response;
            image::v1beta::Encode(mat, response.mutable_image());
            response.set_elapsed(elapsed);
            writer->Write(response);
        };

        auto imageToImageTransformation = compose(imageTransformations, writeImageResponse);

        if (!request->has_other_transformation()) {
            auto mat = image::v1beta::Decode(request->image());
            auto result = imageToImageTransformation(mat);// don't actually need to use the result
            return Status::OK;
        }

        auto charactersTransformation = getCharactersTransformation(request->other_transformation());

        auto writeCharactersResponse = [=](const std::string &characters, double elapsed) {
            TransformResponse response;
            response.set_characters(characters);
            response.set_elapsed(elapsed);
            writer->Write(response);
        };

        auto imageToCharactersTransformation = compose<cv::Mat, std::string>(imageToImageTransformation, charactersTransformation, writeCharactersResponse);

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

    std::function<cv::Mat(cv::Mat)>
    TransformServiceImpl::compose(const std::vector<std::function<cv::Mat(cv::Mat)>> &functions, const std::function<void(cv::Mat, double)> &onTransform) {
        auto composed = std::function<cv::Mat(cv::Mat)>(&IdentityServiceImpl::GetIdentityDefault);
        compose(composed, functions, onTransform);
        return composed;
    }

    template<typename Type>
    void
    TransformServiceImpl::compose(std::function<Type(Type)> &composedFunction, const std::vector<std::function<Type(Type)>> &functions, const std::function<void(Type, double)> &onTransform) {
        for (const auto &function: functions) {
            composedFunction = compose(composedFunction, function, onTransform);
        }
    }
    template<typename IN, typename OUT>
    std::function<OUT(IN)>
    TransformServiceImpl::compose(const std::function<IN(IN)> &composedFunction, const std::function<OUT(IN)> &function, const std::function<void(OUT, double)> &onTransform) {
        return [=](const IN &input) {
            auto previousValue = composedFunction(input);

            auto before = std::chrono::high_resolution_clock::now();
            auto newValue = function(previousValue);
            auto after = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> elapsed = after - before;

            onTransform(newValue, elapsed.count());
            return newValue;
        };
    }

}// namespace mediocre::image::transform::v1beta
