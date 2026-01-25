#include <string.h>

#include <iostream>
#include <string>

#include "client.hpp"
#include "server.hpp"

int main(int argc, char **argv) {
  bool isServer = false;
  std::string targetIP = "127.0.0.1";

  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "--server") == 0) {
      isServer = true;
    } else if (strcmp(argv[i], "--ip") == 0) {
      if (i + 1 < argc) {
        targetIP = argv[i + 1];
        i++;
      } else {
        std::cerr
            << "Erreur : --ip nécessite une adresse (ex: --ip 192.168.1.50)"
            << std::endl;
        return 1;
      }
    }
  }

  if (isServer) {
    return run_server();
  } else {
    return run_client(targetIP);
  }
}