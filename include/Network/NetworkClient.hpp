#pragma once

#include "Network/Network.hpp"

class NetworkClient : public Network {
 private:
  TCPsocket socket;

 public:
  bool start(const char* host, int port) override;
  void stop() override;
};