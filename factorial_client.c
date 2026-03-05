#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0), num;
    struct sockaddr_in servaddr = {AF_INET, htons(8080), inet_addr("127.0.0.1")};
    long long fact;

    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    printf("Enter number: ");
    scanf("%d", &num);

    send(sockfd, &num, sizeof(num), 0);
    recv(sockfd, &fact, sizeof(fact), 0);

    printf("Factorial: %lld\n", fact);
    close(sockfd);
    return 0;
}