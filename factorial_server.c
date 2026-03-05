#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0), new_socket, num;
    struct sockaddr_in address = {AF_INET, htons(8080), INADDR_ANY};
    int addrlen = sizeof(address);
    long long fact;

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);
    printf("[Server] Listening on port 8080...\n");

    while(1) {
        new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        printf("[Server] Connection accepted\n");
        recv(new_socket, &num, sizeof(num), 0);
        printf("[Server] Received number: %d\n", num);

        fact = 1;
        for(int i = 1; i <= num; i++)
            fact *= i;
        printf("[Server] Calculated factorial: %lld\n", fact);

        send(new_socket, &fact, sizeof(fact), 0);
        printf("[Server] Sent factorial to client\n");
        close(new_socket);
        printf("[Server] Connection closed\n");
    }
    return 0;
}