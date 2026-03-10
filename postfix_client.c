#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {AF_INET, htons(8080), INADDR_ANY};
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    connect(s, (struct sockaddr*)&addr, sizeof(addr));

    char infix[100], postfix[100]; int result;
    printf("Enter Infix: ");
    scanf("%[^\n]", infix);

    send(s, infix, sizeof(infix), 0);
    recv(s, postfix, sizeof(postfix), 0);
    recv(s, &result, sizeof(int), 0);

    printf("Postfix: %s\nResult: %d\n", postfix, result);
    close(s);
    return 0;
}