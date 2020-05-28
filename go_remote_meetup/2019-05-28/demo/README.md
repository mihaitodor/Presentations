# Sidecar cluster demo

Disclaimer: This was only tested on OSX with Docker for Mac. It should work on
Linux as well but I haven't checked.

1. Start the DinD containers, which will represent our cluster nodes
```shell
> ./start_cluster.sh
```

2. Start infrastructure and backend services on the DinD containers
```shell
> ./start_services.sh
```

3. Test that Sidecar and Envoy UI are up and running and accessible on the
ingress host
```
Sidecar: http://localhost:7777
Envoy UI: http://localhost:7778
```

4. Check that the WhoAmI TCP and HTTP services are accessible on ports 8888 and
10000 on the ingress host:
```shell
> curl localhost:8888
> curl -H "Host: whoami.one.local" localhost:10000
> curl -H "Host: whoami.two.local" localhost:10000
```

5. Launch another service `whoami-http-demo` on backend2:
```shell
> docker exec -it backend2 sh
> docker run -d --rm -p80 --name whoami-http-demo -l ServiceName=whoami-http-demo -l ServicePort_80=10003 -l HealthCheck="HttpGet" -l HealthCheckArgs="http://{{ host }}:{{ tcp 10003 }}/health" containous/whoami:v1.5.0
> curl dev.local:10003
> exit
> # Uncomment `whoami-http-demo` section under the http_listener virtual_hosts
> # in envoy/ingress.yaml
> docker exec ingress docker restart envoy
> curl -H "Host: whoami.demo.local" localhost:10000
```