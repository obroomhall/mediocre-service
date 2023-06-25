#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/server_builder.h>
#include <iomanip>
#include <libmediocre/dependency/v1/dependency.hpp>
#include <libmediocre/health/v1/health.hpp>
#include <libmediocre/server/server.hpp>

namespace mediocre::server {

    std::string get_server_address(uint16_t port);
    void register_listener(grpc::ServerBuilder &builder, const std::string &server_address);
    void register_services(grpc::ServerBuilder &builder);
    void start_and_wait(grpc::ServerBuilder &builder);

    void run_server(uint16_t port) {
        std::string server_address = get_server_address(port);
        std::cout << "Building server on " << server_address << std::endl;

        grpc::reflection::InitProtoReflectionServerBuilderPlugin();

        grpc::ServerBuilder builder;
        register_listener(builder, server_address);
        register_services(builder);

        start_and_wait(builder);
    }

    std::string get_server_address(uint16_t port) {
        std::ostringstream server_address_stream;
        server_address_stream << "0.0.0.0:" << port;
        return server_address_stream.str();
    }

    void register_listener(grpc::ServerBuilder &builder, const std::string &server_address) {
        // Listen on the given address without any authentication mechanism.
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    }

    void register_services(grpc::ServerBuilder &builder) {
        // Define services.
        std::vector<grpc::Service *> services;
        services.push_back(new grpc::health::v1::HealthServiceImpl());
        services.push_back(new dependency::v1::DependencyServiceImpl());

        // Register services.
        for (auto service: services) {
            builder.RegisterService(service);
        }
    }

    void start_and_wait(grpc::ServerBuilder &builder) {
        // Assemble the server.
        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

        std::cout << "Server started" << std::endl;

        // Wait for the server to shutdown. Note that some other thread must be
        // responsible for shutting down the server for this call to ever return.
        server->Wait();
    }

}// namespace mediocre::server
