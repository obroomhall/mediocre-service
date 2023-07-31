#ifndef mediocre_image_ocr_v1beta_h
#define mediocre_image_ocr_v1beta_h

#include <mediocre/image/ocr/v1beta/ocr.grpc.pb.h>
#include <tesseract/baseapi.h>

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
        static char* GetCharacters(const uint8_t* input, size_t length);
        static void SaveImage(const uint8_t *input, int32_t length);
    };

}// namespace mediocre::image::ocr::v1beta

#endif// mediocre_image_ocr_v1beta_h