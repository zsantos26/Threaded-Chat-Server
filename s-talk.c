
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
 * Main function for initializing the chat application.
 * Checks command line arguments, initializes sockets and threads.
 */
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
  
    return 0;
}
