#include <stdio.h>
#include "server.h"

int main() {
    printf("Starting HTTP server...\n");
    start_server(8080);  // Start server on port 8080
    return 0;
}