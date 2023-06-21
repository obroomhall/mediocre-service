#ifndef health_h
#define health_h

#include <grpcpp/health_check_service_interface.h>
#include <libmediocre/dummy/dummy.grpc.pb.h>

namespace mediocre::dummy {

    class DummyServiceImpl final : public Dummy::Service {
        grpc::Status DoThing(
                grpc::ServerContext *context,
                const DummyRequest *request,
                DummyResponse *response) override;
    };

}// namespace mediocre::dummy

#endif// health_h