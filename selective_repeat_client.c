#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/time.h>
#include<stdlib.h>

int main() {
    int s = socket(AF_INET, SOCK_DGRAM, 0), n;
    struct sockaddr_in a = {AF_INET, htons(8080), 0};
    socklen_t addrsize = sizeof(a);
    inet_pton(AF_INET,"127.0.0.1", &a.sin_addr);
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));

    int totalframes, windowsize;
    printf("Enter no. of frames: ");
    scanf("%d", &totalframes);
    printf("Enter windows size: ");
    scanf("%d", &windowsize);
    int base=1, acked[100]={0},next_to_send = 1;

    while(base <= totalframes){
        while (next_to_send < base + windowsize && next_to_send <= totalframes) {
            printf("Frame %d sent\n", next_to_send);
            sendto(s, &next_to_send, sizeof(int),0, (struct sockaddr*)&a, addrsize);
            next_to_send++;
            usleep(100000);
        }

        int ack;
        n=recvfrom(s, &ack, sizeof(int), 0, (struct sockaddr*)&a, &addrsize);
        if(n<0){
            printf("Timeout for Frame %d\nRetransmitting...\n", base);
            sendto(s, &base, sizeof(int),0, (struct sockaddr*)&a, addrsize);
        }
        else{
            if(ack>0){
                if (!acked[ack]) {
                    printf("ACK received for frame %d\n", ack);
                    acked[ack] = 1;
                }
                while(base<=totalframes && acked[base])
                    base++;
            }
            else{
                printf("ACK not received for %d\n", -ack);
                printf("Retransmitting...\n");
                int negack = abs(ack);
                sendto(s, &negack, sizeof(int),0, (struct sockaddr*)&a, addrsize);
            }
        }
    }
    close(s);
    printf("\nAll frames sent successfully. Client exiting.\n");
    return 0;
}