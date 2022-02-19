# Project Title

Lightweight HTTP server implemented in C

## Getting Started

A server implemented in C language that communicates via the HTTP protocol, which provides various information about the system. The server listens on the specified port and returns the required information according to the url. The answer type is text / plain. Communication with the server is possible using both a web browser and tools such as wget and curl.

### Prerequisites

-   GCC

### Installation

```
$ Copy repository into your local directiory
$ Type 'make' into command line, you should get result similar to 'gcc -std=gnu99 -o hinfosvc hinfosvc.c'
$ Now you are ready to start the server
```

## Usage

A few examples of useful commands and/or tasks.

```
$ Start the server with ./hinfosvc 'port number'
$ Now with server up and running you can request data using http://localhost:'port number'/'data you want to recieve' or in command line by using either GET or curl http://localhost:'port number'/'data you want to recieve'
$ You can request your hostname using http://localhost:'port number'/hostname
$ You can request your cpu name using http://localhost:'port number'/cpu-name
$ You can request your current system load using http://localhost:'port number'/load

```

## Author

Tomáš Ondrušek
VUT FIT
2BIT
3.2022
