<p align="center">
<img src="./public/img/img.png" alt="Logo" width="500" />
</p>

# Http-Cerver

A lightweight HTTP server written in C using sockets, capable of handling basic requests and serving static files.

## Features

- Handles HTTP GET requests
- Serves static files (HTML, CSS, JS, images, etc.) from the `public/` directory
- Returns 404 for missing files
- Simple request parsing and response headers
- Easily extensible for more HTTP methods

> [!note]
>This project is a learning exercise in C programming and socket programming. It is not intended for production use.

## Directory Structure

```
Http-Cerver
├── includes/
│   └── http/
├── public/
│   └── img/
│   └── www/
│       └── css/
│       └── html/
├── src/
│   ├── http/
│   │   ├── parser.c
│   │   ├── responce_headers.c
│   │   └── responce.c
│   ├── main.c
│   ├── proxy.c
│   ├── logging.c
│   └── server.c
├── scripts/
│   ├── build.sh // Build the project
│   └── format.sh // Format the code using clang-format
├── build/
├── bin/
├── CMakeLists.txt
├── LICENSE
├── .clang-format
├── .gitignore
└── README.md
```

## Getting Started

### Prerequisites

- GCC or Clang
- CMake (for building)

### Build

```sh
./scripts/build.sh
```

The server binary will be in `build/http_server`.

### Run

```sh
./build/http_server
```

By default, the server serves files from the `public/` directory.

### Example

Open [http://localhost:4500/](http://localhost:4500/) in your browser to see the default welcome page.

## TODO

- [X] Support HTTPS
- [ ] add dynamic config
- [ ] Support HTTP/2
- [ ] Implement UDP from scratch
- [ ] Implement HTTP/3 from scratch

## License

This project is licensed under the [GNU GPL v3](LICENSE).

## Contributing

Feel free to open issues or submit pull requests for improvements and new features!

## Author

[Ox03bb](https://github.com/Ox03bb/Http-Cerver)