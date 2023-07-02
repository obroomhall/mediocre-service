Library under development, everything here is subject to change.

# Mediocre

Mediocre, or ***M**ediocre **E**xtraction of **D**ata **I**nvolving an **O**ptical **C**haracter **R**ecognition **E**ngine*, is a configurable application for extracting and emitting data in real-time from videos. 

## Local Development in CLion

### Windows Requirements

Other platforms can skip this step. Windows users must first configure CLion to use the bash shell by default:
1. Install bash or git bash
2. Go to `Tools > Terminal` and set `Shell path` to the location of `bash`

### Build

1. Run the `Docker Build with Cache` script run configuration to build the latest image as `mediocre:local`
   1. This uses registry cache to speed up the initial build (should finish building in a matter of seconds)
   2. Use this script if there have been any significant changes to the docker image (full rebuilds take about an hour)
2. Go to `File > Settings > Build, Execution, Deployment` and add a `Docker` toolchain
   1. For image use the previously built `mediocre:local` image
   2. In container settings add `-p 0.0.0.0:50051:50051`
   3. For CMake use `cmake`
3. Go to the `CMake` tab in the toolbar (usually pinned to the bottom of CLion) and reload the project

### Run

For library source file changes, use the `mediocre` cmake run configuration.

Otherwise, for docker changes, use the `Build and Run` docker run configuration.
