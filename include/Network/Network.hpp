#pragma once

class Network {
 public:
  bool initialize();
  virtual bool start(const char* host, int port) = 0;
  virtual void stop() = 0;
  void quit();
};