#ifndef mediocre_image_ocr_v1beta_h
#define mediocre_image_ocr_v1beta_h

#include <mediocre/image/ocr/v1beta/ocr.grpc.pb.h>

namespace mediocre::image::ocr::v1beta {

    using grpc::ServerContext;
    using grpc::Status;

    class OcrServiceImpl final : public OcrService::Service {
        Status GetCharacters(
                ServerContext *context,
                const GetCharactersRequest *request,
                GetCharactersResponse *response) override;
    };

}// namespace mediocre::image::ocr::v1beta

#endif// mediocre_image_ocr_v1beta_h