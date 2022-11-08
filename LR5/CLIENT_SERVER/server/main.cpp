#include <windows.h>
#include <conio.h>
#include <iostream>


int main()
{
    char lpszComLine[100]{}; // for lauching client process with command line args
    STARTUPINFO startup_info{};
    PROCESS_INFORMATION process_information{};
    HANDLE hWritePipe{};
    HANDLE hReadPipe{};
    HANDLE hInheritWritePipe{}; // for duplicate


    // anonymous channel
    if (!CreatePipe(&hReadPipe, &hWritePipe, NULL, 0))
    {
        std::cout << "Create pipe failed.\n";
        std::cout << "Press any key to finish.\n";
        _getch();
        return GetLastError();
    }

    // duplicate for write pipe
    if (!DuplicateHandle(GetCurrentProcess(), hWritePipe, GetCurrentProcess(), &hInheritWritePipe, 0, TRUE,
                         DUPLICATE_SAME_ACCESS))
    {
        std::cout << "Duplicate handle failed.\n";
        std::cout << "Press any key to finish.\n";
        _getch();
        return GetLastError();
    }
    // no need anymore
    CloseHandle(hWritePipe);

    // attributes for launched process
    ZeroMemory(&startup_info, sizeof(STARTUPINFO));
    startup_info.cb = sizeof(STARTUPINFO);

    int number_of_messages{};
    do
    {
        std::cout << "Enter number of messages to send\n" << "";
        std::cin >> number_of_messages;
    } while (number_of_messages <= 0 || number_of_messages >= 15);


    // form command line interface ?
    wsprintf(
            lpszComLine,
            "D:\\Other\\HomeWork\\System Programming\\LR5\\CLIENT_SERVER\\client\\cmake-build-debug\\client.exe %d %d",
            hInheritWritePipe,
            number_of_messages
    );

    // creating (launching) this process
    if (!CreateProcess(NULL, lpszComLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &startup_info,
                       &process_information))
    {
        std::cout << "Create process failed.\n";
        std::cout << "Press any key to finish.\n";
        _getch();
        return GetLastError();
    }

    CloseHandle(process_information.hProcess);
    CloseHandle(process_information.hThread);
    CloseHandle(hInheritWritePipe);

    for (std::size_t counter{0}; counter < number_of_messages; ++counter)
    {
        char received_data[80]{};
        DWORD dwBytesRead{};

        if (!ReadFile(hReadPipe, &received_data, sizeof(received_data), &dwBytesRead, NULL))
        {
            std::cout << "Read from the pipe failed.\n";
            std::cout << "Press any key to finish.\n";
            _getch();

            return GetLastError();
        }
        std::cout << "Received from client: " << received_data << '\n';
    }


    CloseHandle(hReadPipe);
    std::cout << "The process finished reading from the pipe.\n";
    std::cout << "Press any key to exit.\n";

    _getch();

    return 0;
}