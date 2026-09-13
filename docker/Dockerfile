# syntax=docker/dockerfile:1
FROM alpine:3.23 AS marp-installer

ENV NODE_USE_SYSTEM_CA=1
ENV PUPPETEER_SKIP_DOWNLOAD=true

RUN apk add --no-cache \
    ca-certificates \
    nodejs \
    npm

RUN --mount=type=secret,id=local_ca,required=false \
    if [ -f /run/secrets/local_ca ]; then \
        cp /run/secrets/local_ca /usr/local/share/ca-certificates/local-ca.crt; \
        update-ca-certificates; \
    fi \
    && npm install --global --prefix /marp @marp-team/marp-cli@4.2.3 \
    && rm -f /usr/local/share/ca-certificates/local-ca.crt \
    && update-ca-certificates

FROM alpine:3.23

RUN apk add --no-cache \
    benchmark-dev \
    clang \
    cmake \
    chromium-headless-shell \
    font-liberation \
    g++ \
    make \
    ninja-build \
    nodejs \
    && ln -sf /usr/lib/ninja-build/bin/ninja /usr/bin/ninja

COPY --from=marp-installer /marp/ /usr/local/

ENV NODE_USE_SYSTEM_CA=1
ENV PUPPETEER_SKIP_DOWNLOAD=true
ENV PUPPETEER_EXECUTABLE_PATH=/usr/bin/chromium-headless-shell
ENV CHROME_PATH=/usr/bin/chromium-headless-shell
ENV CHROME_NO_SANDBOX=true