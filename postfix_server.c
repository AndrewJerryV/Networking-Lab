#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <unistd.h>

int prec(char c) {
    if (c == '+' || c == '-') return 1;
    if (c == '*' || c == '/') return 2;
    return 0;
}

void solve(char* infix, char* postfix, int* result) {
    char stack[100]; int top = -1, j = 0;
    for (int i = 0; infix[i]; i++) {
        if (isdigit(infix[i])) {
            postfix[j++] = infix[i];
            postfix[j++] = ' '; 
        }
        else if (infix[i] == '(') 
            stack[++top] = '(';
        else if (infix[i] == ')') {
            while (top > -1 && stack[top] != '(') {
                postfix[j++] = stack[top--];
                postfix[j++] = ' '; 
            }
            top--;
        } 
        else {
            while (top > -1 && prec(stack[top]) >= prec(infix[i])) {
                postfix[j++] = stack[top--];
                postfix[j++] = ' '; 
            }
            stack[++top] = infix[i];
        }
    }
    while (top > -1) {
        postfix[j++] = stack[top--];
        postfix[j++] = ' '; 
    }
    postfix[j] = '\0';

    int eval[100]; top = -1;
    for (int i = 0; postfix[i]; i++) {
        if (isdigit(postfix[i])) 
            eval[++top] = postfix[i] - '0';
        else {
            int b = eval[top--], a = eval[top--];
            if (postfix[i] == '+') eval[++top] = a + b;
            else if (postfix[i] == '-') eval[++top] = a - b;
            else if (postfix[i] == '*') eval[++top] = a * b;
            else if (postfix[i] == '/') eval[++top] = a / b;
        }
    }
    *result = eval[top];
}

int main() {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {AF_INET, htons(8080), INADDR_ANY};
    bind(s, (struct sockaddr*)&addr, sizeof(addr));
    listen(s, 5);
    while (1) {
        int n = accept(s, NULL, NULL);
        char infix[100], postfix[100]; int result;
        recv(n, infix, sizeof(infix), 0);
        solve(infix, postfix, &result);
        send(n, postfix, sizeof(postfix), 0);
        send(n, &result, sizeof(int), 0);
        close(n);
    }
}