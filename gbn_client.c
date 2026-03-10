#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <stdlib.h>

int main() {
    int s = socket(AF_INET, SOCK_DGRAM, 0), n;
    struct sockaddr_in a = {AF_INET, htons(8080), 0};
    socklen_t addrsize = sizeof(a);
    inet_pton(AF_INET, "127.0.0.1", &a.sin_addr);
    
    // Set 2-second timeout for recvfrom
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));

    int totalframes, windowsize;
    printf("Enter no. of frames: ");
    scanf("%d", &totalframes);
    printf("Enter window size: ");
    scanf("%d", &windowsize);
    
    int base = 1, next_to_send = 1;

    while (base <= totalframes) {
        
        // 1. Send frames up to the window limit
        while (next_to_send < base + windowsize && next_to_send <= totalframes) {
            printf("Frame %d sent\n", next_to_send);
            sendto(s, &next_to_send, sizeof(int), 0, (struct sockaddr*)&a, addrsize);
            next_to_send++;
            usleep(100000); // Short delay
        }

        // 2. Wait for ACK
        int ack;
        n = recvfrom(s, &ack, sizeof(int), 0, (struct sockaddr*)&a, &addrsize);
        
        if (n < 0) {
            // TIMEOUT: Go-Back-N triggers here!
            printf("--> Timeout for Frame %d! Going back to frame %d...\n", base, base);
            // Reset next_to_send back to base so the while loop re-sends the whole window
            next_to_send = base; 
        } else {
            // GBN uses Cumulative ACKs
            if (ack >= base) {
                printf("ACK received for frame %d\n", ack);
                // Slide the window forward to the next unacknowledged frame
                base = ack + 1; 
            }
        }
    }

    close(s);
    return 0;
}