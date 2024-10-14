#include "mediocre_numeric_display.hpp"
#include <leptonica/allheaders.h>
#include <mediocre/image/ocr/v1beta/ocr.hpp>
#include <mediocre/image/v1beta/image.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <tesseract/baseapi.h>

namespace mediocre::image::ocr::v1beta {

    Status OcrServiceImpl::GetCharacters(
            ServerContext *context,
            const GetCharactersRequest *request,
            GetCharactersResponse *response) {
        GetCharacters(request->image(), response->mutable_characters(), request->params());
        return Status::OK;
    }

    void OcrServiceImpl::GetCharacters(const Image &image, std::string *mutable_string, const GetCharactersParams &params) {
        const auto decoded = image::v1beta::Decode(image);
        const auto characters = GetCharacters(decoded, params);
        mutable_string->append(characters);
    }

    std::string OcrServiceImpl::GetCharacters(const cv::Mat &input, const GetCharactersParams &params) {
        if (params.has_tesseract_params()) {
            return Tesseract(input, params.tesseract_params());
        } else if (params.has_mediocre_params()) {
            return Mediocre(input, params.mediocre_params());
        }

        throw std::invalid_argument("No ocr engine was supplied");
    }

    std::string OcrServiceImpl::Tesseract(const cv::Mat &input, const TesseractParams &params) {
        // it's worth consulting the opencv-tesseract wrapper
        // https://github.com/opencv/opencv_contrib/blob/4.x/modules/text/src/ocr_tesseract.cpp
        // should we use that as well / instead?

        // Do we need to initialise on every use?
        // Potentially, see https://github.com/tesseract-ocr/tesseract/issues/3109#issuecomment-700509450
        // We may want to initialise a pool of tesseract instances, and then grab from that pool when we want
        //  to process images
        auto *api = new tesseract::TessBaseAPI();
        if (api->Init(nullptr, "eng")) {
            fprintf(stderr, "Could not initialize tesseract.\n");
            exit(1);
        }

        //        api->SetVariable("tessedit_char_whitelist", "0123456789");
        api->SetImage(input.data, input.cols, input.rows, input.channels(), input.step);
        const auto *outText = api->GetUTF8Text();

        api->End();
        delete api;

        return outText;
    }

    std::string OcrServiceImpl::Mediocre(const cv::Mat &input, const MediocreParams &params) {
        if (params.has_numeric_display_params()) {
            return MediocreNumericDisplay::GetCharacters(input, params.numeric_display_params());
        }

        throw std::invalid_argument("No method supplied for mediocre ocr");
    }

}// namespace mediocre::image::ocr::v1beta
