#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/server_builder.h>
#include <iomanip>
#include <libmediocre/dependency/v1/dependency.hpp>
#include <libmediocre/health/v1/health.hpp>
#include <libmediocre/main/main.hpp>
#include <libmediocre/server/server.hpp>

namespace mediocre::server {

    void run(uint16_t port) {
        std::ostringstream server_address_stream;
        server_address_stream << "0.0.0.0:" << port;
        std::string server_address = server_address_stream.str();

        // Why doesn't the default health check server work?
        // grpc::EnableDefaultHealthCheckService(true);
        grpc::reflection::InitProtoReflectionServerBuilderPlugin();
        grpc::ServerBuilder builder;

        // Listen on the given address without any authentication mechanism.
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

        // Define services.
        grpc::health::v1::HealthServiceImpl health_service;
        mediocre::dependency::v1::DependencyServiceImpl dependency_service;

        // Register services.
        builder.RegisterService(&health_service);
        builder.RegisterService(&dependency_service);

        // Finally assemble the server.
        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
        std::cout << "Server listening on " << server_address << std::endl;

        // Setup health checks.
        //    grpc::HealthCheckServiceInterface *server = server->GetHealthCheckService();
        //    server->SetServingStatus("healthy_service", true);

        // Wait for the server to shutdown. Note that some other thread must be
        // responsible for shutting down the server for this call to ever return.
        server->Wait();
    }

}// namespace mediocre::server
