#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <chrono>

int countOnes(int number)
{
    int count = 0;
    while (number != 0)
    {
        count += number & 1;
        number >>= 1;
    }
    return count;
}

void initMatrix(int**& matrix, int M, int N)
{
    matrix = new int*[M];
    srand(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < M; ++i)
    {
        matrix[i] = new int[N];
        for (int j = 0; j < N; ++j)
        {
            matrix[i][j] = rand() % 10 + 1;
        }
    }
}

void calculateSumWithOpenMP(int** matrix, int M, int N)
{
    int sum;
    int i, j1, j2;
#pragma omp parallel shared(sum, matrix) private(i)
    {
#pragma omp for private(j1, j2)
        for (i = 0; i < N; i++)
        {
            sum = 0;
            for (j1 = 0; j1 < M; j1++)
            {
                for (j2 = 0; j2 < M; j2++)
                {
                    sum += countOnes(matrix[i][j1] * matrix[i][j2]);
                }
            }
//            printf("Сумма для строки %d равна %d\n", i, sum);
        }
    }
}

void calculateSumWithoutOpenMP(int** matrix, int M, int N)
{
    int sum;
    for (int i = 0; i < N; i++)
    {
        sum = 0;
        for (int j1 = 0; j1 < M; j1++)
        {
            for (int j2 = 0; j2 < M; j2++)
            {
                sum += countOnes(matrix[i][j1] * matrix[i][j2]);
            }
        }
//        printf("Сумма для строки %d равна %d\n", i, sum);
    }
}

int main()
{
    int M = 500; // Rows
    int N = 400; // ColumnsXX

    int** matrix;

    initMatrix(matrix, M, N);

    auto start = std::chrono::steady_clock::now();

    calculateSumWithOpenMP(matrix, M, N);
    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> openMPTime = end - start;
    std::cout << "With OpenMP: " << openMPTime.count() << std::endl;

    start = std::chrono::steady_clock::now();
    calculateSumWithoutOpenMP(matrix, M, N);
    end = std::chrono::steady_clock::now();
    std::chrono::duration<double> withoutOpenMPTime = end - start;
    std::cout << "Without OpenMP: " << withoutOpenMPTime.count() << std::endl;

    for (int i = 0; i < M; ++i)
    {
        delete[] matrix[i];
    }
    delete[] matrix;

    return 0;
}