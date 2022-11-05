/****************
 * *FILENAME	      : Server.c
 *
 * *DESCRIPTION        : This file defines the functions that consists of various subfunctions
 * 			            to perform certain operations from server end.
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

// argc is the total number of parameter we are passing

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Port not provided . Program Terminated \n");
        exit(1);
    }

    int sockfd, newsockfd, portno, n;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen; // socklen_t is a datatype in socket.h 32 bit
    clilen = sizeof(cli_addr);
    bzero((char *)&serv_addr, sizeof(serv_addr)); // it clears all the data to what it reference to

    sockfd = socket(AF_INET, SOCK_STREAM, 0); // create socket
    if (sockfd < 0) {
        // if sock fd is resulting in failure
        error("Error opening socket");
        exit(0);
    }

    portno = atoi(argv[1]); // assign port no. from argv[1]
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) // bind the socket to server address
    {
        error("Binding Failed");
        exit(0);
    }

    // next step is our server is trying to connect
    listen(sockfd, 1); // make the server wait server for the client

    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen); // accept the incoming request to the server
    if (newsockfd < 0) {
        error("Error on Accept");
        exit(0);
    }

    send(newsockfd, "Connected to server\n", sizeof("Connected to server\n"),0);

    // user authentication
    char buffer[255]; // to store msg at send
    int errmsg;
    struct user newuser;
    recv(newsockfd, &newuser, sizeof(struct user), 0); // receving userid and password entered by the client
    if (strcmp(newuser.username, "useradmin") != 0) {
        errmsg = write(newsockfd, "Id not found !!!", strlen("Id not found !!!"));
        close(newsockfd);
        return 0;
    }

    if (strcmp(newuser.password, "******") != 0) {
        errmsg = write(newsockfd, "Wrong Password !!!", strlen("Wrong Password !!!"));
        close(newsockfd);
        return 0;
    }

    int msg1, msg2, msg3, answer, choice;

    char num[256] = "";
    char operatrs[256] = "";
    char expression[256] = "";
    while (1) {
        msg1 = write(newsockfd, "ENTER THE NUMBER OF OPERANDS FOLLOWED BY THE OPERANDS : ", strlen("ENTER THE NUMBER OF OPERANDS FOLLOWED BY THE OPERANDS : "));
        if (msg1 < 0) {
            error("error writing to socket");
        }
        read(newsockfd, &num, sizeof(num)); // read operands from client

        // defining numbers and operands array

        msg2 = write(newsockfd, "OPERATORS ALLOWED ARE ( + , - , * , /) !!! \nENTER THE  OPERATORS SEPERATED BY SEMICOLON ", strlen("OPERATORS ALLOWED ARE ( + , - , * , /) !!! \nENTER THE  OPERATORS SEPERATED BY SEMICOLON "));
        read(newsockfd, &operatrs, sizeof(operatrs)); // read operators from client

        tn = -1; // number stack(top) initialization
        to = -1; // operator stack(top) initialization

        int c1 = checkFormat(num);
        int c2 = checkFormat_operator(operatrs);

        int x;
        char msger[200];
        if (c1 == 0 || c2 == 0) {
            strcpy(msger, "Invalid !!! Enter Operands or Operators Correctly\n");
            x = write(newsockfd, &msger, sizeof(msger));

        }

        else {
            solve(expression, num, operatrs); // forming an expression using operands and operators string
            printf("Entered expression is : %s \n", expression);
            answer = eval(expression); // evaluate the expression formed and store it in the variable answer

            char finans[256] = "";

            sprintf(finans, "%d", answer); // convert the answer to string

            x = write(newsockfd, &finans, sizeof(finans)); // send the evaluated answer to the client

            if (x < 0) {
                error("answer printing error");
                exit(0);
            }
        }

        read(newsockfd, &x, sizeof(int)); // receive the choice entered by client
        if (x == 1)                       // continue is choosed
        {
            // initializing strings
            strcpy(num, "");
            strcpy(operatrs, "");
            strcpy(buffer, "");
            strcpy(expression, "");
        } else {
            break;
        }
    }
    close(newsockfd);
    close(sockfd);
    return 0;
}
