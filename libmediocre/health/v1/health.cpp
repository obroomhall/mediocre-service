#include <libmediocre/health/v1/health.hpp>

namespace grpc::health::v1 {

    using grpc::Status;
    using grpc::ServerContext;

    Status HealthServiceImpl::Check(
            ServerContext *context,
            const HealthCheckRequest *request,
            HealthCheckResponse *response) {
        response->set_status(HealthCheckResponse_ServingStatus_NOT_SERVING);
        return Status::OK;
    }

}// namespace grpc::health::v1
