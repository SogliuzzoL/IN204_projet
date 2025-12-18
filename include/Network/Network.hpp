#pragma once

class Network {
 public:
  /**
   * Initialize the network subsystem.
   * @return true if initialization was successful, false otherwise.
   */
  bool initialize();

  /**
   * Pure virtual method to start the network (server or client).
   * @param host The host address to connect to or bind.
   * @param port The port number to connect to or bind.
   * @return true if the network started successfully, false otherwise.
   */
  virtual bool start(const char* host, int port) = 0;

  /**
   * Pure virtual method to stop the network (server or client).
   */
  virtual void stop() = 0;

  /**
   * Quit the network subsystem.
   */
  void quit();
};