#include <windows.h>
#include <conio.h>
#include <iostream>


int main(int argc, char *argv[])
{
    const HANDLE hWritePipe{(HANDLE) atoi(argv[1])};

    const std::size_t count{(std::size_t) atoi(argv[2])};
    std::cout << "Going to send " << count << " messages\n" << '\n';

    
    std::cout << "Press any key to start communication.\n";
    _getch();


    std::size_t counter{0};
    while (counter < count)
    {
        DWORD dwBytesWritten{};
        char temp[80];
        std::cout << "Enter a word to send to server\n";
        std::cin >> temp;

        if (!WriteFile(
                hWritePipe,
                &temp,
                sizeof(temp),
                &dwBytesWritten,
                NULL))
        {
            std::cout << "Write to file failed.\n";
            std::cout << "Press any key to finish.\n";
            _getch();
            return GetLastError();
        }
        std::cout << temp << " is sent to pipe\n";

        ++counter;
    }

    CloseHandle(hWritePipe);
    std::cout << "The process finished writing to the pipe.\n";
    std::cout << "Press any key to exit.\n";
    _getch();
    return 0;
}