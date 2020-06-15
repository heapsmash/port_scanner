#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void my_turn(void)
{
        for (int i = 0; i < 8; i++)
        {
                sleep(1);
                puts("my_turn");
        }
}

void *your_turn(void *arg)
{
        int *iptr = (int *)arg;
        for (int i = 0; i < 8; i++)
        {
                sleep(1);
                printf("your_turn %d %d\n", i, *iptr);
                (*iptr)++;
        }

        return NULL;
}

int main(int argc, char *argv[])
{
        pthread_t newthread;
        int v = 5;

        pthread_create(&newthread, NULL, your_turn, &v);
        my_turn();

        pthread_join(newthread, NULL);
        printf("thread's done: v=%d\n", v);

        return 0;
}
