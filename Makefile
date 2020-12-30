
$(eval GO_GTK3_EXAMPLES := $(shell echo 'go-gtk3-examples'));
$(eval CONTAINER_ID_EXAMPLES := $(shell podman ps -a | grep $(GO_GTK3_EXAMPLES) | awk '{ print $$1 }'))


examples:
ifeq ($(CONTAINER_ID_EXAMPLES),)
	podman build -f go-gtk3.dockerfile -t go-gtk3 .
	podman build -f go-gtk3-examples.dockerfile -t $(GO_GTK3_EXAMPLES) .
	podman run -it --rm \
		--name $(GO_GTK3_EXAMPLES) \
		-e DISPLAY \
		-v /tmp/.X11-unix:/tmp/.X11-unix:ro \
		--device /dev/dri \
		--device /dev/snd \
		--ipc=host \
		--userns keep-id \
		-u ${USER} \
		$(GO_GTK3_EXAMPLES)
else
	podman start -ia $(CONTAINER_ID_EXAMPLES)
endif

$(eval GO_GTK3_CANVAS_EXAMPLE := $(shell echo 'go-gtk3-examples-canvas'));
$(eval CONTAINER_ID_CANVAS_EXAMPLE := $(shell podman ps -a | grep $(GO_GTK3_CANVAS_EXAMPLE) | awk '{ print $$1 }'))

canvas:
ifeq ($(CONTAINER_ID_CANVAS_EXAMPLE),)
	podman build -f go-gtk3.dockerfile -t go-gtk3 .
	podman build -f go-gtk3.example-canvas.dockerfile -t $(GO_GTK3_CANVAS_EXAMPLE) .
	podman run -it --rm \
		--name $(GO_GTK3_CANVAS_EXAMPLE) \
		-e DISPLAY \
		-v /tmp/.X11-unix:/tmp/.X11-unix:ro \
		--device /dev/dri \
		--device /dev/snd \
		--ipc=host \
		--userns keep-id \
		-u ${USER} \
		$(GO_GTK3_CANVAS_EXAMPLE)
else
	podman start -ia $(CONTAINER_ID_CANVAS_EXAMPLE)
endif