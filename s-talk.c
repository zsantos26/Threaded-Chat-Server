/*
 * s-talk.c
 *
 * Description: Main file for chat server. Contains main(), thread functions to enable message interactions, and variable initialization. 
 * Author: Zeth Santos
 * Created: Oct 9 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "list.h"
#include <pthread.h>

// Size of the buffer used for input and output messages
#define BUFFER_SIZE 1024

// Thread handlers for input, send, receive, and output operations
pthread_t input_thread, send_thread, receive_thread, output_thread;

// Socket file descriptor
int sockfd;

// Structures for holding information about server and client addresses
struct sockaddr_in serverAddr, clientAddr;

// Lists for storing input and output messages
List* inputList, *outputList;

// Mutexes for synchronizing access to the input and output lists
pthread_mutex_t inputList_mutex, outputList_mutex;

// Condition variables for signaling when input and output lists are not empty
pthread_cond_t inputList_cond, outputList_cond;

/**
 * Thread function for handling user input.
 * Reads user input from the standard input and appends it to the input list.
 * Signals when input is available. Exits when user enters "!".
 */
void* input_function(void* arg) {
    char buffer[BUFFER_SIZE];
    while (1) {
        printf("Enter a message: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strlen(buffer) - 1] = '\0';

        pthread_mutex_lock(&inputList_mutex);
        List_append(inputList, strdup(buffer));
        pthread_cond_signal(&inputList_cond);
        pthread_mutex_unlock(&inputList_mutex);

        if (strcmp(buffer, "!") == 0) {
            break;
        }
    }
    return NULL;
}

/**
 * Thread function for sending messages.
 * Takes messages from the input list and sends them to the remote user.
 * Exits when message to send is "!".
 */
void* send_function(void* arg) {
    char* message;
    while (1) {
        pthread_mutex_lock(&inputList_mutex);
        while ((message = List_remove(inputList)) == NULL) {
            pthread_cond_wait(&inputList_cond, &inputList_mutex);
        }
        pthread_mutex_unlock(&inputList_mutex);

        ssize_t bytesSent = sendto(sockfd, message, strlen(message), 0, (struct sockaddr*)&clientAddr, sizeof(clientAddr));
        if (bytesSent < 0) {
            perror("Error sending data");
        }

        if (strcmp(message, "!") == 0) {
            close(sockfd);
            free(message);
            exit(0);
        }

        free(message);
    }
    return NULL;
}

/**
 * Thread function for receiving messages.
 * Receives messages from the remote user and adds them to the output list.
 * Exits when received message is "!".
 */
void* receive_function(void* arg) {
    char buffer[BUFFER_SIZE];
    socklen_t addr_size = sizeof(clientAddr);

    while (1) {
        ssize_t bytesReceived = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&clientAddr, &addr_size);
        if (bytesReceived < 0) {
            perror("Error receiving data");
            continue;
        }

        buffer[bytesReceived] = '\0';

        pthread_mutex_lock(&outputList_mutex);
        List_append(outputList, strdup(buffer));
        pthread_cond_signal(&outputList_cond);
        pthread_mutex_unlock(&outputList_mutex);

        if (strcmp(buffer, "!") == 0) {
            printf("Connection terminated by the other side.\n");
            close(sockfd);
            pthread_mutex_lock(&outputList_mutex);
            List_free(outputList, free);
            pthread_mutex_unlock(&outputList_mutex);
            exit(0);
        }
    }
    return NULL;
}

/**
 * Thread function for outputting messages.
 * Takes messages from the output list and prints them to the terminal.
 */
void* output_function(void* arg) {
    char* message;
    while (1) {
        pthread_mutex_lock(&outputList_mutex);
        while ((message = List_remove(outputList)) == NULL) {
            pthread_cond_wait(&outputList_cond, &outputList_mutex);
        }
        pthread_mutex_unlock(&outputList_mutex);

        printf("\nReceived: %s\n", message);
        free(message);
    }
    return NULL;
}



int main(int argc, char *argv[]) {
    // Check command line arguments
    if (argc != 4) {
        printf("Usage: %s [my port number] [remote machine name] [remote port number]\n", argv[0]);
        exit(1);
    }

    // Convert port numbers from strings to integers
    int myPort = atoi(argv[1]);
    char* remoteMachineName = argv[2];
    int remotePort = atoi(argv[3]);

    // Ensure that valid port numbers are used
    if (myPort < 1024 || remotePort < 1024) {
        printf("Please use port numbers greater than or equal to 1024.\n");
        exit(1);
    }

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        exit(1);
    }

    // Initialize server address structure
    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(myPort);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Initialize client address structure
    memset(&clientAddr, '\0', sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(remotePort);
    clientAddr.sin_addr.s_addr = inet_addr(remoteMachineName);

    // Bind socket
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Error binding socket");
        close(sockfd);
        exit(1);
    }

    // Initialize mutexes and condition variables
    pthread_mutex_init(&inputList_mutex, NULL);
    pthread_cond_init(&inputList_cond, NULL);
    inputList = List_create();

    pthread_mutex_init(&outputList_mutex, NULL);
    pthread_cond_init(&outputList_cond, NULL);
    outputList = List_create();

    // Create threads for input, send, receive, and output operations
    pthread_create(&input_thread, NULL, input_function, NULL);
    pthread_create(&send_thread, NULL, send_function, NULL);
    pthread_create(&receive_thread, NULL, receive_function, NULL);
    pthread_create(&output_thread, NULL, output_function, NULL);

    // Wait for threads to finish
    pthread_join(input_thread, NULL);
    pthread_join(send_thread, NULL);
    pthread_join(receive_thread, NULL);
    pthread_join(output_thread, NULL);

    // Clean up mutexes and condition variables
    pthread_mutex_destroy(&inputList_mutex);
    pthread_cond_destroy(&inputList_cond);
    pthread_mutex_destroy(&outputList_mutex);
    pthread_cond_destroy(&outputList_cond);

    // Close socket and exit
    close(sockfd);
    return 0;
}
