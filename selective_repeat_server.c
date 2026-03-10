#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>

int main(){
    int s = socket(AF_INET, SOCK_DGRAM, 0), n;
    struct sockaddr_in a = {AF_INET, htons(8080), 0};
    bind(s, (struct sockaddr*)&a, sizeof(a));
    printf("Server ready...\n");
    socklen_t addrsize = sizeof(a);

    int expectedframe=1, buffer[100]={0};
    while(1){
        int receivedframe;
        n=recvfrom(s, &receivedframe, sizeof(int), 0, (struct sockaddr*) &a, &addrsize);
        static int dropped_2 = 0;
        static int dropped_5 = 0;
        if (receivedframe == 2 && !dropped_2) {
            dropped_2 = 1; 
            continue; 
        }
        if (receivedframe == 5 && !dropped_5) {
            dropped_5 = 1;
            continue;
        }

        if(receivedframe==expectedframe){
            printf("Frame %d received, sending ACK for %d\n", receivedframe, receivedframe);
            sendto(s, &receivedframe, sizeof(int), 0, (struct sockaddr*) &a, addrsize);
            expectedframe++;
        
            while(buffer[expectedframe]==1){
                printf("Frame %d received, sending ACK for %d\n", expectedframe, expectedframe);
                sendto(s, &expectedframe, sizeof(int), 0, (struct sockaddr*) &a, addrsize);
                buffer[expectedframe]=0;
                expectedframe++;
            }
        }
        else if(receivedframe>expectedframe){
            printf("Received frame %d, expected Frame %d\n",receivedframe, expectedframe);
            static int last_nak_sent = 0;
            if (last_nak_sent != expectedframe) {
                int nak = -expectedframe;
                printf("Sending NAK for Frame %d\n", expectedframe);
                sendto(s, &nak, sizeof(int), 0, (struct sockaddr*) &a, addrsize);
                last_nak_sent = expectedframe;
            }
            if(receivedframe<100)
                buffer[receivedframe]=1;
        }   

    }
    
    close(s);
    return 0;
}