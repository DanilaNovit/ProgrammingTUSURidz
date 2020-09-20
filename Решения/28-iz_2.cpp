// Дана целочисленная матрица размера M × N.
// Найти количество её строк, все элементы которых различны.

#include <stdio.h>
#include <stdlib.h>

int variousElements(int* line, int size)
{
    for (int i = 0; i < (size - 1); ++i)
    {
        for (int j = (i + 1); j < size; ++j)
        {
            if (line[i] == line[j])
            {
                return 0;
            }
        }
    }

    return 1;
}

int main()
{
    int M = 6;
    int N = 3;

    int** mat = (int**) malloc(sizeof(int*) * M);

    for (int i = 0; i < M; ++i)
    {
        mat[i] = (int*) malloc(sizeof(int) * N);
    }

    for (int i = 0; i < M; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            mat[i][j] = rand() % 5;
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    int count = 0;

    for (int i = 0; i < M; ++i)
    {
        if (variousElements(mat[i], N))
        {
            ++count;
        }
    }

    printf("%d\n", count);
}