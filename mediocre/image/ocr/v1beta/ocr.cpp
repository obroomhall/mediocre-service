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

        std::ostringstream characters;
        const image::v1beta::Image& image = request->image();
        characters << image.height() << "h " << image.width() << "w " << image.image_data().length();

        response->set_characters(characters.str());

        return Status::OK;
    }

}// namespace mediocre::image::ocr::v1beta
