FROM golang:1.24.0 AS builder
WORKDIR /app
COPY . .

RUN go env -w CGO_ENABLED=1 && \
    go env -w GO111MODULE=on && \
    go env -w CGO_LDFLAGS="-L./bin/library/libtokenizers" && \
    go env -w GOPROXY=https://goproxy.cn,https://mirrors.aliyun.com/goproxy,direct

ARG VERSION=v1.7.0
RUN go build -ldflags="-s -w -X 'main.SoftwareVer=$VERSION'" -o code-completion *.go
RUN chmod 755 code-completion

FROM frolvlad/alpine-glibc:alpine-3.21_glibc-2.41 AS runtime
RUN apk add --no-cache libstdc++

ENV env prod
ENV TZ Asia/Shanghai
WORKDIR /app
COPY --from=builder /app/code-completion /app/code-completion
COPY --from=builder /app/bin/cgtok/starcoder_tokenizer.json /app/bin/cgtok/starcoder_tokenizer.json

ENTRYPOINT ["/app/code-completion"]
