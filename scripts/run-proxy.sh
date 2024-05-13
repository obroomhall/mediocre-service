#!/bin/bash

command=(
  "grpcwebproxy"
  "--backend_addr=0.0.0.0:8081"
)

if [[ -f $TLS_CERT_PATH && -f $TLS_KEY_PATH ]]; then
  command+=(
    "--server_tls_cert_file=$TLS_CERT_PATH"
    "--server_tls_key_file=$TLS_KEY_PATH"
  )
  ALLOWED_ORIGINS=${ALLOWED_ORIGINS:-http://localhost:5173,https://localhost:5173}
else
  command+=(
    "--run_tls_server=false"
  )
  ALLOWED_ORIGINS=${ALLOWED_ORIGINS:-http://localhost:5173}
fi

if [ "$ALLOWED_ORIGINS" = "*" ]; then
  command+=(
    "--allow_all_origins"
  )
else
  command+=(
    "--allowed_origins=$ALLOWED_ORIGINS"
  )
fi

echo "> ${command[@]}"
"${command[@]}"