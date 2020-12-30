FROM go-gtk3

USER rianby64

RUN mkdir -p $HOME/example-canvas

WORKDIR $HOME/example-canvas

COPY src/example-canvas.c ./

RUN gcc -I /usr/include/goocanvas-2.0/ $(pkg-config goocanvas-2.0 --libs --cflags) example-canvas.c -o example-canvas

CMD [ "./example-canvas" ]