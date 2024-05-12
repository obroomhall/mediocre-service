FROM debian:bookworm as builder

RUN --mount=type=cache,sharing=locked,target=/var/cache/apt \
    apt-get update \
    && apt-get install -y wget unzip

# install grpc web proxy
ARG GRPC_WEB_PROXY_VERSION=0.15.0
RUN mkdir -p /local/grpcwebproxy && cd /local/grpcwebproxy \
    && mkdir ./download && mkdir ./install && mkdir ./bin \
    && wget -q -O ./download/source.zip https://github.com/improbable-eng/grpc-web/releases/download/v$GRPC_WEB_PROXY_VERSION/grpcwebproxy-v$GRPC_WEB_PROXY_VERSION\-linux-x86_64.zip \
    && unzip ./download/source.zip -d ./install \
    && cp ./install/dist/grpcwebproxy-v$GRPC_WEB_PROXY_VERSION\-linux-x86_64 ./bin/grpcwebproxy


FROM debian:bookworm

RUN --mount=type=cache,sharing=locked,target=/var/cache/apt \
    apt-get update \
    && apt-get install -y ca-certificates

COPY --from=builder /local/grpcwebproxy/bin/grpcwebproxy /usr/local/bin/

RUN chmod +x /usr/local/bin/grpcwebproxy

ENTRYPOINT grpcwebproxy \
    --server_tls_cert_file=${TLS_CERT_PATH:-/certificates/cert.pem} \
    --server_tls_key_file=${TLS_KEY_PATH:-/certificates/key.pem} \
    --backend_addr=$GRPC_SERVER_ADDRESS \
    --allowed_origins=$ALLOWED_ORIGINS