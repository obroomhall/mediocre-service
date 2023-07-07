#ifndef mediocre_dependency_v1_h
#define mediocre_dependency_v1_h

#include <mediocre/dependency/v1/dependency.grpc.pb.h>

namespace mediocre::dependency::v1 {

    using grpc::Status;
    using grpc::ServerContext;

    class DependencyServiceImpl final : public DependencyService::Service {
        Status CheckOpenCV(
                ServerContext *context,
                const CheckOpenCVRequest *request,
                CheckOpenCVResponse *response) override;
        Status CheckTesseract(
                ServerContext *context,
                const CheckTesseractRequest *request,
                CheckTesseractResponse *response) override;
    };

}// namespace mediocre::dependency::v1

#endif// mediocre_dependency_v1_h