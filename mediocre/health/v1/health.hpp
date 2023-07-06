#ifndef mediocre_health_v1_h
#define mediocre_health_v1_h

#include <mediocre/health/v1/health.grpc.pb.h>

namespace grpc::health::v1 {

    class HealthServiceImpl final : public Health::Service {
        Status Check(
                ServerContext *context,
                const HealthCheckRequest *request,
                HealthCheckResponse *response) override;
    };

}// namespace grpc::health::v1

#endif// mediocre_health_v1_h