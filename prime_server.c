#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in servaddr = {AF_INET, htons(8080), INADDR_ANY}, cliaddr;

    bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    printf("[Server] Listening on port 8080...\n");

    socklen_t len = sizeof(cliaddr);
    int num;
    char response[1024];

    while(1) {
        recvfrom(sockfd, &num, sizeof(num), 0, (struct sockaddr *)&cliaddr, &len);
        printf("[Server] Received number: %d\n", num);

        int is_prime = 1;
        if (num <= 1) 
            is_prime = 0;
        for (int i = 2; i <= num / 2; i++) {
            if (num % i == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            sprintf(response, "Prime. Factors: 1, %d", num);
            printf("[Server] %d is prime\n", num);
        } 
        else {
            sprintf(response, "Composite");
            printf("[Server] %d is composite\n", num);
        }

        sendto(sockfd, response, strlen(response), 0, (const struct sockaddr *)&cliaddr, len);
        printf("[Server] Sent response to client: %s\n", response);
    }
    return 0;
}