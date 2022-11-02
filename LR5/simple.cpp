#include <iostream>
#include <windows.h>
#include <vector>
#include <ctime>


const std::size_t kMaxNumber{999999999};
const std::size_t data_size{1000000};

std::vector<std::size_t> data{};

bool array_generated{false};

void create_array()
{
    std::cout << "create_array: started creating random array\n";
    std::srand(time(nullptr));

    Sleep(5000);

    for (std::size_t counter{0}; counter < data.size(); ++counter)
        data.at(counter) = std::rand() % kMaxNumber;

    Sleep(5000);

    std::cout << "create_array: finished creating random array\n";
    array_generated = true;
}

void sort_array()
{
    std::cout << "sort_array: Waiting array to be generated\n";

    while (!array_generated);

    std::cout << "sort_array: Finished waiting array to be generated\n";
    std::cout << "sort_array: Started sorting array generated\n";

    std::sort(std::begin(data), std::end(data));

    std::cout << "sort_array: Finished sorting array\n";
}

int main()
{
    std::cout.setf(std::ios::boolalpha);

    HANDLE threads[2]{};

    DWORD id_create_array_thread{};
    DWORD id_sort_array_thread{};

    threads[0] = CreateThread(
            NULL,
            0,
            (LPTHREAD_START_ROUTINE) create_array,
            NULL,
            0,
            &id_create_array_thread
    );

    if (threads[0] == NULL) return GetLastError();

    threads[1] = CreateThread(
            NULL,
            0,
            (LPTHREAD_START_ROUTINE) sort_array,
            NULL,
            0,
            &id_sort_array_thread
    );

    if (threads[1] == NULL) return GetLastError();

    const auto response {WaitForMultipleObjects(2, threads, TRUE, INFINITE)};

    std::cout << std::boolalpha << std::is_sorted(std::cbegin(data), std::cend(data)) << '\n';


    return 0;
}