#include <csignal>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <mediocre/dependency/v1/dependency.hpp>
#include <mediocre/image/ocr/v1beta/ocr.hpp>
#include <mediocre/server/server.hpp>

namespace mediocre::server {

    Server::Server(uint16_t port) {
        server_address = get_server_address(port);
    }

    void Server::run_server() {
        std::cout << "Building server on " << server_address << "." << std::endl;

        grpc::EnableDefaultHealthCheckService(true);
        grpc::reflection::InitProtoReflectionServerBuilderPlugin();

        grpc::ServerBuilder builder;
        register_listener(builder, server_address);
        register_services(builder);

        server = builder.BuildAndStart();
        std::cout << "Server started." << std::endl;

        server->Wait();
    }

    std::string Server::get_server_address(uint16_t port) {
        std::ostringstream server_address_stream;
        server_address_stream << "0.0.0.0:" << port;
        return server_address_stream.str();
    }

    void Server::register_listener(grpc::ServerBuilder &builder, const std::string &server_address) {
        // Listen on the given address without any authentication mechanism.
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    }

    void Server::register_services(grpc::ServerBuilder &builder) {
        // Define services.
        std::vector<grpc::Service *> services({
                new dependency::v1::DependencyServiceImpl(),
                new image::ocr::v1beta::OcrServiceImpl(),
        });

        // Register services.
        for (auto service: services) {
            builder.RegisterService(service);
        }

        std::cout << "Registered " << services.size() << " services." << std::endl;
    }

    void Server::shutdown_server() {
        std::cout << "Shutting down server." << std::endl;
        server->Shutdown();
    }

}// namespace mediocre::server
