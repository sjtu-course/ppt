# building stage
FROM golang:1.15 AS builder
WORKDIR /build
COPY . .
ENV GOPROXY https://goproxy.cn
RUN go mod download && \
    mkdir -p /target/log && \
    cp -r conf /target && \
    CGO_ENABLED=0 GO111MODULE=on go build -o /target/demo cmd/main.go

# running stage
FROM alpine:latest AS runner
COPY --from=builder /target /home/runner/target
RUN adduser -u 10001 -D runner && \
    mkdir -p /home/runner/target/log && \
    chmod o+rw /home/runner/target/log && \
    apk update && \
    apk add --no-cache curl
USER runner
WORKDIR /home/runner
EXPOSE 3001
CMD ./target/demo -mode=prod -conf_dir=./target/conf