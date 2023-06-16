#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <iomanip>
#include <libmediocre/main/main.hpp>

namespace mediocre::main {
    // Logic and data behind the server's behavior.
    class GreeterServiceImpl final : public Greeter::Service {
        grpc::Status SayHello(grpc::ServerContext *context, const HelloRequest *request,
                              HelloReply *reply) override {
            std::string prefix("Hello ");
            std::string suffix(" from C++");
            reply->set_message(prefix + request->name() + suffix);
            return grpc::Status::OK;
        }
    };

    void RunServer(uint16_t port) {
        std::ostringstream server_address_stream;
        server_address_stream << "0.0.0.0:" << port;
        std::string server_address = server_address_stream.str();
        GreeterServiceImpl service;

        grpc::EnableDefaultHealthCheckService(true);
        grpc::reflection::InitProtoReflectionServerBuilderPlugin();
        grpc::ServerBuilder builder;
        // Listen on the given address without any authentication mechanism.
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        // Register "service" as the instance through which we'll communicate with
        // clients. In this case it corresponds to an *synchronous* service.
        builder.RegisterService(&service);
        // Finally assemble the server.
        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
        std::cout << "Server listening on " << server_address << std::endl;

        // Wait for the server to shutdown. Note that some other thread must be
        // responsible for shutting down the server for this call to ever return.
        server->Wait();
    }
}

int main(int argc, char *argv[]) {
    mediocre::main::RunServer(50052);
    return 0;
}
