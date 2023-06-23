#include <iomanip>
//#include <leptonica/allheaders.h>
#include <libmediocre/dependency/v1/dependency.hpp>
#include <opencv4/opencv2/opencv.hpp>
//#include <tesseract/baseapi.h>

namespace mediocre::dependency::v1 {

    using namespace grpc;

    Status DependencyServiceImpl::CheckOpenCV(
            ServerContext *context,
            const OpenCVCheckRequest *request,
            OpenCVCheckResponse *response) {

        try {
            std::string version(cv::getVersionString());
            response->set_healthy(version.length() > 0);
            response->set_version(version);
        } catch (const std::exception &e) {
            response->set_error(e.what());
        }

        return Status::OK;
    }


    //    int tesseract() {
    //        char *outText;
    //
    //        auto *api = new tesseract::TessBaseAPI();
    //        // Initialize tesseract-ocr with English, without specifying tessdata path
    //        if (api->Init(nullptr, "eng")) {
    //            fprintf(stderr, "Could not initialize tesseract.\n");
    //            exit(1);
    //        }
    //
    //        // Open input image with leptonica library
    //        Pix *image = pixRead("libmediocre/dependency/phototest.tif");
    //        api->SetImage(image);
    //        // Get OCR result
    //        outText = api->GetUTF8Text();
    //        printf("OCR output:\n%s", outText);
    //
    //        // Destroy used object and release memory
    //        api->End();
    //        delete api;
    //        delete[] outText;
    //        pixDestroy(&image);
    //
    //        return 0;
    //    }


}// namespace mediocre::dependency::v1
