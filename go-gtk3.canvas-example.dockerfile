FROM go-gtk3

USER rianby64

RUN mkdir -p $HOME/example-canvas

WORKDIR $HOME/example-canvas

RUN code --install-extension ms-vscode.cpptools
RUN code --install-extension ms-vscode.cpptools-themes

#COPY src/example-canvas.c ./

#RUN gcc $(pkg-config goocanvas-2.0 --libs --cflags) example-canvas.c -o example-canvas

#CMD [ "./example-canvas" ]

USER root

CMD [ "bash" ]