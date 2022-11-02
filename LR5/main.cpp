#include <windows.h>
#include <vector>
#include <ctime>
#include <iostream>


const std::size_t kArrayLength{10};
const std::size_t kMaxArrayItem{100};

std::vector<std::size_t> data(kArrayLength);


HANDLE created_array_event{};
HANDLE sorted_array_event{};


DWORD WINAPI create_array(LPVOID);

DWORD WINAPI sort_array(LPVOID);

DWORD WINAPI print_sorted_array(LPVOID);


int main()
{
    const constexpr std::size_t kThreadsCount{3};
    HANDLE threads[kThreadsCount]{};
    DWORD ids[kThreadsCount]{};

    created_array_event = CreateEvent(NULL, FALSE, FALSE, NULL);
    sorted_array_event = CreateEvent(NULL, FALSE, FALSE, NULL);


    threads[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) create_array, NULL, 0, &ids[0]);
    threads[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) sort_array, NULL, 0, &ids[1]);
    threads[2] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) print_sorted_array, NULL, 0, &ids[1]);


    const auto threads_result{WaitForMultipleObjects(kThreadsCount, threads, TRUE, INFINITE)};

    CloseHandle(threads[0]);
    CloseHandle(threads[1]);
    CloseHandle(threads[2]);
    CloseHandle(sorted_array_event);
    CloseHandle(created_array_event);

    return 0;
}

DWORD WINAPI create_array(LPVOID parameters)
{
    std::srand(std::time(NULL));

    for (std::size_t counter{0}; counter < data.size(); ++counter)
    {
        data.at(counter) = std::rand() % kMaxArrayItem;
        Sleep(500);
    }

    SetEvent(created_array_event);

    return 0;
}

DWORD WINAPI sort_array(LPVOID parameters)
{
    WaitForSingleObject(created_array_event, INFINITE);

    for (const auto &item : data)
        std::cout << item << ' ';
    std::cout << '\n';

    std::sort(std::begin(data), std::end(data));

    SetEvent(sorted_array_event);

    return 0;
}

DWORD WINAPI print_sorted_array(LPVOID parameters)
{
    WaitForSingleObject(sorted_array_event, INFINITE);

    for (const auto &item : data)
        std::cout << item << ' ';

    std::cout << '\n';

    return 0;
}