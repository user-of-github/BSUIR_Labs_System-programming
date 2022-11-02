#include <iostream>
#include <vector>
#include <windows.h>
#include <chrono>
#include <algorithm>


std::vector<int> create_array_with_random_values(const std::size_t);

std::vector<int> get_subarray(const std::vector<int> &, const std::size_t, const std::size_t);

void print_vector(const std::vector<int> &);

std::vector<int> sorted(const std::vector<int> &);

std::vector<int> merge_two_arrays(const std::vector<int> &, const std::vector<int> &);


// data for threads
std::vector<int> first_part{};
std::vector<int> second_part{};


// function prototypes for CreateThread()
DWORD WINAPI SortFirstPart(LPVOID);

DWORD WINAPI SortSecondPart(LPVOID);


int main()
{
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    const std::size_t array_size{10000000};
    std::cout << "Test randomized array size: " << array_size << '\n';

    const auto values{create_array_with_random_values(array_size)};
    std::cout << "SOURCE: " << '\n';
    print_vector(values);

    {
        std::cout << "SINGLE THREAD MODE: \n";
        const auto first_partt{get_subarray(values, 0, values.size() / 2 - 1)};
        const auto second_partt{get_subarray(values, values.size() / 2, values.size() - 1)};



        const auto start_single_thread{std::chrono::high_resolution_clock::now()};
        const auto first_sorted{sorted(first_partt)};
        const auto second_sorted{sorted(second_partt)};
        const auto response{merge_two_arrays(first_sorted, second_sorted)};
        const auto stop_single_thread{std::chrono::high_resolution_clock::now()};

        std::cout << "Time: " << duration_cast<std::chrono::microseconds>(stop_single_thread - start_single_thread).count() << " microseconds \n";
        print_vector(response);
    }

    std::cout << "----------------\n";

    {
        std::cout << "DOUBLE THREAD MODE: \n";
        std::cout << "SOURCE: " << '\n';
        print_vector(values);
        const auto start_double_thread{std::chrono::high_resolution_clock::now()};

        first_part = get_subarray(values, 0, values.size() / 2 - 1);
        second_part = get_subarray(values, values.size() / 2, values.size() - 1);

        //print_vector(first_part);
        //print_vector(second_part);

        DWORD first_thread_id{};
        DWORD second_thread_id{};
        HANDLE threads[2]{};

        threads[0] = CreateThread(
                NULL,
                0,
                SortFirstPart,
                NULL,
                0,
                &first_thread_id
                );

        threads[1] = CreateThread(
                NULL,
                0,
                SortSecondPart,
                NULL,
                0,
                &second_thread_id
                );

        Sleep(100);
        TerminateThread(threads[0], 1);

        const DWORD threads_result {WaitForMultipleObjects(2, threads, TRUE, INFINITE)};

        if (threads_result == WAIT_OBJECT_0)
        {
            DWORD exit_code1{};
            DWORD exit_code2 {};
            const BOOL first {GetExitCodeThread(threads[0], &exit_code1)};

            const BOOL second {GetExitCodeThread(threads[1], &exit_code2)};
            std::cout << "ERROR CODES OF THREADS: " << exit_code1 << ' ' << exit_code2 << '\n';

            try
            {
                if (exit_code1 == 1) throw std::runtime_error("First thread was terminated !!");
                if (exit_code2 == 1) throw std::runtime_error("Second thread was terminated !!");
            }
            catch(const std::exception &exception)
            {
                std::cout << exception.what() << " => EXITING PROGRAM";
                CloseHandle(threads[0]);
                CloseHandle(threads[1]);
                exit(0);
            }
        }
        else
        {
            std::cout << "WaitFoprMultipleObjects() FAILED\n";
            CloseHandle(threads[0]);
            CloseHandle(threads[1]);
            exit(0);
        }

        const auto response{merge_two_arrays(first_part, second_part)};
        const auto stop_double_thread{std::chrono::high_resolution_clock::now()};

        std::cout << "Time: " << duration_cast<std::chrono::microseconds>(stop_double_thread - start_double_thread).count() << " microseconds\n";

        CloseHandle(threads[0]);
        CloseHandle(threads[1]);

        //print_vector(first_part);
        //print_vector(second_part);

        print_vector(response);
    }

    return 0;
}


std::vector<int> create_array_with_random_values(const std::size_t size)
{
    const auto kMaxNumberForArray{1000000000};

    std::vector<int> response{};
    response.reserve(size);

    std::srand(std::time(NULL));

    for (std::size_t counter{0}; counter < size; ++counter)
        response.push_back(std::rand() % kMaxNumberForArray);

    return response;
}

std::vector<int> get_subarray(const std::vector<int> &source, const std::size_t from, const std::size_t to)
{
    std::vector<int> response{};
    response.reserve(to - from + 1);
    for (std::size_t index{from}; index <= to; ++index)
        response.push_back(source.at(index));

    return response;
}

void print_vector(const std::vector<int> &array)
{
    for (std::size_t counter {0}; counter < std::min(std::size_t{100}, array.size()); ++counter)
        std::cout << array.at(counter) << ' ';

    std::cout << '\n';
}

std::vector<int> sorted(const std::vector<int> &source)
{
    std::vector<int> response {};
    for (const auto item : source)
    {
        response.push_back(item);
    }

    std::sort(response.begin(), response.end());

    return response;
}

std::vector<int> merge_two_arrays(const std::vector<int> &first, const std::vector<int> &second)
{
    std::size_t i{0};
    std::size_t j{0};
    std::size_t k{0};

    std::vector<int> response(first.size() + second.size(), 0);

    // Traverse both array
    while (i < first.size() && j < second.size())
    {
        if (first.at(i) < second.at(j))
            response.at(k++) = first.at(i++);
        else
            response.at(k++) = second.at(j++);
    }

    while (i < first.size()) response.at(k++) = first.at(i++);

    while (j < second.size()) response.at(k++) = second.at(j++);

    return response;
}

DWORD WINAPI SortFirstPart(LPVOID t)
{
    std::cout << "1 thread is started\n";
    std::sort(std::begin(first_part), std::end(first_part));
    std::cout << "1 thread is ended\n";

    return 0;
}

DWORD WINAPI SortSecondPart(LPVOID t)
{
    std::cout << "2 thread is started\n";
    std::sort(std::begin(second_part), std::end(second_part));
    std::cout << "2 thread is ended\n";

    return 0;
}


/*  INPUT
std::cin >> array_size;
try
{
    if (std::cin.fail())
        throw std::invalid_argument("Invalid array size provided. Exiting...");
}
catch (const std::exception &exception)
{
    std::cout << exception.what() << '\n';
    return 0;
}*/