Library under development, everything here is subject to change.

# Mediocre

Mediocre, or ***M**ediocre **E**xtraction of **D**ata **I**nvolving an **O**ptical **C**haracter **R**ecognition **E**ngine*, is a configurable application for extracting and emitting data in real-time from videos. 

## Setup in CLion

1. Run the provided `Dockerfile` run configuration to build the `mediocre:dev` image
2. Go to `File > Settings > Build, Execution, Deployment`
    1. Add a `Docker` toolchain and reference the built image
    2. Add a `CMake` profile using the toolchain you just created
3. Go to the `CMake` tab in the toolbar (usually pinned to the bottom of CLion) and reload the project
4. Run the provided `all` cmake run configuration to compile the project
5. A good build log will contain at least the following
    1. `Running cpp protocol buffer compiler on ...`
    2. `Built target proto`
    3. `Built target mediocre`