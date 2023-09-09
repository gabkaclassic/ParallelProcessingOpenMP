#include <iostream>
#include <chrono>
#include <omp.h>
#include <functional>

const int N = 200000;

void initializeArrays(int* A, int* B)
{
    for (int i = 0; i < N; i++)
    {
        A[i] = i;
        B[i] = i;
    }
}

int calculateCParallel(int* A, int* B, int* C)
{
    int sum = 0;

#pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < N; i++)
    {
        C[i] = A[i] + B[i];
        sum += C[i];
    }

    return sum;
}

int calculateCSerial(int* A, int* B, int* C)
{
    int sum = 0;

    for (int i = 0; i < N; i++)
    {
        C[i] = A[i] + B[i];
        sum += C[i];
    }

    return sum;
}

int calculateCParallel_Critical(int* A, int* B, int* C)
{
    int sum = 0;

#pragma omp parallel
    {
        int local_sum = 0;

#pragma omp for
        for (int i = 0; i < N; i++)
        {
            C[i] = A[i] + B[i];
            local_sum += C[i];
        }

#pragma omp critical
        {
            sum += local_sum;
        }
    }

    return sum;
}

int calculateCParallel_Atomic(int* A, int* B, int* C)
{
    int sum = 0;

#pragma omp parallel for
    for (int i = 0; i < N; i++)
    {
        C[i] = A[i] + B[i];
#pragma omp atomic
        sum += C[i];
    }

    return sum;
}

int calculateCParallel_Sections_4(int* A, int* B, int* C)
{
    int sum = 0;

#pragma omp parallel sections reduction(+:sum)
    {
#pragma omp section
        {
            for (int i = 0; i < N / 4; i++)
            {
                C[i] = A[i] + B[i];
                sum += C[i];
            }
        }

#pragma omp section
        {
            for (int i = N / 4; i < N / 2; i++)
            {
                C[i] = A[i] + B[i];
                sum += C[i];
            }
        }

#pragma omp section
        {
            for (int i = N / 2; i < 3 * N / 4; i++)
            {
                C[i] = A[i] + B[i];
                sum += C[i];
            }
        }

#pragma omp section
        {
            for (int i = 3 * N / 4; i < N; i++)
            {
                C[i] = A[i] + B[i];
                sum += C[i];
            }
        }
    }

    return sum;
}

int calculateCParallel_Sections_2(int* A, int* B, int* C)
{
    int sum = 0;

#pragma omp parallel sections reduction(+:sum)
    {
#pragma omp section
        {
            for (int i = 0; i < N / 2; i++)
            {
                C[i] = A[i] + B[i];
                sum += C[i];
            }
        }

#pragma omp section
        {
            for (int i = N / 2; i < N; i++)
            {
                C[i] = A[i] + B[i];
                sum += C[i];
            }
        }
    }

    return sum;
}

void profileFunction(const std::string& name, std::function<int(int*, int*, int*)> func, int* A, int* B, int* C)
{
    auto start = std::chrono::steady_clock::now();
    int sum = func(A, B, C);
    auto end = std::chrono::steady_clock::now();

    std::cout << name << " : " << std::chrono::duration<double>(end - start).count() << std::endl;
    std::cout << "sum(C): " << sum << std::endl;
}

int main()
{
    int A[N];
    int B[N];
    int C[N];

    initializeArrays(A, B);

    std::cout << "With OpenMP:" << std::endl;
    profileFunction("calculateCParallel", calculateCParallel, A, B, C);

    std::cout << std::endl;

    std::cout << "Without OpenMP:" << std::endl;
    profileFunction("calculateCSerial", calculateCSerial, A, B, C);

    std::cout << "Critical: " << std::endl;
    profileFunction("calculateCParallel_Critical", calculateCParallel_Critical, A, B, C);

    std::cout << "Atomic:" << std::endl;
    profileFunction("calculateCParallel_Atomic", calculateCParallel_Atomic, A, B, C);

    std::cout << "2 sections:" << std::endl;
    profileFunction("calculateCParallel_Sections_2", calculateCParallel_Sections_2, A, B, C);

    std::cout << "4 sections:" << std::endl;
    profileFunction("calculateCParallel_Sections_4", calculateCParallel_Sections_4, A, B, C);

    return 0;
}