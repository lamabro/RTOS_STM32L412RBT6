# RTOSP1

RTOSP1 is an STM32 real-time embedded project that imitates the principles of a TTA-style protocol used in industrial safety systems. It explores deterministic master/slave communication, device discovery, status supervision, GPIO commands, and fault-aware protocol behavior on an STM32L412RB target.

This repository is an engineering and learning reference implementation. It is not a certified safety product and must not be treated as a replacement for the requirements, verification, validation, or certification activities of a production safety system.

#Note.  Target is that
all individual nodes will send the data at their specific slots not all at same time
build simplex communications meaning device can send or read only at a time instead of both.

Each at its own slot and cycle
0ms.      none send and everyone listening
50ms.    Only master send the data and all other nodes listen

100ms.    Only Node 1 send , all other devices listen to it.

150ms.    ONly Node 2 send , all other devices listen to it.

Each nodes will run a cycle of 150ms to get its time to send.

Eg Master  cycle to send is 50ms---200ms---350--ms

Eg Node 1.   100ms--250ms--400ms
Eg Node 2.   150ms--300ms--450ms---


## Project Goals

- Implement a modular TTA-inspired communication protocol for an embedded target.
- Separate protocol logic from STM32 HAL, FreeRTOS, and hardware-specific code.
- Provide master and slave finite-state machines with scheduler support.
- Build a foundation for deterministic, maintainable real-time communication.
- Make protocol behavior testable on a host machine before deployment to hardware.
- Keep development, static analysis, testing, and documentation reproducible through automation.

## Architecture

The project is organized into clear layers:

- `Core/`: STM32 application, interrupt, FreeRTOS, and HAL integration code.
- `TTA_Protocol/Common/`: frame construction, parsing, CRC, definitions, and shared protocol types.
- `TTA_Protocol/Master/`: master state machine, database, scheduler, and task integration.
- `TTA_Protocol/Slave/`: slave state machine, GPIO logic, scheduler, and task integration.
- `TTA_Protocol/Tasks/`: RTOS-facing master and slave task organization.
- `tests/`: native host unit tests and the Unity test framework.
- `cmake/`: CMake toolchain and STM32CubeMX build integration.

The intended data path is:

```text
Application or RTOS task
        -> master/slave state machine
        -> scheduler and protocol logic
        -> frame builder/parser and CRC
        -> HAL transport
```

The protocol modules are designed to remain independent of the STM32 HAL, FreeRTOS, and physical transport wherever practical.

## Development Environment

The supported development environment is the Docker image defined in `Dockerfile`. It provides:

- Ubuntu 22.04
- CMake and Ninja
- ARM GNU `arm-none-eabi` toolchain
- GCC/G++ for native unit tests
- STM32/OpenOCD development tools
- Doxygen and Graphviz for documentation
- Cppcheck for static analysis
- Python tooling, including Lizard for complexity reporting

Build the image with:

```bash
docker build -t rtosp1-dev .
```

The CMake configuration is container-oriented. Running the commands below inside the development container keeps the toolchain and analysis versions consistent with CI.

## Building

Build the STM32 application in Debug mode:

```bash
cmake --preset=Debug-Application
ninja -C build/Target/application/Debug
```

Build the STM32 application in Release mode:

```bash
cmake --preset=Release-Application
ninja -C build/Target/application/Release
```

Build the bootloader configurations when bootloader sources are enabled:

```bash
cmake --preset=Debug-Bootloader
ninja -C build/Target/bootloader/Debug

cmake --preset=Release-Bootloader
ninja -C build/Target/bootloader/Release
```

The application build generates the ELF output and post-build binary and Intel HEX images.

For a convenience build of Debug and Release application targets, use:

```bash
./buildall.sh
```

## Unit Testing

Unit tests are compiled natively with GCC, without the embedded ARM toolchain or STM32 hardware. This makes protocol and state-machine behavior quick to validate during development.

Build the unit-test target:

```bash
cmake --preset=Debug-UnitTest
ninja -C build/Target/application/unit_test
```

Run the tests through CTest:

```bash
ctest --test-dir build/Target/application/unit_test --output-on-failure
```

The current test setup uses Unity and includes coverage for the master finite-state-machine behavior. Additional protocol, parser, CRC, scheduler, and slave tests can be added under `tests/` as the implementation grows.

## CI/CD

GitHub Actions is defined in `.github/workflows/ci.yml`. The pipeline runs on pushes and pull requests targeting `main` or `develop`, and can also be started manually.

The automated matrix currently:

- Builds the application in Debug mode.
- Builds the application in Release mode.
- Builds and runs the native unit tests with CTest.
- Runs Cppcheck over the application and protocol sources.
- Executes all of these checks inside the project Docker image.
- Fails the workflow when a build, test, or static-analysis step fails.

See [GITHUB_ACTIONS_SETUP.md](GITHUB_ACTIONS_SETUP.md) for branch-protection and required-status-check guidance.

## Static Analysis

Run the repository's Cppcheck task from the configured development environment:

```bash
cppcheck --addon=misra \
         --enable=all \
         --inconclusive \
         --std=c11 \
         --language=c \
         --force \
         --inline-suppr \
         --suppress=missingIncludeSystem \
         --suppress=misra-config \
         --error-exitcode=1 \
         -I Core/Inc \
         -I Drivers/CMSIS/Device/ST/STM32L4xx/Include \
         -I Drivers/CMSIS/Include \
         -I Middlewares/Third_Party/FreeRTOS/include \
         Core/Src
```

The CI workflow also checks the protocol sources under `TTA_Protocol/`.

## Documentation

Doxygen configuration is provided in `Doxyfile`. Generate API documentation from a configured CMake build with:

```bash
cmake --build build/Target/application/Debug --target docs
```

When Lizard is installed, the `complexity` and `docs-full` targets can also generate a code-complexity report alongside the documentation.

Protocol design notes and the current frame-flow description are available in:

- [TTA protocol README](TTA_Protocol/README.md)
- [Protocol instructions](TTA_Protocol/PROTOCOL_INSTRUCTIONS.md)

## Repository Status

The repository is actively structured around the protocol, RTOS task integration, host-side testing, static analysis, and automated CI/CD foundations. Protocol details and safety-related behavior should be reviewed and extended alongside the test suite before any use beyond development or research.
