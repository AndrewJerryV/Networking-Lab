#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0), n;
    char buffer[1024];
    struct sockaddr_in servaddr = {AF_INET, htons(8080), INADDR_ANY};
    socklen_t len = sizeof(servaddr);

    printf("Enter number: ");
    scanf("%d", &n);    
    sendto(sockfd, &n, sizeof(n), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    
    int m = recvfrom(sockfd, (char *)buffer, 1024, 0, (struct sockaddr *)&servaddr, &len);
    buffer[m] = '\0';
    printf("Server response: %s\n", buffer);
    
    close(sockfd);
    return 0;
}