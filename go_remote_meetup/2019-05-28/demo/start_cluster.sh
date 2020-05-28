#!/bin/bash

set -e

cleanup () {
    echo "Tearing down services"
    docker-compose -f docker-compose.dind.yaml down
}
trap cleanup EXIT

is_docker_healthy() {
    address="$1"

    status=$(curl -s -o /dev/null -w "%{http_code}" http://${address}/version || true)
    if [ "$status" = "200" ]; then
        return 0
    fi

    return 1
}

echo "Starting DinD containers"

docker-compose -f docker-compose.dind.yaml up -d

echo "Waiting for DinD containers to start..."

dind_docker_ports=$(docker-compose -f docker-compose.dind.yaml ps | sed -n 's/.*0.0.0.0:\(.*\)->2375\/tcp.*/\1/p')
while true; do
    healthy="true"

    for port in ${dind_docker_ports}; do
        if ! is_docker_healthy "localhost:${port}"; then
            echo "Failed Docker API health check for port: ${port}"
            healthy="false"
        fi
    done

    if [ "${healthy}" = "true" ]; then
        break
    fi

    sleep 1;
done

echo "DinD containers up and running:"
docker-compose -f docker-compose.dind.yaml ps

echo "Press Ctrl+C to exit"
read -r -d '' _ </dev/tty