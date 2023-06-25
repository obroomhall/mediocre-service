#include <leptonica/allheaders.h>
#include <libmediocre/functions/ocr/v1/ocr.hpp>
#include <tesseract/baseapi.h>

namespace mediocre::functions::ocr::v1 {

    using grpc::ServerContext;
    using grpc::Status;

    Status OcrServiceImpl::GetText(
            ServerContext *context,
            const ImageToTextRequest *request,
            ImageToTextResponse *response) {


        return Status::OK;
    }

}// namespace mediocre::functions::ocr::v1
