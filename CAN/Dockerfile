FROM arm64v8/debian:bullseye-slim

# Install build dependencies
RUN apt-get update && apt-get install -y \
    gcc \
    libc6-dev \
    make \
    git \
    && rm -rf /var/lib/apt/lists/*

# Copy source files
WORKDIR /app
COPY can_transmitter.c .

# Compile the application
RUN gcc can_transmitter.c -o can_transmitter

# Install runtime dependencies
RUN apt-get update && apt-get install -y \
    can-utils \
    && rm -rf /var/lib/apt/lists/*

# Set entrypoint
ENTRYPOINT ["/app/can_transmitter"]