#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>

int main(){
    int s = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in a = {AF_INET, htons(8080), 0};
    inet_pton(AF_INET, "127.0.0.1", &a.sin_addr);
    connect(s, (struct sockaddr *)&a, sizeof(a));

    char buf[1024] = {0};

    int r1,c1,r2,c2;
    printf("Enter dimension of Matrix A (n m): ");
    scanf("%d %d", &r1, &c1);
    printf("Enter dimension of Matrix B (n m): ");
    scanf("%d %d", &r2, &c2);
    int mat1[r1][c1];
    int mat2[r2][c2];
    printf("Enter Matrix A:\n");
    for(int i=0;i<r1;i++){
        for(int j=0;j<c1;j++)
            scanf("%d", &mat1[i][j]);
    }
    printf("Enter Matrix B:\n");
    for(int i=0;i<r2;i++){
        for(int j=0;j<c2;j++)
            scanf("%d", &mat2[i][j]);
    }
    
    send(s, &r1, sizeof(int),0);
    send(s, &c1, sizeof(int),0);
    send(s, mat1, sizeof(int)*r1*c1, 0);

    send(s, &r2, sizeof(int),0);
    send(s, &c2, sizeof(int),0);
    send(s, mat2, sizeof(int)*r2*c2, 0);

    read(s, &r1, sizeof(int));
    read(s, &c1, sizeof(int));
    printf("Resultant matrix AxB: %dx%d\n", r1,c1);
    read(s, &mat1, sizeof(int)*r1*c1);
    printf("Matrix AxB:\n");
    for(int i=0;i<r1;i++){
        for(int j=0;j<c1;j++)
            printf("%d ", mat1[i][j]);
        printf("\n");
    }

    close(s);
    return 0;
}