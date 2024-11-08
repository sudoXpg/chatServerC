# Chat Server with Spam Filter and Logging

This project is a C-based chat server that supports multiple client connections, spam filtering, logging of user messages, and dynamic handling of client connections. The server includes a `Makefile` to simplify the build and execution process.

## Features

- **Multi-Client Support**: Allows multiple clients to connect and communicate through the server.
- **Spam Filter**: Detects and kicks users who repeatedly use censored words.
- **Logging**: Logs IPs, usernames, and chat messages for reference.
- **Chat Restoration**: When a new user joins, they see the chat history.
- **User Registration**: Each client registers a username upon joining.
  
## Project Structure

- `chat.h`: Contains definitions, function prototypes, and global variables.
- `chat.c`: Implements the server logic, client handling, spam filtering, and logging.
- `badwords.h`: Defines censored words that trigger the spam filter.
- `Makefile`: Provides commands to compile, run, and clean up the project.

## Getting Started

### Prerequisites

- GCC compiler (usually pre-installed on most Linux systems)
- Basic networking tools (`telnet` or `nc` to connect as a client)

### Building and Running the Server

1. **Clone the Repository**
    ```bash
    git clone https://github.com/sudoXpg/chatServerC
    cd chatServerC
    ```

2. **Build and Run the Server**
    Use the `Makefile` to compile and run the server:
    ```bash
    make
    ```

   This command will:
   - Compile `chat.c` into an executable named `chat`.
   - Run the server on port `9034` with logging enabled.

3. **Connect Clients**
   Use a TCP client (like `telnet` or `nc`) to connect to the server:
   ```bash
   telnet localhost 9034
   ```

Upon connection, users will be prompted to enter a username.
Additional Makefile Commands


Clean Up: To remove the compiled executable and logs, use:
```bash
make clean
```
Run the Server: To start the server without recompiling, use:
```bash
make run
```

### License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

### Contribution

Contributions are welcome! Please fork this repository, make your changes, and submit a pull request.
<br><br><br><br>
## Future Updates

Here are some ideas for future improvements to this chat server:

- **Dynamic User List Allocation**: Shift from a fixed-size user list to a dynamically allocated structure, allowing more clients to connect as needed.
- **Emoji Responses**: Add emoji responses for specific keywords (e.g., typing "happy" would send a 😊 emoji).
- **User Authentication**: Implement optional password protection to allow registered users to reserve usernames.
- **Private Messaging**: Enable users to send direct messages to each other.
- **Admin Commands**: Allow an admin to kick or mute users manually, view active users, or control the server remotely.
- **Enhanced Logging Options**: Provide configuration options for logging levels (e.g., minimal, verbose) and log rotation for better file management.
- **Encrypted Connections**: Add support for encrypted connections (e.g., using SSL/TLS) for increased security.


These future updates would add more functionality, security, and ease of use, making the chat server more robust and user-friendly.





### Acknowledgments

This project was heavily inspired by Beej's Guide to Network Programming, which provides an excellent introduction to sockets and network programming in C.