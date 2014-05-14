#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex;
pthread_cond_t cond;
pthread_t threads[10];
long int p, q, A, B, key;
long int count_th = 10;  //число потоков, при помощи которых будет вычисляться ключ


long int count_deg(long int num, long int deg, long int mod) { //num в степень deg по модулю mod
    long int result = 1;
    long int current_deg = 0;
    while ( current_deg < deg ) {
        result *= num;
        result %= mod;
        //if ( num == 2 && deg == 3 && mod == 1)
        //printf("current_deg = %ld, deg = %ld, num = %ld, mod = %ld, result = %ld", current_deg, deg, num, mod, result);
        ++ current_deg;
    }
    return result;
}

void key_found(long int a) { //в этой функции сообщется о том, что значение найдено и убиваются все потоки

    pthread_mutex_lock(&mutex);

    ::key = count_deg(B, a, q);
    printf("Key  = %ld\n", key);
    pthread_t self_id = pthread_self();
    //pthread_mutex_signal(&cond);

    for ( long int i = 0; i < count_th; ++i ) {
        if ( !pthread_equal(threads[i], self_id) )
            pthread_cancel(threads[i]);
    }
    pthread_mutex_unlock(&mutex);
}


void * calc(void *c) { // каждый поток вычисляем на отрезке [l,r]
    pthread_mutex_lock(&mutex);
    pthread_mutex_unlock(&mutex);
    long int id = *( long int * ) c;
    long int l = id * 1000;
    long int r = l + 999;
    //printf(" l = %ld, r = %ld, p = %ld, q = %ld, A = %ld\n", l, r, p , q, A);
    for (long int i = l; i <= r; ++i) {
        long int tmp = count_deg(p, i, q);
        if ( tmp == A ) {
            //printf("i = %ld", i);
            key_found(i);
            break;
        }
    }
    pthread_exit(NULL);
}



int main(/*int arvc, char *argv[]*/) {
        /*if ( arvc < 5 ) {
            printf("Not enough arguments\n");
            return 0;
        }
        ::p = atoi(argv[1]);
        ::q = atoi(argv[2]);
        ::A = atoi(argv[3]); // A = p^a mod q
        ::B = atoi(argv[4]); // B = p^b mod q
        */
        printf("The program is currently using %ld threads\n", count_th);
        printf("Please enter p, q, A, B\n");
        scanf("%ld%ld%ld%ld", &p, &q, &A, &B);
        long int id[count_th];

        long int i = 0;

        for (i = 0; i < count_th; ++i)
            id[i] = i;

        pthread_mutex_init (&mutex, NULL);
        pthread_cond_init (&cond, NULL);
        pthread_mutex_lock(&mutex);

        for (long int i = 0; i < count_th; ++i) {
            pthread_create(&threads[i], NULL, calc, &id[i]);
        }

        pthread_cond_wait(&cond, &mutex);
        pthread_mutex_unlock(&mutex);
        pthread_cond_destroy(&cond);
        return 0;

}
