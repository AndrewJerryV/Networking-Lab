#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int main() {
    int bucket_size, output_rate, n, incoming, storage = 0;
    int dropped, sent, i;

    printf("Enter bucket size: ");
    scanf("%d", &bucket_size);
    printf("Enter output rate (packets/sec): ");
    scanf("%d", &output_rate);
    printf("Enter number of time intervals (seconds): ");
    scanf("%d", &n);
    printf("\nTime\tIncoming\tSent\tDropped\tRemaining\n");
    printf("--------------------------------------------------------\n");

    for (i = 1; i <= n; i++) {
        printf("%d\t", i);
        printf("Enter incoming for t=%d: ", i); 
        scanf("%d", &incoming);
        storage += incoming;
        // Check for Overflow
        if (storage > bucket_size) {
            dropped = storage - bucket_size;
            storage = bucket_size;
        } else {
            dropped = 0;
        }
        // Send packets (Leak)
        sent = MIN(storage, output_rate);
        storage -= sent;
        printf("\r%d\t%d\t\t%d\t%d\t%d\n", i, incoming, sent, dropped, storage);
    }
    // Process remaining packets in the bucket after inputs stop
    while (storage > 0) {
        i++;
        sent = MIN(storage, output_rate);
        storage -= sent;
        printf("%d\t0\t\t%d\t0\t%d\n", i, sent, storage);
    }
    return 0;
}