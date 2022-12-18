#include <iostream>
#include <winsock2.h>


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
    SOCKET socket_handle{socket(AF_INET, SOCK_STREAM, 0)};

    // stores connection settings
    SOCKADDR_IN socket_information{};
    memset(&socket_information, 0, sizeof(socket_information));
    socket_information.sin_family = AF_INET; // work only with ipv4 addresses
    socket_information.sin_port = htons(1234);

    char choice{};
    std::cout << "s => server" << '\n' << "c => client" << '\n';
    std::cin >> choice;

    if (choice == 'c')
        run_client(socket_handle, socket_information);
    else if (choice == 's')
        run_server(socket_handle, socket_information);


    closesocket(socket_handle);

    return 0;
}


void run_client(const SOCKET &socket_handle, SOCKADDR_IN &socket_information)
{
    socket_information.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    connect(socket_handle, reinterpret_cast<const sockaddr *>(&socket_information), sizeof(socket_information));

    char message[kMaxMessageBufferSize];
    std::cin >> message;

    // socket descriptor, pointer to buffer, buffer size
    send(socket_handle, message, kMaxMessageBufferSize, 0);
}

void run_server(const SOCKET &socket_handle, const SOCKADDR_IN &socket_information)
{
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
            std::cout << buf << '\n';
    }
}