#!/bin/bash

echo "Starting mediocre"
mediocre &

TLS_CERT_PATH=${TLS_CERT_PATH:-/certificates/cert.pem}
TLS_KEY_PATH=${TLS_KEY_PATH:-/certificates/key.pem}
if [[ -f $TLS_CERT_PATH && -f $TLS_KEY_PATH ]]; then

  if [ ! -z "$ALLOWED_ORIGINS" ]; then
    echo "Using allowed origins: $ALLOWED_ORIGINS"
  else
    ALLOWED_ORIGINS=https://localhost:5173
    echo "No allowed origins set, falling back to $ALLOWED_ORIGINS"
  fi

  echo "Starting proxy with TLS"
  grpcwebproxy \
      --server_tls_cert_file=$TLS_CERT_PATH \
      --server_tls_key_file=$TLS_KEY_PATH \
      --allowed_origins=$ALLOWED_ORIGINS \
      --backend_addr=0.0.0.0:8081 \
      --enable_health_check_service \
      --enable_health_endpoint

else

  if [ ! -z "$ALLOWED_ORIGINS" ]; then
    echo "Using allowed origins: $ALLOWED_ORIGINS"
  else
    ALLOWED_ORIGINS=http://localhost:5173
    echo "No allowed origins set, falling back to $ALLOWED_ORIGINS"
  fi

  echo "No certificates found, starting proxy without TLS"
  grpcwebproxy \
      --run_tls_server=false \
      --allowed_origins=ALLOWED_ORIGINS \
      --backend_addr=0.0.0.0:8081 \
      --enable_health_check_service \
      --enable_health_endpoint

fi
