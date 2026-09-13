# syntax=docker/dockerfile:1
FROM alpine:3.23
RUN apk add --no-cache \
    make \
    g++ \
    cmake \
    clang \
    benchmark-dev \
    chromium \
    font-noto \
    nodejs \
    npm \
    ninja-build \
    && ln -sf /usr/lib/ninja-build/bin/ninja /usr/bin/ninja

ENV NODE_USE_SYSTEM_CA=1
ENV PUPPETEER_SKIP_DOWNLOAD=true
ENV CHROME_PATH=/usr/bin/chromium
ENV CHROME_NO_SANDBOX=true

RUN --mount=type=secret,id=local_ca,required=false \
    if [ -f /run/secrets/local_ca ]; then \
        cp /run/secrets/local_ca /usr/local/share/ca-certificates/local-ca.crt; \
        update-ca-certificates; \
    fi \
    && npm install --global @marp-team/marp-cli@4.2.3 \
    && rm -f /usr/local/share/ca-certificates/local-ca.crt \
    && update-ca-certificates