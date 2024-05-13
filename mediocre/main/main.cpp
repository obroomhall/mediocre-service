#include <csignal>
#include <cstdlib>
#include <mediocre/main/main.hpp>
#include <mediocre/server/server.hpp>
#include <thread>

static auto server = mediocre::server::Server(8081);

void shutdown() {
    server.shutdown_server();
}

void signal_handler(const int signal_num) {
    std::thread thread(shutdown);
    thread.join();
}

int main(int argc, char *argv[]) {
    signal(SIGTERM, signal_handler);
    server.run_server();
    return EXIT_SUCCESS;
}
