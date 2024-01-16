# Threaded Chat Server Application README

## Overview
Based on material from SFU course CMPT300 Operating Systems I

This Chat Server application is a simple, terminal-based chat application written in C. The program is capable of running on a single machine or across a network. The application employs multithreading to handle user input, sending, receiving, and displaying messages.

## Prerequisites
- C compiler (like 'gcc')
- make (for building the application)
- A Unix-like environment (Linux, macOS, etc.)

## Compilation
To compile the program, navigate to the directory containing the source code and run the following command:
`make`

This will create an executable named s-talk.

## Running the Application
s-talk requires three command-line arguments:
  1. Your Port Number: The port number on which your instance will listen.
  2. Remote Machine Name/IP Address: The hostname or IP address of the remote machine you wish to chat with.
  3. Remote Port Number: The port number on which the remote instance is listening.

Choose an IP address and a port number for the server to listen on. If you're running both instances on the same machine, you can use 127.0.0.1 (localhost) as the IP address. Select an available port number (e.g., 5000).
Run the program with the following command: `./s-talk [my port number] [remote machine name] [remote port number]`
### Example Usage
**Temrinal A (acting as Server)**
```
./s-talk 4444 127.0.0.1 5555
```
**Terminal B (acting as Client)**
```
./s-talk 5555 127.0.0.1 4444
```

### Ending Chat
To end the chat, any user can simply enter `!`
