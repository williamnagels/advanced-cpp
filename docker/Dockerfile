FROM alpine:3.23
RUN apk add --no-cache \
    make \
    g++ \
    cmake \
    clang \
    benchmark-dev \
    ninja-build \
    && ln -sf /usr/lib/ninja-build/bin/ninja /usr/bin/ninja