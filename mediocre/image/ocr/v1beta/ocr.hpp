#ifndef mediocre_image_ocr_v1beta_h
#define mediocre_image_ocr_v1beta_h

#include <mediocre/image/ocr/v1beta/ocr.grpc.pb.h>
#include <opencv2/core/mat.hpp>

namespace mediocre::image::ocr::v1beta {

    using grpc::ServerContext;
    using grpc::Status;
    using mediocre::image::v1beta::Image;

    class OcrServiceImpl final : public OcrService::Service {
    public:
        Status GetCharacters(
                ServerContext *context,
                const GetCharactersRequest *request,
                GetCharactersResponse *response) override;
        static std::string GetCharacters(const cv::Mat &input, const GetCharactersParams &params);

    private:
        static std::string GetCharacters(const Image &image, const GetCharactersParams &params);
    };

}// namespace mediocre::image::ocr::v1beta

#endif// mediocre_image_ocr_v1beta_h