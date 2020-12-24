FROM go-gtk3

COPY main.go /go/src/github.com/test/main.go

WORKDIR /go/src/github.com/test

CMD [ "go", "run", "main.go" ]