#include <iostream>
#include <chrono>
#include <omp.h>

int NMAX = 100;
int LIMIT = 100;

void performCalculation()
{
    int i, j;
    float sum;
    float a[NMAX][NMAX];

    for (i = 0; i < NMAX; i++)
        for (j = 0; j < NMAX; j++)
            a[i][j] = i + j;

#pragma omp parallel shared(a) if (NMAX > LIMIT)
    {
#pragma omp for private(i, j, sum)
        for (i = 0; i < NMAX; i++)
        {
            sum = 0;
            for (j = 0; j < NMAX; j++)
                sum += a[i][j];
//            printf("Сумма элементов строки %d равна %f\n", i, sum);
        }
    }
}

int main()
{
    const int numTests = 10; // Количество испытаний для каждой комбинации параметров NMAX и LIMIT

    int matchedNMAX = -1;
    int matchedLIMIT = -1;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < numTests; i++)
    {
        performCalculation();
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto durationSingle = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    for (int N = 1; N <= NMAX; N++)
    {
        for (int L = 1; L <= LIMIT; L++)
        {

            start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < numTests; i++)
            {
                performCalculation();
            }
            end = std::chrono::high_resolution_clock::now();
            auto durationMulti = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

//            printf("Single: %d, multi: %d\n", durationSingle, durationMulti);
            if (durationMulti == durationSingle)
            {
                matchedNMAX = N;
                matchedLIMIT = L;
                break;
            }
        }
        if (matchedNMAX != -1)
            break;
    }

    // Вывод результатов
    std::cout << "Values:  " << std::endl;
    std::cout << "NMAX: " << matchedNMAX << std::endl;
    std::cout << "LIMIT: " << matchedLIMIT << std::endl;

    return 0;
}
