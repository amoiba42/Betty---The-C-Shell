#include "headers.h"

void remove_triple_newline(char *str)
{
    char *pos;
    while ((pos = strstr(str, "\n\n\n")) != NULL)
    {
        memmove(pos, pos + 3, strlen(pos + 3) + 1);
    }
}

void strip_html(char *html, char **text, size_t *text_size)
{
    size_t i = 0;
    size_t k = 0;
    size_t current_size = *text_size;

    *text = malloc(current_size);
    if (*text == NULL)
    {
        perror(red"Error: Unable to allocate memory for text"reset_color);
        exit(EXIT_FAILURE);
    }

    while (html[i])
    {
        if (k >= current_size - 1)
        {
            current_size *= 2;
            *text = realloc(*text, current_size);
            if (*text == NULL)
            {
                perror(red"Error: Unable to reallocate memory"reset_color);
                exit(EXIT_FAILURE);
            }
        }

        if (html[i] == '<')
        {
            while (html[i] && html[i] != '>')
                i++;
        }
        else
        {
            (*text)[k++] = html[i];
        }
        i++;
    }

    (*text)[k] = '\0';
    remove_triple_newline(*text);
}

void showpages(char **args)
{
    int sockfd;
    struct sockaddr_in server_addr;
    struct hostent *server;
    char request[MAX];
    char *response = NULL;
    size_t response_size = 0;
    char *stripped_response = NULL;
    size_t stripped_response_size = MAX; 
    ssize_t n;
    size_t total_size = 0;

    server = gethostbyname("man.he.net");
    if (server == NULL)
    {
        printf(red"Error: No such host\n"reset_color);
        exit(EXIT_FAILURE);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror(red"Error: Unable to create socket"reset_color);
        exit(EXIT_FAILURE);
    }

    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);
    server_addr.sin_port = htons(80);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror(red"Error: Unable to connect"reset_color);
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    snprintf(request, sizeof(request),
             "GET /?topic=%s&section=all HTTP/1.1\r\n"
             "Host: man.he.net\r\n"
             "Connection: close\r\n\r\n",
             args[0]);

    if (write(sockfd, request, strlen(request)) < 0)
    {
        perror(red"Error: Unable to send request"reset_color);
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    response_size = 8192; 
    response = malloc(response_size);
    if (response == NULL)
    {
        perror(red"Error: Unable to allocate memory for response"reset_color);
        exit(EXIT_FAILURE);
    }

    while ((n = read(sockfd, response + total_size, response_size - total_size - 1)) > 0)
    {
        total_size += n;

        if (total_size >= response_size - 1) 
        {
            response_size *= 2;
            response = realloc(response, response_size);
            if (response == NULL)
            {
                perror(red"Error: unable to reallocate memory"reset_color);
                close(sockfd);
                exit(EXIT_FAILURE);
            }
        }
    }

    if (n < 0)
    {
        perror(red"Error: unable to read response"reset_color);
        free(response);
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    response[total_size] = '\0';

    char *html_start = strstr(response, "text/html");
    if (html_start != NULL)
    {
        html_start += strlen("text/html");
        strip_html(html_start, &stripped_response, &stripped_response_size);
        printf("%s", stripped_response);
    }

    // Clean up
    free(response);
    free(stripped_response);
    close(sockfd);
}
