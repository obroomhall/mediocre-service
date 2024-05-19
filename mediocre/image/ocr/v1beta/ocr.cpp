#include <mediocre/image/v1beta/image.hpp>
#include <leptonica/allheaders.h>
#include <mediocre/image/ocr/v1beta/ocr.hpp>
#include <opencv2/core/mat.hpp>
#include <tesseract/baseapi.h>

namespace mediocre::image::ocr::v1beta {

    Status OcrServiceImpl::GetCharacters(
            ServerContext *context,
            const GetCharactersRequest *request,
            GetCharactersResponse *response) {

        const auto decoded = image::v1beta::Decode(request->image());
        const auto *characters = GetCharacters(decoded);

        response->set_characters(characters);

        return Status::OK;
    }

    // it's worth consulting the opencv-tesseract wrapper
    // https://github.com/opencv/opencv_contrib/blob/4.x/modules/text/src/ocr_tesseract.cpp
    // should we use that as well / instead?

    const char *OcrServiceImpl::GetCharacters(const cv::Mat &input) {
        // Do we need to initialise on every use?
        // Potentially, see https://github.com/tesseract-ocr/tesseract/issues/3109#issuecomment-700509450
        // We may want to initialise a pool of tesseract instances, and then grab from that pool when we want
        //  to process images
        auto *api = new tesseract::TessBaseAPI();
        if (api->Init(nullptr, "eng")) {
            fprintf(stderr, "Could not initialize tesseract.\n");
            exit(1);
        }

        api->SetImage(input.data, input.cols, input.rows, input.channels(), input.step);
        const auto *outText = api->GetUTF8Text();

        api->End();
        delete api;

        return outText;
    }

}// namespace mediocre::image::ocr::v1beta
