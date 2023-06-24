#ifndef health_h
#define health_h

#include <libmediocre/health/v1/health.grpc.pb.h>

namespace grpc::health::v1 {

    class HealthServiceImpl final : public Health::Service {
        Status Check(
                ServerContext *context,
                const HealthCheckRequest *request,
                HealthCheckResponse *response) override;
    };

}// namespace grpc::health::v1

#endif// health_h