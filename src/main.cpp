#include <string>

#include "client.hpp"
#include "server.hpp"

int main(int argc, char** argv) {
  // Server Mode
  if (argc > 1)
    if (std::string(argv[1]) == "--server") return run_server();

  // Client Mode
  return run_client();
}