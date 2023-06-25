#ifndef mediocre_functions_ocr_v1_h
#define mediocre_functions_ocr_v1_h

#include <libmediocre/functions/ocr/v1/ocr.grpc.pb.h>

namespace mediocre::functions::ocr::v1 {

    using grpc::ServerContext;
    using grpc::Status;

    class OcrServiceImpl final : public Ocr::Service {
        Status GetText(
                ServerContext *context,
                const ImageToTextRequest *request,
                ImageToTextResponse *response) override;
    };

}// namespace mediocre::functions::ocr::v1

#endif// mediocre_functions_ocr_v1_h