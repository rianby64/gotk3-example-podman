FROM rianby64/vscode AS go-gtk3

USER rianby64

ENV GOPATH=$HOME/go
ENV GOBIN=$GOPATH/bin

RUN mkdir -p $GOPATH

USER root

RUN yum install -y \
    golang.x86_64 \
    glib2-devel.x86_64 \
    goocanvas2-devel.x86_64 \
    cairo-gobject-devel.x86_64 \
    pango-devel.x86_64 \
    gtk3-devel.x86_64

USER rianby64

RUN go get github.com/gotk3/gotk3/gtk

CMD ["bash"]
