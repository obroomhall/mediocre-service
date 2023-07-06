#include <leptonica/allheaders.h>
#include <mediocre/dependency/v1/dependency.hpp>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

namespace mediocre::dependency::v1 {

    using grpc::Status;
    using grpc::ServerContext;

    Status DependencyServiceImpl::CheckOpenCV(
            ServerContext *context,
            const CheckOpenCVRequest *request,
            CheckOpenCVResponse *response) {

        try {
            std::string version(cv::getVersionString());
            response->set_healthy(version.length() > 0);
            response->set_version(version);
        } catch (const std::exception &e) {
            response->set_error(e.what());
        }

        return Status::OK;
    }

    Status DependencyServiceImpl::CheckTesseract(
            ServerContext *context,
            const CheckTesseractRequest *request,
            CheckTesseractResponse *response) {

        try {
            auto *api = new tesseract::TessBaseAPI();

            if (api->Init(nullptr, "eng")) {
                response->set_healthy(false);
                response->set_error("Could not initialize tesseract");
                return Status::OK;
            }

            std::string leptonica_version(getLeptonicaVersion());

            response->set_healthy(leptonica_version.length() > 0);
            response->set_leptonica_version(leptonica_version);

        } catch (const std::exception &e) {
            response->set_error(e.what());
        }

        return Status::OK;
    }

}// namespace mediocre::dependency::v1
