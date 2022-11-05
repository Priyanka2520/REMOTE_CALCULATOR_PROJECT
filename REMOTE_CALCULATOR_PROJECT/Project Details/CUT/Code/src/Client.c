/****************
 * *FILENAME	      : Client.c
 *
 * *DESCRIPTION        : This file defines the functions that consists of various subfunctions
 * 			            to perform certain operations from client end.
 *
 *
 *
 *
 ***************/
#include "../Header/Header.h"

// function to print error message
void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {

    if (argc < 3) {
        fprintf(stderr, "usage %s hotname port\n", argv[0]);
        exit(1);
    }

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server; // This structure describes an Internet host.
    bzero((char *)&serv_addr, sizeof(serv_addr)); // reset the address buffer to zero

    portno = atoi(argv[2]);                   // assign the port no. from argv[2]
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // create a socket

    if (sockfd < 0) {
        error("Error opening socket");
        exit(0);
    }

    server = gethostbyname(argv[1]); // store internet host in server

    if (server == NULL) {
        fprintf(stderr, "no Such host");
        exit(0);
    }
    
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length); // giving socket address to the internet host
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) // server connection is made
    {
        error("Connection Failed !!");
        exit(0);
    }

    char buffer[256] = "";
    recv(sockfd, &buffer, sizeof(buffer), 0);
    printf("%s\n", buffer);

    struct user newuser = getdetails(1);                   // get userid and passsword from the user
    send(sockfd, &newuser, sizeof(newuser), 0); // send the user details to the server for validation

    char expression[256] = "";
    char nums[256] = "";
    char operands[256] = "";

    while (1) {

        bzero(buffer, 256);
        read(sockfd, buffer, 256);
        if (strcmp(buffer, "Id not found !!!") == 0 || strcmp(buffer, "Wrong Password !!!") == 0) {
            printf("%s\n", buffer);
            break;
        }
        printf("SERVER - %s", buffer);

        scanf("%s", nums);

        int x = write(sockfd, &nums, sizeof(nums));
        bzero(buffer, 256);
        read(sockfd, buffer, 256);
        printf("SERVER - %s", buffer);

        scanf("%s", operands);

        int y = write(sockfd, &operands, sizeof(operands));

        char answer[256];
        read(sockfd, &answer, sizeof(answer));
        printf("SERVER : %s\n", answer);
        int choice;
        printf("\n\n1.CONTINUE \n2.EXIT\n");
        scanf("%d", &choice);
        write(sockfd, &choice, sizeof(choice));

        if (choice != 1) {
            printf("YOU HAVE SELECTED TO EXIT \nEXIT SUCCESSFULLY!!!\n");
            break;
        }
    }

    close(sockfd);
    return 0;
}
