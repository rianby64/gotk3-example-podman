FROM go-gtk3

RUN git clone https://github.com/gotk3/gotk3-examples.git /go/src/github.com/gotk3/gotk3-examples

WORKDIR /go/src/github.com/gotk3/gotk3-examples/gtk-examples

CMD [ "bash" ]