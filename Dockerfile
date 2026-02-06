# 2DNGE - 2D Game Engine Development Container
# Based on stack: C++20, SDL2, Python3, Dear ImGui, ENet, GLM, CMake

FROM ubuntu:22.04

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install build essentials and C++20 toolchain
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    git \
    pkg-config \
    g++-12 \
    gcc-12 \
    gdb \
    && update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 100 \
    && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-12 100 \
    && rm -rf /var/lib/apt/lists/*

# Install SDL2 and related libraries
RUN apt-get update && apt-get install -y \
    libsdl2-dev \
    libsdl2-image-dev \
    libsdl2-ttf-dev \
    libsdl2-mixer-dev \
    && rm -rf /var/lib/apt/lists/*

# Install Python3 for scripting
RUN apt-get update && apt-get install -y \
    python3 \
    python3-dev \
    python3-pip \
    && rm -rf /var/lib/apt/lists/*

# Install ENet for networking
RUN apt-get update && apt-get install -y \
    libenet-dev \
    && rm -rf /var/lib/apt/lists/*

# Install GLM (header-only math library)
RUN apt-get update && apt-get install -y \
    libglm-dev \
    && rm -rf /var/lib/apt/lists/*

# Install Dear ImGui dependencies (OpenGL)
RUN apt-get update && apt-get install -y \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libx11-dev \
    libxrandr-dev \
    libxi-dev \
    && rm -rf /var/lib/apt/lists/*

# Install testing frameworks
RUN apt-get update && apt-get install -y \
    catch2 \
    libgtest-dev \
    && rm -rf /var/lib/apt/lists/*

# Build Google Test from source (needed for linking)
RUN cd /usr/src/gtest && cmake . && make && cp lib/*.a /usr/lib/

# Set working directory
WORKDIR /app

# Copy project files
COPY . .

# Create build directory
RUN mkdir -p build

# Default command - configure and build
CMD ["bash", "-c", "cd build && cmake .. -G Ninja && ninja"]
