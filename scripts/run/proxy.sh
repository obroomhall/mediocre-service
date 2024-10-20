#!/bin/bash

command=(
  "grpcwebproxy"
  "--backend_addr=0.0.0.0:8081"
  "--backend_max_call_recv_msg_size=10485760" # 10MB
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

if [[ -f $TLS_CA_PATH ]]; then
  command+=(
    "--server_tls_client_ca_files=$TLS_CA_PATH"
    "--server_tls_client_cert_verification=require"
  )
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

if [ -n "$MAX_BACKOFF" ]; then
  command+=(
    "--backend_backoff_max_delay=$MAX_BACKOFF"
  )
fi

echo "> ${command[@]}"
"${command[@]}"