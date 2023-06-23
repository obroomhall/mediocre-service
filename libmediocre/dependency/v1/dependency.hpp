#ifndef dependency_h
#define dependency_h

#include <libmediocre/dependency/v1/dependency.grpc.pb.h>

namespace mediocre::dependency::v1 {

    using namespace grpc;

    class DependencyServiceImpl final : public Dependency::Service {
        Status CheckOpenCV(
                ServerContext *context,
                const OpenCVCheckRequest *request,
                OpenCVCheckResponse *response) override;
    };

}// namespace mediocre::dependency::v1

#endif// dependency_h