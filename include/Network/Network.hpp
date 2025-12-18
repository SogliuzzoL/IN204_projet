#pragma once

#include <string>

class Network {
 public:
  /**
   * Initialize the network subsystem.
   * @return true if initialization was successful, false otherwise.
   */
  bool initialize();

  /**
   * Pure virtual method to handle incoming data.
   * @return 1 if a new packet is available, or -1 on error. 0 means no packets
   * were currently available.
   */
  virtual int handleIncomingData() = 0;

  /**
   * Pure virtual method to send data.
   * @param message The message to send.
   * @return true if the message was sent successfully, false otherwise.
   */
  virtual bool sendData(const std::string& message) = 0;

  /**
   * Pure virtual method to stop the network (server or client).
   */
  virtual void stop() = 0;

  /**
   * Quit the network subsystem.
   */
  void quit();
};