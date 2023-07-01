#include <cstdlib>
#include <libmediocre/main/main.hpp>
#include <libmediocre/server/server.hpp>

int main(int argc, char *argv[]) {
    auto server = mediocre::server::Server(50051);
    server.run_server();
    return EXIT_SUCCESS;
}
