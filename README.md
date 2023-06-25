Library under development, everything here is subject to change.

# Mediocre

Mediocre, or ***M**ediocre **E**xtraction of **D**ata **I**nvolving an **O**ptical **C**haracter **R**ecognition **E**ngine*, is a configurable application for extracting and emitting data in real-time from videos. 

## Setup in CLion

1. Run [pull-base-and-tag-local.ps1](pull-base-and-tag-local.ps1) to pull and retag the latest base image
2. Go to `File > Settings > Build, Execution, Deployment` and add a `Docker` toolchain
   1. For image use `mediocre:base-dev-local`
   2. In container settings add a port binding for `50051`
   3. For CMake use `\local\cmake\bin\cmake`
3. Go to the `CMake` tab in the toolbar (usually pinned to the bottom of CLion) and reload the project
4. Run the provided `all` cmake run configuration to compile the project
5. A good build log will contain at least the following
   1. `Running cpp protocol buffer compiler on ...`
   2. `Built target proto-objects`
   3. `Built target mediocre`