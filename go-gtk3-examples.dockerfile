FROM go-gtk3

WORKDIR $GOPATH/src/github.com

RUN git clone https://github.com/gotk3/gotk3-examples.git $GOPATH/src/github.com/gotk3-examples

WORKDIR $GOPATH/src/github.com/gotk3-examples/gtk-examples

CMD [ "bash" ]