# Multi-stage Dockerfile for RTOSP1 - Container-Only Development Environment
# 
# This container provides a complete isolated development environment
# with all required tools for embedded ARM development.
#
# Build: docker build -t rtosp1-dev:latest .
# Run:   docker-compose run --rm rtosp1-dev bash

FROM ubuntu:22.04

# Set non-interactive mode to avoid prompts during build
ENV DEBIAN_FRONTEND=noninteractive \
    LANG=C.UTF-8 \
    LC_ALL=C.UTF-8

# ==============================================================================
# Step 1: Install system packages and core dependencies
# ==============================================================================
RUN apt-get update && apt-get install -y --no-install-recommends \
    wget \
    curl \
    ca-certificates \
    xz-utils \
    build-essential \
    git \
    cmake \
    ninja-build \
    python3 \
    python3-pip \
    python3-dev \
    python3-venv \

    # Documentation tools
    doxygen \
    graphviz \
    # Static analysis
    cppcheck \
    
    # Debugging tools
    gdb \
    # Additional tools
    pkg-config \
    make \
    autoconf \
    automake \
    libtool \
    texinfo \
    bison \
    flex \
    libffi-dev \
    libssl-dev \
    # USB support for debuggers
    libusb-1.0-0 \
    libusb-1.0-0-dev \
    openocd \
    # Cleanup to reduce image size
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

# ==============================================================================
# Step 2: Install ARM GNU Toolchain (arm-none-eabi)
# ==============================================================================
# Version: 14.3.1 (matches your macOS installation)
# Location: /opt/arm-toolchain/gcc (container standard path)
#
# This is the toolchain for compiling C code for ARM Cortex-M4 (STM32L4)

#apt-get update
#apt-get install -y software-properties-common
#add-apt-repository ppa:deadsnakes/ppa
#apt-get update
#apt-get install -y python3.8 python3.8-dev


# Downloads from xpack-dev-tools (same as Homebrew uses)

# INSTALL OPEN OCD TOO FOR permanent image

RUN set -e && \
    echo "Installing ARM GNU Toolchain..." && \
    mkdir -p /opt/arm-toolchain && \
    cd /opt/arm-toolchain && \
    curl -L -o arm-gcc.tar.xz \
    #arm-gnu-toolchain-14.3.rel1-aarch64-arm-none-eabi.tar.xz
    #Please USE latest FILE above

     https://developer.arm.com/-/media/Files/downloads/gnu/14.3.rel1/binrel/arm-gnu-toolchain-14.3.rel1-aarch64-arm-none-eabi.tar.xz && \
    tar -xf arm-gcc.tar.xz && \
    rm arm-gcc.tar.xz && \
    mv arm-gnu-toolchain-14.3* gcc && \
    /opt/arm-toolchain/gcc/bin/arm-none-eabi-gcc --version


# Ninja build system (in standard /usr/bin path, already set by apt)
ENV NINJA_BIN=/usr/bin/ninja

# CMake
ENV CMAKE_BIN=/usr/bin/cmake

# Python 3
ENV PYTHON_BIN=/usr/bin/python3

# Git
ENV GIT_BIN=/usr/bin/git

# Doxygen
ENV DOXYGEN_BIN=/usr/bin/doxygen

# cppcheck
ENV CPPCHECK_BIN=/usr/bin/cppcheck

# GDB
ENV GDB_BIN=/usr/bin/gdb

# Working directory inside container
WORKDIR /kcepdx_repo

###############################################################################
# Create non-root user for build operations
###############################################################################
RUN useradd -m -s /bin/bash builder && \
    mkdir -p /home/builder/.ssh && \
    chmod 700 /home/builder/.ssh

###############################################################################
# ⭐ 3. SWITCH to builder user and create the venv
###############################################################################
USER builder

# Create Python virtual environment for project tools____                                SHould we remove thi?
RUN python3 -m venv /home/builder/.suresh

# Install Python requirements from requirements.txt 

#install inside venv.



RUN python3 -m venv /home/builder/.suresh && \
    /home/builder/.suresh/bin/pip install --no-cache-dir --upgrade pip setuptools wheel && \
    /home/builder/.suresh/bin/pip install --no-cache-dir \
        GitPython==3.1.27 \
        pylink-square==1.2.0 \
        ninja==1.11.1.1 \
        pytest==8.3.3 \
        pytest-html==4.1.1 \
        pyserial==3.5 \
        pyusb==1.3.0 \
        "pyads>=3.5.0" \
        "matplotlib>=3.9.4" \
        pydantic==2.9.2 \
        flake8==7.0.0 \
        black==24.1.1 \
        isort==5.13.2 \
        click==8.1.7 \
        tqdm==4.66.2 && \
    /home/builder/.suresh/bin/pip install lizard && \
    echo "✓ All Python packages installed"








# ==============================================================================
# Step 5: Set up container environment variables
# ==============================================================================
# These paths are used by CMakeLists.txt for container builds
ENV CONTAINER_TOOLCHAIN_PATH=/opt/arm-toolchain/gcc
ENV CONTAINER_TOOLS_PATH=/usr/bin
ENV CONTAINER_PYTHON_PATH=/usr/bin/python3
#ENV PATH="/opt/arm-toolchain/gcc/bin:/home/builder/.suresh/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"
ENV  PATH="/opt/arm-toolchain/gcc/bin:/home/builder/.suresh/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"
ENV RUNNING_IN_CONTAINER=1
ENV CCACHE_DIR=/root/.ccache


    #PATH FOR OPENOCD TO BE ADDED LATER

  

# ==============================================================================
# Step 6: Verify all tools installed correctly
# ==============================================================================




# ==============================================================================
# Step 7: Create workspace directory
# ==============================================================================
# This is where your project files are mounted via docker-compose.yml
WORKDIR /RTOSP1

# ==============================================================================
# Step 8: Default command
# ==============================================================================
# When container starts, drop into bash shell
CMD ["/bin/bash"]