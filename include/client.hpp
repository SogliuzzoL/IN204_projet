#pragma once
#include <string>

/**
 * Starts and runs the client.
 * @param serverIP The IP address of the server to connect to. Defaults to
 * localhost
 * @return 0 on success, negative value on failure.
 */
int run_client(const std::string& serverIP = "127.0.0.1");