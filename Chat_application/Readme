#       MINI PROJECT: CHAT APPLICATION


## Overview


**Chat application** is a app chat between devices(clients) but not need server, clients can directly send and receive message through **Socket Connection* and *TCP protocol*

### Main functional:
        - Connect to 1 or more peers at the same time
        - Manage connections
        - Send messages to peer and received message from peer

- This is a mini project in *Linux Programming* course and focusing on **Multithread, socket/network programming, C programming**

---


## Features


### Functinal requirements:
- *Command Line Interface - CLI*:
        - `help`: Show available commands
        - `my ip & my port`: Show ip address and listening port
        - `connect <IP> <port>`: Connect to IP and port of peer
        - `list`: Show list connection
        - `terminate <ID>` close connection ID
        - `send <ID> <message>`: Send message to ID peer
        - `exit`: Close all peer connect and exit app
---
### Non-Functional Requirements

#### **Performance:**
- Support multiple concurrent connections using multi-threading or asynchronous I/O.
- Ensure the application does not hang when handling multiple connections or unexpected peer disconnections.

#### **Security:**
- Validate IP and port before establishing a connection.
- Prevent unauthorized or invalid connections.

#### **Reliability:**
- Handle errors such as:
  - Connection loss.
  - Unresponsive peers.
  - Messages exceeding 100 characters.
- Display clear and user-friendly error messages.

#### **Memory Management:**
- Free resources (sockets, memory) when a connection is closed or the program exits.
- Use tools like Valgrind to check for memory leaks in C/C++.

#### **Coding Standards:**
- Follow consistent naming conventions for variables and functions.
- Separate logic (processing) from the user interface (CLI) for better maintainability.
- Add comments to explain critical parts of the code.

#### **Clean Code:**
- Avoid code duplication.
- Break down complex logic into smaller functions/methods.
- Use meaningful and descriptive names for variables and functions.
- Implement robust error handling to prevent crashes.
---

### **Project structure**
```
── bin
│   ├── command.h
│   ├── connection.h
│   ├── define.h
│   ├── server.h
│   └── socket.h
├── Makefile
├── Reame.md
└── src
    ├── command.c
    ├── connection.c
    ├── main.c
    ├── server.c
    └── socket.c

```
---
