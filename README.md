# eXaDrumsServer

A JSON-RPC 2.0 server for [libeXaDrums](https://github.com/SpintroniK/libeXaDrums) that is mainly meant to be used with [eXaDrumsLite](https://github.com/SpintroniK/eXaDrumsLite).

## Dependencies 

eXaDrumsServer depends on eXaDrums and libeXaDrums. In order to install libeXaDrums, please refer to the [project's page](https://github.com/SpintroniK/libeXaDrums). Note that the best way to do that, is to get the latest realease's packages and install them. In particular, the libexadrums-dev package is required, as it provides the header files that are needed to compile this project.

The project also needs the configuration files from eXaDrums.
Although it only needs the Data folder, it is best to clone the entire [eXaDrums repository](https://github.com/SpintroniK/eXaDrums) and compile the project to make sure that the configuration is right.

All the other dependencies ([JSON for modern C++](https://github.com/nlohmann/json) and [CppSockets
](https://github.com/SpintroniK/CppSockets)) are included in this repository.

## Building eXaDrumsServer

In order to build eXaDrumsServer under linux, run the following command: 

```g++ -std=c++14 -O3 eXaDrumsServer.cpp -o eXaDrumsServer -lexadrums```.

## Usage

To start the server, run the following command in the executable's folder: 

```./eXaDrumsServer /home/user/path/to/eXaDrums/Data/```

where `/home/user/path/to/eXaDrums/Data/` is the path to the eXaDrums' Data/ folder.
