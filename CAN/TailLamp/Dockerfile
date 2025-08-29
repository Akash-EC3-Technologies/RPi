# Stage 1: Builder
FROM arm64v8/debian:bullseye-slim AS builder

# Install build tools
RUN apt-get update && apt-get install -y \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy source files
COPY . .

# Build the application with mock HAL for testing
RUN make build_test

# Stage 2: Runtime

FROM arm64v8/debian:bullseye-slim

WORKDIR /app

# Copy only the built binary from builder stage
COPY --from=builder /app/tail_lamp /app/tail_lamp

# Ensure the binary is executable
RUN chmod +x /app/tail_lamp

ENTRYPOINT ["/app/tail_lamp"]