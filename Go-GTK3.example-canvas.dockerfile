FROM go-gtk3

RUN yum install -y goocanvas2-devel.x86_64

RUN mkdir example-canvas

WORKDIR /example-canvas

COPY example-canvas.c ./

RUN gcc -I /usr/include/goocanvas-2.0/ $(pkg-config goocanvas-2.0 --libs --cflags) example-canvas.c -o example-canvas

CMD [ "./example-canvas" ]