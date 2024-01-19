# Data engineering at scale: Real-world case studies with Benthos

## Setup

Install [`docker`](https://www.docker.com), [`jless`](https://jless.io), [`pgcli`](https://www.pgcli.com) and [`benthos`](https://www.benthos.dev/).

## Demo

```shell
> docker run --rm -d --name kafka -p 9092:9092 docker.vectorized.io/vectorized/redpanda start --smp 1 --overprovisioned --kafka-addr 0.0.0.0:9092 --advertise-kafka-addr localhost:9092
```

```shell
> docker run --rm -d --name postgres -p 5432:5432 -e POSTGRES_USER=testuser -e POSTGRES_PASSWORD=testpass -e POSTGRES_DB=testdb postgres
```

```shell
> benthos -c users_api.yaml
> benthos -c db_writer.yaml
```

```shell
> benthos -c generate_data.yaml
> curl -s http://localhost:4195/api?department=sales | jless
```

```shell
> pgcli postgres://testuser:testpass@localhost:5432/testdb?sslmode=disable
> SELECT * FROM users;
```

```shell
> curl http://localhost:4195/metrics | jless
> curl http://localhost:4196/metrics | jless
```
