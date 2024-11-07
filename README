# Chat Server with Polling

A simple, multi-user chat server implemented in C using `poll()`. This server can handle multiple simultaneous connections, broadcasting messages from any user to all others. It uses non-blocking socket I/O for scalability and is built to work over IPv4 and IPv6.

## Features

- **Multi-client Support**: Handles multiple users at once using the `poll()` function.
- **Message Broadcasting**: Relays messages from one user to all others connected to the server.
- **Dynamic Socket Management**: Adds and removes sockets dynamically, adjusting for new connections or disconnections in real-time.
- **IPv4 and IPv6 Support**: Compatible with both IP versions using an adaptable socket interface.
- **Connection Logs**: Logs new connections and disconnections in the server output.

## Getting Started

### Prerequisites

- **C Compiler**: A GCC or similar C compiler for compiling the server code.
- **POSIX-compatible System**: Tested on Linux; should work on other POSIX systems (e.g., macOS).

### Installation

1. Clone this repository:
   ```bash
   git clone https://github.com/sudoXpg/chatServerC
   cd chatServerC
    ```

2. Run the make file:
    ``` 
    make
    ``` 
### User login
Connect multiple clients using a telnet client or any TCP client with the server's IP and port (9034 by default):

    telnet <hostname> 9034

Type messages in the telnet window. Each message from one client will be broadcasted to all other connected clients.

### License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

### Contribution

Contributions are welcome! Please fork this repository, make your changes, and submit a pull request.
<br><br><br><br>
# Future Updates

### 1. **User Authentication**
   - Implement a basic authentication system to allow users to log in with usernames and passwords.
   - Use hashed passwords for security.

### 2. **Private Messaging**
   - Add support for direct, private messages between two users within the same chat server.
   - Allow users to send commands like `/msg username message`.

### 3. **Persistent Chat Logs**
   - Save chat logs to a file or database, so conversations can be reviewed later.
   - Implement log rotation or archiving to manage file sizes over time.

### 4. **User Commands and Server Commands**
   - Add user commands such as `/list` to show active users or `/quit` to leave the chat.
   - Implement server-side commands for administrative control, e.g., to kick or ban users.

### 5. **Enhanced Error Handling and Logging**
   - Improve error handling to provide clearer messages for common errors (e.g., connection timeouts).
   - Add comprehensive logging to track user activities, errors, and connection details for debugging.

### 6. **Encryption (SSL/TLS)**
   - Add SSL/TLS support to encrypt data transmission and secure user communication.
   - Use OpenSSL or similar libraries to integrate SSL/TLS for secure connections.

### 7. **Cross-Platform Compatibility**
   - Improve compatibility with Windows, possibly using platform-independent libraries to handle sockets and polling.

### 8. **WebSocket Support for Web-Based Chat Clients**
   - Add WebSocket support to allow users to connect via web browsers.
   - Create a simple web client interface using HTML and JavaScript for more accessibility.

### 9. **Rate Limiting and Spam Prevention**
   - Implement rate limiting to prevent users from flooding the chat with too many messages.
   - Add basic spam filters or flagging mechanisms for inappropriate content.

### 10. **Improved User Interface for Clients**
   - Enhance the command-line interface to display usernames, timestamps, and better message formatting.
   - Consider integrating simple text-based UI libraries for better user experience in the terminal.

### 11. **Automatic Reconnection for Clients**
   - Allow clients to automatically attempt reconnection if the server temporarily disconnects.
   - Save session information so users can pick up where they left off.

These future updates would add more functionality, security, and ease of use, making the chat server more robust and user-friendly.





### Acknowledgments

This project was heavily inspired by Beej's Guide to Network Programming, which provides an excellent introduction to sockets and network programming in C.