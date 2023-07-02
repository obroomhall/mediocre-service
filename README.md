Library under development, everything here is subject to change.

# Mediocre

Mediocre, or ***M**ediocre **E**xtraction of **D**ata **I**nvolving an **O**ptical **C**haracter **R**ecognition **E**ngine*, is a configurable application for extracting and emitting data in real-time from videos. 

## Setup in CLion

1. If running in Windows go to `Tools > Terminal` and set `Shell path` to `bash`
2. Run the `Docker Build` script run configuration to build the latest image as `mediocre:local`
3. Go to `File > Settings > Build, Execution, Deployment` and add a `Docker` toolchain
   1. For image use the previously built `mediocre:local` image
   2. In container settings add `-p 0.0.0.0:50051:50051`
   3. For CMake use `cmake`
4. Go to the `CMake` tab in the toolbar (usually pinned to the bottom of CLion) and reload the project
5. Run the project
   1. If making changes only to library source files, use the `mediocre` cmake run configuration
   2. If making changes to docker files, use `Build and Run` docker run configuration
