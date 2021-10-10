# HTTP Server
[Report Bug](https://github.com/agoryelov/COMP4981_HTTP/issues) - [Request Feature](https://github.com/agoryelov/COMP4981_HTTP/issues)

## About this project
This is an HTTP Server written in C that supports HTTP/1.0 protocol. You can configure the server using a build-in CLI.

### Key Features
* Fully supported HTTP GET and HTTP HEAD methods
* Updating server configuration with no downtime
* Multi-threading and multi-processing support

### Future Plans
* HTTP POST method
* HTTP over TLS (HTTPS)
* HTTP/1.1 protocol compliance

## How to run locally

### Prerequisites
Before building and running this server, make sure you have the following:
* POSIX-compliant operating system (Linux, Mac, FreeBSD, etc.)
* CMake version 3.17 or higher
* Libconfig library installed
* Ncurses library installed
* [Libdc](https://github.com/darcy-bcit/libdc) library installed

### Build and run
1. Clone this repository
1. Create a build directory inside the root folder
1. Use `cmake ../` to create build files
1. Use `cmake --build .` to build the project
1. Use `sudo ./server` to start the server with default settings
1. Open your browser to `localhost:<port>` to see the server running
