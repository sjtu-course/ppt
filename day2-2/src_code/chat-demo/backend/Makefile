GOCMD=GO111MODULE=on go

all: test build

build: clean
	mkdir -p target/log
	cp -r conf target
	$(GOCMD) build -o target/demo cmd/main.go

prod: build
	nohup target/demo -mode=prod -conf_dir=target/conf >/dev/null 2>&1 &

dev: build
	nohup target/demo -mode=dev -conf_dir=target/conf >/dev/null 2>&1 &

stop:
	pkill -f target/demo

clean:
	rm -rf target/

test:
	$(GOCMD) test -v ./...


