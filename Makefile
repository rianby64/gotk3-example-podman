
$(eval GO_GTK3_EXAMPLES := $(shell echo 'go-gtk3-examples'));
$(eval CONTAINER_ID_EXAMPLES := $(shell podman ps -a | grep $(GO_GTK3_EXAMPLES) | awk '{ print $$1 }'))


examples:
ifeq ($(CONTAINER_ID_EXAMPLES),)
	podman build -f go-gtk3.dockerfile -t go-gtk3 .
	podman build -f go-gtk3-examples.dockerfile -t $(GO_GTK3_EXAMPLES) .
	podman run -it \
		--name $(GO_GTK3_EXAMPLES) \
		-e DISPLAY \
		-v /run/dbus/system_bus_socket:/run/dbus/system_bus_socket:ro \
		-v /tmp/.X11-unix:/tmp/.X11-unix:ro \
		-v /home/rianby64/GoGTK/src:/home/rianby64/go/src/GoGTK \
		--device /dev/dri \
		--device /dev/snd \
		--ipc=host \
		--userns keep-id \
		-u ${USER} \
		$(GO_GTK3_EXAMPLES)
else
	podman start -ia $(CONTAINER_ID_EXAMPLES)
endif

$(eval GO_GTK3_CANVAS_EXAMPLE := $(shell echo 'go-gtk3-canvas-examples'));
$(eval CONTAINER_ID_CANVAS_EXAMPLE := $(shell podman ps -a | grep $(GO_GTK3_CANVAS_EXAMPLE) | awk '{ print $$1 }'))

canvas:
ifeq ($(CONTAINER_ID_CANVAS_EXAMPLE),)
	podman build -f go-gtk3.dockerfile -t go-gtk3 .
	podman build -f go-gtk3.canvas-example.dockerfile -t $(GO_GTK3_CANVAS_EXAMPLE) .
	podman run -it \
		--name $(GO_GTK3_CANVAS_EXAMPLE) \
		-e DISPLAY \
		-v /run/dbus/system_bus_socket:/run/dbus/system_bus_socket:ro \
		-v /tmp/.X11-unix:/tmp/.X11-unix:ro \
		-v /home/rianby64/GoGTK/src:/home/rianby64/example-canvas \
		--device /dev/dri \
		--device /dev/snd \
		--ipc=host \
		--userns keep-id \
		-u ${USER} \
		$(GO_GTK3_CANVAS_EXAMPLE)
else
	podman start -ia $(CONTAINER_ID_CANVAS_EXAMPLE)
endif

clean:
ifeq ($(CONTAINER_ID_EXAMPLES),)
	@echo "Examples already gone"
else
	podman rm $(CONTAINER_ID_EXAMPLES)
endif
ifeq ($(CONTAINER_ID_CANVAS_EXAMPLE),)
	@echo "Canvas already gone"
else
	podman rm $(CONTAINER_ID_CANVAS_EXAMPLE)
endif