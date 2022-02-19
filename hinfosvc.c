// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
void subString(char string[], char subString[], int from, int length)
{
    int c = 0;
    while (c < length - from) // pokial je c < dlzka substringu
    {
        subString[c] = string[from + c]; // prepisujem hodnoty tak, ze do nulteho indexu subStringu
                                         // ulozim hodnotu v zavislost od c + zaciatocneho inxedu
        c++;                             // inkrementujem c
    }
    subString[c] = '\0'; // ukoncujem string znakom \0
    return;              // vraciam vsetky hodnoty, moj substring je ulozeny v char subString[]
}
void getString(char *array, char *result, char *delimeter, int get)
{
    int stringLength = strlen(array);
    int i = 0, setPoint = 0, lastSetPoint = 0, coll = 1;
    for (i = setPoint; i < stringLength; i++) // iterujem po kazdom znaku riadku az po koniec(jeho dlzku)
    {
        if (array[i] == delimeter[0]) // ak  som narazil na oddelovac buniek
        {
            setPoint = i;    // koniec stlpca
            if (coll == get) // ak som v spravnom stlpci
            {
                subString(array, result, lastSetPoint, setPoint); // volam funkciu na substring
                return;
            }
            lastSetPoint = setPoint + 1; // zaciatok noveho stlpca prepisem ako koniec predosleho + 1
            coll++;
        }
    }
    if (coll == get) // ak som na poslednom stlpci tak mi ho for nevypise lebo nevie kde konci, len
                     // kde zacina, teda tu volam funkciu s parametrami zaciatku stlpca a koncom celeho riadku(stringu)
    {
        subString(array, result, lastSetPoint, i);
        return;
    }
}
float cpu()
{
    char previous[1024] = {0};
    char current[1024] = {0};
    FILE *cmd = popen("cat /proc/stat | awk '/^cpu /{flag=1}/^intr/{flag=0}flag'", "r");
    fgets(previous, sizeof(previous), cmd);
    pclose(cmd);
    sleep(1);
    cmd = popen("cat /proc/stat | awk '/^cpu /{flag=1}/^intr/{flag=0}flag'", "r");
    fgets(current, sizeof(current), cmd);
    pclose(cmd);
    printf("riadky z vypisu \n%s \n%s\n", previous, current);
    char string_prev[10] = {0};
    char string_curr[10] = {0};
    float numb_prev = 0;
    float numb_curr = 0;
    // vypocet vyuzitia procesora
    for (int i = 1; i < 10; i++)
    {
        getString(previous, string_prev, " ", i);
        getString(current, string_curr, " ", i);
        numb_prev += atoi(string_prev);
        numb_curr += atoi(string_curr);
    }
    printf("suma sumaru\nprev : %f\ncurr : %f\n", numb_prev, numb_curr);
    numb_curr -= numb_prev;
    getString(previous, string_prev, " ", 6);
    getString(current, string_curr, " ", 6);
    numb_prev = atoi(string_curr) - atoi(string_prev);
    printf("delta : %f\nidle : %f\nvypocet : %.2f / %.2f\n", numb_curr, numb_prev, numb_curr - numb_prev, numb_curr);
    return 100 * (numb_curr - numb_prev) / numb_curr;
}

int main(int argc, char const *argv[])
{
    if (argc < 2 || argc == 1) // kontrola argumentov
    {
        perror("Argument error");
        return (EXIT_FAILURE);
    }
    int port = atoi(argv[1]);
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char info[1024] = {0};
    char msg[1024] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) // vytvaranie socketu, zapisanie descriptora do server_fd
    {
        perror("Socket failed");
        return (EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) // pripojenie socketu na port z argumentu
    {
        perror("Setsockopt failed");
        return (EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        return (EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        return (EXIT_FAILURE);
    }

    while (1)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            return (EXIT_FAILURE);
        }
        valread = read(new_socket, buffer, 1024);
        printf("%s", buffer);
        getString(buffer, info, "/", 2); // ziskanie parametrov z URL
        getString(info, info, " ", 1);
        char result[1024] = {0};
        if (!strcmp("hostname", info))
        {
            FILE *cmd = popen("cat /proc/sys/kernel/hostname", "r"); // ziskanie hostname-u
            fgets(result, sizeof(result), cmd);
            pclose(cmd);
            strcpy(msg, "HTTP/1.1 200 OK\r\nContent-Type: text/plain;\r\n\r\n");
            strcat(msg, result);
            send(new_socket, msg, strlen(msg), 0);
        }
        else if (!strcmp("cpu-name", info))
        {
            FILE *cmd = popen("cat /proc/cpuinfo | grep \"model name\" | head -n 1 | awk -F \':\' \'{print $2}\'", "r"); // ziskanie nazvu procesora
            fgets(result, sizeof(result), cmd);
            pclose(cmd);
            strcpy(msg, "HTTP/1.1 200 OK\r\nContent-Type: text/plain;\r\n\r\n");
            strcat(msg, result);
            send(new_socket, msg, strlen(msg), 0);
        }
        else if (!strcmp("load", info))
        {
            strcpy(msg, "HTTP/1.1 200 OK\r\nContent-Type: text/plain;\r\n\r\n");
            strcpy(result, "");
            sprintf(result, "%.2f", cpu()); // volanie funkcie na vypocet vyuzitia procesora
            strcat(msg, result);
            strcat(msg, "%");
            send(new_socket, msg, strlen(msg), 0);
        }
        else
        {
            strcpy(msg, "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain;\r\n\r\n"); // ak bol zadany iny poziadavok
            send(new_socket, msg, strlen(msg), 0);
        }
        close(new_socket);
    }
    return 0;
}