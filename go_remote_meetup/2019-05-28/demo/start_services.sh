#!/bin/bash

set -e

die() {
	echo $1
	exit 1
}

is_sidecar_healthy() {
    address="${1}"

    status=$(curl -s -m 3 -o /dev/null -w "%{http_code}" http://${address}/api/services.json || true)
    if [ "${status}" = "200" ]; then
        return 0
    fi

    return 1
}

run_docker_compose() {
    task="${1}"
    container_colon_port="${2}"
    seeds="${3}" # Optional

    container="${container_colon_port%:*}"
    port="${container_colon_port#*:}"
    container_ip="$(docker inspect -f "{{ range .NetworkSettings.Networks }}{{ .IPAddress }}{{ end }}" "${container}")"

    node_type="backend"
    [ "${container}" = "ingress" ] && node_type="ingress"

    # Funny enough `docker-compose pull` can pull images in parallel, while
    # `docker-compose up` can't. It's faster to call them one after the other
    SIDECAR_SEEDS="${seeds}" HOSTNAME="${container}" ENVOY_CONFIG="${node_type}.yaml" DOCKER_HOST="tcp://localhost:${port}" ADVERTISE_IP="${container_ip}" docker-compose -f "docker-compose.${task}.yaml" -p "${task}" pull
    SIDECAR_SEEDS="${seeds}" HOSTNAME="${container}" ENVOY_CONFIG="${node_type}.yaml" DOCKER_HOST="tcp://localhost:${port}" ADVERTISE_IP="${container_ip}" docker-compose -f "docker-compose.${task}.yaml" -p "${task}" up -d
}
# Need to export this to use it via xargs...
export -f run_docker_compose

dind_ingress_containers_colon_ports=$(docker-compose -f docker-compose.dind.yaml ps | grep ingress | sed -n 's/\(^[[:alnum:]]*\).*:\(.*\)->2375\/tcp.*/\1:\2/p')
dind_backend_containers_colon_ports=$(docker-compose -f docker-compose.dind.yaml ps | grep backend | sed -n 's/\(^[[:alnum:]]*\).*:\(.*\)->2375\/tcp.*/\1:\2/p')
dind_sidecar_ports=$(docker-compose -f docker-compose.dind.yaml ps | sed -n 's/.*:\(.*\)->7777\/tcp.*/\1/p')
dind_backend_sidecar_ports=$(docker-compose -f docker-compose.dind.yaml ps | grep backend | sed -n 's/.*:\(.*\)->7777\/tcp.*/\1/p')

[ -n "${dind_ingress_containers_colon_ports}" ] && [ -n "${dind_backend_containers_colon_ports}" ] && [ -n "${dind_backend_sidecar_ports}" ] || die "Please start cluster first"

# Use all the backends as Sidecar seeds. This can be slow if there are many of
# them, in which case we can use a subset
backend_hosts="$(echo "${dind_backend_containers_colon_ports}" | awk -F':' '{print $1}' | paste -s -d, -)"

# Avoid split-brain issues by launching Sidecar on one of the backend nodes first
echo "Starting infrastructure for the first Sidecar seed backend node"

run_docker_compose "infrastructure" "$(echo "${dind_backend_containers_colon_ports}" | head -1)" "${backend_hosts}"

# Make sure Sidecar is alive on the seed node before proceeding
while ! is_sidecar_healthy "localhost:$(echo "${dind_backend_sidecar_ports}" | head -1)"; do sleep 1; done

echo "Starting infrastructure for the rest of the backend nodes"

# Start the rest of the backend Sidecars
# xargs is magical: We execute run_docker_compose via a new shell which takes
# "sh" as $0 to respect conventions, "{}" as $2 and "${backend_hosts}" as $3
echo "${dind_backend_containers_colon_ports}" | sed 1d | xargs -P4 -I {} sh -c 'run_docker_compose "$@"' "sh" "infrastructure" "{}" "${backend_hosts}"

echo "Starting infrastructure on the ingress nodes"

echo "${dind_ingress_containers_colon_ports}" | xargs -P4 -I {} sh -c 'run_docker_compose "$@"' "sh" "infrastructure" "{}" "${backend_hosts}"

# Make sure Sidecar is alive on all of the nodes, including ingress
while true; do
    healthy="true"

    for port in ${dind_sidecar_ports}; do
        if ! is_sidecar_healthy "localhost:${port}"; then
            echo "Failed Sidecar health check for port: ${port}"
            healthy="false"
        fi
    done

    if [ "${healthy}" = "true" ]; then
        break
    fi

    sleep 1;
done

echo "Starting services on the backend nodes"

echo "${dind_backend_containers_colon_ports}" | xargs -P4 -I {} sh -c 'run_docker_compose "$@"' "sh" "services" "{}" "${backend_hosts}"

echo "Infrastructure services for the nodes up and running:"
docker-compose -f docker-compose.dind.yaml ps | sed -n 's/\(^[[:alnum:]]*\).*:\(.*\)->7777\/tcp.*:\(.*\)->7778\/tcp.*:\(.*\)->9901\/tcp.*/\1 | Sidecar: http:\/\/localhost:\2 | Envoy UI: http:\/\/localhost:\3 | Envoy Admin: http:\/\/localhost:\4/p'