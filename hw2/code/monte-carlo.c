# include <stdio.h>
# include <pthread.h>
# include <sys/shm.h>
# include <stdlib.h>
# include <omp.h>

int num_points = 15000;
int point_in_circle = 0;
int point_in_square = 0;

void monte_carlo() {
    // scale the random number to be between -1 and 1
    double x = (double)rand() / RAND_MAX * 2 - 1;
    double y = (double)rand() / RAND_MAX * 2 - 1;
    if (x * x + y * y <= 1) {
        point_in_circle++;
    }
    point_in_square++;
}

int main () {
    # pragma omp parallel num_threads(num_points)
    {
        monte_carlo();
    }

    # pragma omp master
    {
        double pi = 4 * (double)point_in_circle / point_in_square;
        printf("Pi: %f\n", pi);
    }
}