# include <stdio.h>
# include <pthread.h>
# include <sys/shm.h>
# include <stdlib.h>
# include <omp.h>

int point_in_circle = 0;
int point_in_square = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void monte_carlo() {
    // scale the random number to be between -1 and 1
    double x = (double)rand() / RAND_MAX * 2 - 1;
    double y = (double)rand() / RAND_MAX * 2 - 1;
    pthread_mutex_lock(&mutex);
    if (x * x + y * y <= 1) {
        point_in_circle++;
    }
    point_in_square++;
    pthread_mutex_unlock(&mutex);
}

int main (int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <number of points>\n", argv[0]);
        return 1;
    }

    int num_points = atoi(argv[1]);
    
    # pragma omp parallel num_threads(num_points)
    {
        monte_carlo();
    }

    # pragma omp barrier

    # pragma omp master
    {
        double pi = 4 * (double)point_in_circle / point_in_square;
        printf("Points in circle: %d\n", point_in_circle);
        printf("Points in square: %d\n", point_in_square);
        printf("Pi: %f\n", pi);
    }
}