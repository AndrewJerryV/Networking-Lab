#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int s = socket(AF_INET, SOCK_DGRAM, 0), n;
    struct sockaddr_in a = {AF_INET, htons(8080), 0};
    bind(s, (struct sockaddr*)&a, sizeof(a));
    printf("GBN Server ready...\n");
    socklen_t addrsize = sizeof(a);

    int expectedframe = 1;

    while (1) {
        int receivedframe;
        n = recvfrom(s, &receivedframe, sizeof(int), 0, (struct sockaddr*) &a, &addrsize);
        
        // Simulate dropping frames 2 and 5 exactly like the SR code
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

        if (receivedframe == expectedframe) {
            // In-order frame received perfectly
            printf("Frame %d received, sending ACK for %d\n", receivedframe, receivedframe);
            sendto(s, &receivedframe, sizeof(int), 0, (struct sockaddr*) &a, addrsize);
            expectedframe++;
        } 
        else if (receivedframe > expectedframe) {
            // Out-of-order frame! GBN has no buffer, so we discard it.
            int last_good_ack = expectedframe - 1;
            printf("Received out-of-order Frame %d. Expected %d. Discarding...\n", receivedframe, expectedframe);
            
            // Re-acknowledge the highest correctly received in-order frame (Cumulative ACK)
            if (last_good_ack > 0) {
                printf("  -> Re-sending cumulative ACK for %d\n", last_good_ack);
                sendto(s, &last_good_ack, sizeof(int), 0, (struct sockaddr*) &a, addrsize);
            }
        }
    }
    
    close(s);
    return 0;
}