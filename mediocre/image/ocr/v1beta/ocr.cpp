#include <leptonica/allheaders.h>
#include <mediocre/image/ocr/v1beta/ocr.hpp>
#include <tesseract/baseapi.h>

namespace mediocre::image::ocr::v1beta {

    using grpc::ServerContext;
    using grpc::Status;

    Status OcrServiceImpl::GetCharacters(
            ServerContext *context,
            const GetCharactersRequest *request,
            GetCharactersResponse *response) {


        return Status::OK;
    }

}// namespace mediocre::image::ocr::v1beta
