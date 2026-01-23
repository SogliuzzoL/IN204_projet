#pragma once

#include <string>

class Network {
 public:
  /** Initializes the networking subsystem.
   * @return true if initialization was successful, false otherwise.
   */
  bool initialize();

  /** Cleans up the networking subsystem.
   * @return true if cleanup was successful, false otherwise.
   */
  virtual int handleIncomingData() = 0;

  /** Sends data over the network.
   * @param data Pointer to the data to send.
   * @param size Size of the data in bytes.
   * @return true if the data was sent successfully, false otherwise.
   */
  virtual bool sendData(void* data, int size) = 0;

  /** Stops the network and cleans up resources. */
  virtual void stop() = 0;

  /** Quits the networking subsystem. */
  void quit();
};