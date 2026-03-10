#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<stdint.h>

void mat_multiply(int r1, int c1, int mat1[r1][c1], int r2, int c2, int mat2[r2][c2], int n){
    int mat[r1][c2];
    for(int i=0;i<r1;i++){
        for(int j=0;j<c2;j++){
            mat[i][j]=0;
            for(int k=0; k<r1;k++)
                mat[i][j]+=mat1[i][k]*mat2[k][j];
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
    send(n, &r1, sizeof(int),0);
    send(n, &c2, sizeof(int),0);
    send(n, mat, sizeof(int)*r1*c2, 0);
    printf("Matrix AxB:\n");
    for(int i=0;i<r1;i++){
        for(int j=0;j<c1;j++)
            printf("%d ", mat[i][j]);
        printf("\n");
    }
    printf("Result sent to client\n");
}

void * matmulthread(void *arg){
    int n = *(int*)arg;
    printf("New client connected\n");
    int r1,c1,r2,c2;
    read(n, &r1, sizeof(int));
    read(n, &c1, sizeof(int));
    printf("Dimension of matrix A: %dx%d\n", r1,c1);
    int mat1[r1][c1];
    read(n, &mat1, sizeof(int)*r1*c1);
    printf("Enter Matrix A:\n");
    for(int i=0;i<r1;i++){
        for(int j=0;j<c1;j++)
            printf("%d ", mat1[i][j]);
        printf("\n");
    }

    read(n, &r2, sizeof(int));
    read(n, &c2, sizeof(int));
    printf("Dimension of matrix B: %dx%d\n", r2,c2);
    int mat2[r2][c2];
    read(n, &mat2, sizeof(int)*r2*c2);
    printf("Enter Matrix B:\n");
    for(int i=0;i<r2;i++){
        for(int j=0;j<c2;j++)
            printf("%d ", mat2[i][j]);
        printf("\n");
    }

    mat_multiply(r1,c1,mat1,r2,c2,mat2, n);
    close(n);
    pthread_exit(NULL);
}

int main(){
    int s = socket(AF_INET, SOCK_STREAM, 0), n;
    struct sockaddr_in a = {AF_INET, htons(8080), 0};
    pthread_t mul_thread;

    int opt=1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    bind(s,(struct sockaddr*)&a, sizeof(a));
    listen(s,5);
    printf("Server listening on port 8080...\n");

    socklen_t addrlen = sizeof(a);
    while(1){
        n=accept(s,0,0);
        pthread_create(&mul_thread,NULL,matmulthread,(void *)&n);
    }
    
    close(s);
    return 0;
}