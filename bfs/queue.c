#include <stdlib.h>
#include <stdio.h>


struct queue {
    int items[160000];
    int head;
    int tail;
};

struct queue* createQueue() {
    struct queue* q = malloc(sizeof(struct queue));
    q->head = -1;
    q->tail = -1;
    return q;
}

int isEmpty(struct queue* q) {
    return q->tail == -1;
}

void enqueue(struct queue* q, int value) {
    if (q->tail == 80000-1) {
        printf("Queue is FULL\n");
    } else {
        if (q->head == -1) {
            q->head = 0;
        }
        q->tail++;
        q->items[q->tail] = value;
    }
}

int sizeOfQ(struct queue* q) {
    return q->tail+1;
}

int contains(struct queue* q, int value) {
    int i;
    for (i = 0; i < 80000; i++) {
        if (q->items[i] == value) {
            return 1;
        }
    }
    return 0;
}

int dequeue(struct queue* q) {
    int item;
    if (isEmpty(q)) {
        printf("Queue is empty!\n");
        item = -1;
    } else {
        item = q->items[q->head];
        q->head++;
        if (q->head > q->tail) {
            //reset
            q->head = q->tail = -1;
        }
    }
    return item;
}
