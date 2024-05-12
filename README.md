Library under development, everything here is subject to change.

# Mediocre

Mediocre, or ***M**ediocre **E**xtraction of **D**ata **I**nvolving an **O**ptical **C**haracter **R**ecognition 
**E**ngine*, is a configurable application for extracting and emitting data in real-time from videos.

Key libraries used:
- [gRPC](https://github.com/grpc/grpc) for exposing functions in a language and platform neutral manner
- [OpenCV](https://github.com/opencv/opencv) for various image processing methods
- [Tesseract](https://github.com/tesseract-ocr/tesseract) for optical character recognition

## Install and Run

### Docker (for insecure non-web requests)

Currently, the best way to run mediocre is via the pre-built docker images, though there are plans to provide 
executables for various platforms in the future.

Docker hub: https://hub.docker.com/repository/docker/obroomhall/mediocre

Tag convention:
- `obroomhall/mediocre:master` is the most up-to-date image
- `obroomhall/mediocre:pr-x` is built from each pull request, where `x` is the pull request number
- `obroomhall/mediocre:sha-abc` is built from each commit, where `abc` is the commit hash
- `obroomhall/mediocre:*-cache` exist to speed up subsequent builds, these are not runnable
- There will also be some `vX.Y.Z` tags after the first release

You should expose port `50051` for the gRPC server, for example:

```commandline
docker run -p 50051:50051 --name mediocre obroomhall/mediocre:master
```

### Docker (for secure web requests)

The C++ implementation of gRPC 
[cannot handle gRPC-Web requests directly.](https://github.com/grpc/grpc-web/discussions/1348#discussioncomment-6569324)
Therefore, if you wish to query mediocre from the web, you must use a proxy which can receive gRPC-Web requests, and
pass them to mediocre as standard gRPC requests. You can either follow the 
[documentation for gRPC-Web](https://github.com/grpc/grpc-web), or try the docker-compose setup in [compose.yaml](proxy/compose.yaml):

1. Change to the proxy directory `cd proxy`
2. Start up the docker containers `docker-compose up -d`
3. Add [minica.pem](proxy/certificates/minica.pem) to your browsers trusted certificates (or supply your own)

## Usage

### gRPC Tools

One way to invoke the functions in mediocre is to use a gRPC tool, some of which are listed in the 
[grpc-ecosystem](https://github.com/grpc-ecosystem/awesome-grpc#tools) project. Ideally choose a tool that uses gRPC
server reflection, so that it can automatically detect which methods mediocre exports.

Some that I have used in the past:
- [grpc-client-cli](https://github.com/vadimi/grpc-client-cli) - an interactive gRPC client command line tool
- [grpcui](https://github.com/fullstorydev/grpcui) - an interactive web UI for gRPC

### Custom Client

You can also use invoke mediocre from any language that has gRPC support, for example; C/C++, C#, Go, Java, Python. 
The full list of officially supported languages can be found in the [gRPC docs](https://grpc.io/docs/).

For some languages, you can add a dependency to mediocre via a 
[Buf.Build Remote Package](https://buf.build/broomy/mediocre/assets), this is the easiest method, but is limited to
the languages currently supported by the Buf Schema Registry.

If there is no remote package available for your language, you must build the protobuf source files yourself.
Instructions on how to do that can be found at https://buf.build/plugins.

For example, to build source files for C#, you could create a `buf.gen.yaml` as:
```yaml
version: v1
managed:
  enabled: true
plugins:
  - plugin: buf.build/grpc/csharp:v1.56.0
    out: gen
  - plugin: buf.build/protocolbuffers/csharp
    out: gen
```

And then build with `buf generate`.

## Documentation

Documentation can be found on the [Buf Schema Registry](https://buf.build/broomy/mediocre).

## Contributing

Mediocre was developed in [JetBrains CLion](https://www.jetbrains.com/clion/), and though other IDEs can be used the
instructions below are tailored towards building and running in CLion.

### Build

1. Run the `Docker Toolchain with Cache` script run configuration to build the latest image as `mediocre:local-develop`
   1. This uses registry cache to speed up the initial build (should finish building in a matter of seconds)
   2. Use this script if there have been any significant changes to the docker image (full rebuilds take about an hour)
2. Go to `File > Settings > Build, Execution, Deployment` and add a `Docker` toolchain
   1. For image use the previously built `mediocre:local-develop` image
   2. In container settings add `-p 0.0.0.0:50051:50051`
   3. For CMake use `cmake`
   4. For debugger, use `\local\gdb\install\bin\gdb`
3. Build the protobuf files using the `proto-objects` cmake run configuration

### Run

For library source file changes, run the `mediocre` cmake run configuration.

Otherwise, for docker changes, run the `Build and Run` docker run configuration.
