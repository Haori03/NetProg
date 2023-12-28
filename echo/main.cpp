#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdexcept>

using namespace std;

class critical_error:public std::runtime_error
{
public:
    critical_error(const std::string& s):std::runtime_error(s) {}
};

int main()
{
    try {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            throw critical_error("Ошибка создания сокета");
        }

        struct sockaddr_in servaddr;
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(7);
        inet_pton(AF_INET, "82.179.90.12", &servaddr.sin_addr);

        if (connect(sock, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
            throw critical_error("Ошибка подключения к серверу");
        }

        char buffer[1024] = {0};
        cout << "Введите сообщение: ";
        cin.getline(buffer, 1024);

        if (send(sock, buffer, strlen(buffer), 0) < 0) {
            throw critical_error("Ошибка отправки сообщения на сервер");
        }

        if (recv(sock, buffer, 1024, 0) < 0) {
            throw critical_error("Ошибка приема сообщения от сервера");
        }

        cout << "Ответ от сервера: " << buffer << endl;

        close(sock);
    } catch(critical_error &e) {
        cout << e.what() << endl;
    }

    return 0;
}
