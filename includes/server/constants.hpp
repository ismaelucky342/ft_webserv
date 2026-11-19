#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

const int CLIENT_TIMEOUT = 30;
const int POLL_TIMEOUT = 1000; // 1 segundo para revisar timeouts de clientes y poder salir del bucle si g_running se pone a 0 (SIGINT).

#define FILE_BUFFER_SIZE 8192

#endif