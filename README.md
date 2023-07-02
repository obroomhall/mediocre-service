Library under development, everything here is subject to change.

# Mediocre

Mediocre, or ***M**ediocre **E**xtraction of **D**ata **I**nvolving an **O**ptical **C**haracter **R**ecognition **E**ngine*, is a configurable application for extracting and emitting data in real-time from videos. 

## Local Development in CLion

### Windows Requirements

Other platforms can skip this step. Windows users must first configure CLion to use the bash shell by default:
1. Install bash or git bash
2. Go to `Tools > Terminal` and set `Shell path` to the location of `bash`

### Build

This is a little more complicated than I would have liked, but the docker run configurations are a big buggy at the moment.
1. Edit the `Build and Run` docker run configuration
   1. In `Before launch` add a reference to the `Docker Build` shell run configuration
   2. You may need instead to configure an `External tool` to run the shell script via git bash
2. Run the `Build and Run` docker run configuration to build the latest image as `mediocre:local`
3. Go to `File > Settings > Build, Execution, Deployment` and add a `Docker` toolchain
   1. For image use the previously built `mediocre:local` image
   2. In container settings add `-p 0.0.0.0:50051:50051`
   3. For CMake use `cmake`
4. Go to the `CMake` tab in the toolbar (usually pinned to the bottom of CLion) and reload the project

### Run

For library source file changes, use the `mediocre` cmake run configuration.

Otherwise, for docker changes, use the `Build and Run` docker run configuration.
