#include <iostream>
#include <winsock2.h>
#include <fstream>
#include <ctime>
#include <iomanip>


void run_server(const SOCKET &, const SOCKADDR_IN &);

void run_client(const SOCKET &, SOCKADDR_IN &);


const std::size_t kMaxMessageBufferSize{100};


int main()
{
    // structure contains information about the Windows Sockets implementation
    WSADATA windows_socket{};

    // initializes socket work (socket version, parameters object to fill)
    WSAStartup(MAKEWORD(2, 2), &windows_socket);

    // AF_INET => with type of address ipv4 (4 bytes address)
    // SOCK_STREAM => protocol TCP used
    // 0 => unused, so leave as 0

    // or SOCK_STREAM 0
    // or SOCK_DGRAM IPPROTO_UDP
    SOCKET socket_handle{socket(AF_INET, SOCK_STREAM, 0)};

    // stores connection settings
    SOCKADDR_IN socket_information{};
    memset(&socket_information, 0, sizeof(socket_information));
    socket_information.sin_family = AF_INET; // work only with ipv4 addresses
    socket_information.sin_port = htons(1234);

    char choice{};
    std::cout << "s => server" << '\n' << "c => client" << '\n';


    if (std::cin >> choice; choice == 'c')
        run_client(socket_handle, socket_information);
    else if (choice == 's')
        run_server(socket_handle, socket_information);

    closesocket(socket_handle);

    return 0;
}
// launch 2 instances of EXE
// in first run server, in second - client
// send messages from client to server
// after closing check file with logging


void run_client(const SOCKET &socket_handle, SOCKADDR_IN &socket_information)
{
    socket_information.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    connect(socket_handle, reinterpret_cast<const sockaddr *>(&socket_information), sizeof(socket_information));

    char message[kMaxMessageBufferSize];
    std::cin >> message;

    while (std::string{message} != "stop")
    {
        std::cin >> message;
        // socket descriptor, pointer to buffer, buffer size
        send(socket_handle, message, kMaxMessageBufferSize, 0);
    }
}

void run_server(const SOCKET &socket_handle, const SOCKADDR_IN &socket_information)
{
    std::ofstream write{};
    write.open("log.txt");

    bind(socket_handle, reinterpret_cast<const sockaddr *>(&socket_information), sizeof(socket_information));

    //socket, queue size
    listen(socket_handle, 100);

    char buf[kMaxMessageBufferSize];
    memset(buf, ' ', sizeof(buf));

    SOCKET client_socket{};
    SOCKADDR_IN client_addr{};

    int client_addr_size{sizeof(client_addr)};

    while ((client_socket = accept(socket_handle, reinterpret_cast<sockaddr *>(&client_addr), &client_addr_size)))
    {
        std::cout << "Connection OK" << '\n';

        while (recv(client_socket, buf, sizeof(buf), 0) > 0)
        {
            std::cout << buf << '\n';

            const auto t{std::time(nullptr)};
            const auto tm{*std::localtime(&t)};

            write << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << ' ';
            write << buf;
            write << '\n';
        }
        write.close();
    }
}