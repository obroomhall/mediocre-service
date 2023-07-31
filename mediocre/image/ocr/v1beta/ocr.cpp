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

        const auto& image_data_string = request->image().image_data();

        const auto* image_data = reinterpret_cast<const uint8_t*>(image_data_string.c_str());
        const auto* characters = GetCharacters(image_data, image_data_string.length());

        response->set_characters(characters);

        return Status::OK;
    }

    char* OcrServiceImpl::GetCharacters(const uint8_t* input, size_t length)
    {
        // Do we need to initialise on every use?
        auto *api = new tesseract::TessBaseAPI();
        if (api->Init(nullptr, "eng")) {
            fprintf(stderr, "Could not initialize tesseract.\n");
            exit(1);
        }

        Pix *image = pixReadMemPng(input, length);
        api->SetImage(image);
        char *outText = api->GetUTF8Text();

        api->End();
        delete api;
        pixDestroy(&image);

        return outText;
    }

    void OcrServiceImpl::SaveImage(const uint8_t* input, int32_t length)
    {
        auto *api = new tesseract::TessBaseAPI();
        if (api->Init(nullptr, "eng")) {
            fprintf(stderr, "Could not initialize tesseract.\n");
            exit(1);
        }

        Pix *image = pixReadMemPng(input, length);
        pixWrite("/tmp/mediocre/out.png", image, pixChooseOutputFormat(image));

        // Destroy used object and release memory
        api->End();
        delete api;
        pixDestroy(&image);
    }
    

}// namespace mediocre::image::ocr::v1beta
