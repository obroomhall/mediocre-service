#include <grpcpp/ext/health_check_service_server_builder_option.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <iomanip>
#include <libmediocre/dummy/dummy.hpp>
#include <libmediocre/main/main.hpp>

void RunServer(uint16_t port) {
    std::ostringstream server_address_stream;
    server_address_stream << "0.0.0.0:" << port;
    std::string server_address = server_address_stream.str();

    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    grpc::ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

    // Define services.
    mediocre::dummy::DummyServiceImpl dummy_service;

    // Register services.
    builder.RegisterService(&dummy_service);

    // Finally assemble the server.
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // Setup health checks.
//    grpc::HealthCheckServiceInterface *service = server->GetHealthCheckService();
//    service->SetServingStatus("healthy_service", true);

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

int main(int argc, char *argv[]) {
    RunServer(50051);
    return EXIT_SUCCESS;
}
