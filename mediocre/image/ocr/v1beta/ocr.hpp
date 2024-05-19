#ifndef mediocre_image_ocr_v1beta_h
#define mediocre_image_ocr_v1beta_h

#include <mediocre/image/ocr/v1beta/ocr.grpc.pb.h>
#include <opencv2/core/mat.hpp>

namespace mediocre::image::ocr::v1beta {

    using grpc::ServerContext;
    using grpc::Status;

    class OcrServiceImpl final : public OcrService::Service {
    public:
        Status GetCharacters(
                ServerContext *context,
                const GetCharactersRequest *request,
                GetCharactersResponse *response) override;
    private:
        static const char* GetCharacters(const cv::Mat& input);
    };

}// namespace mediocre::image::ocr::v1beta

#endif// mediocre_image_ocr_v1beta_h