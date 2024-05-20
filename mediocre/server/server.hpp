#ifndef mediocre_server_h
#define mediocre_server_h

#include <cstdint>
#include <grpcpp/server_builder.h>
#include <iomanip>

namespace mediocre::server {

    class Server {
    private:
        static std::string get_server_address(uint16_t port);
        static void register_listener(grpc::ServerBuilder &builder, const std::string &server_address);
        static void register_services(grpc::ServerBuilder &builder);
        static void set_options(grpc::ServerBuilder &builder);
        std::string server_address;
        std::unique_ptr<grpc::Server> server;

    public:
        explicit Server(uint16_t port);
        void run_server();
        void shutdown_server();
    };


}// namespace mediocre::server

#endif// mediocre_server_h