#include <cstdlib>
#include <libmediocre/main/main.hpp>
#include <libmediocre/server/server.hpp>

int main(int argc, char *argv[]) {
    mediocre::server::run(50051);
    return EXIT_SUCCESS;
}
