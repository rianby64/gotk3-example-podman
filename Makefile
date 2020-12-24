
test:
	podman build -f ContainerFile.simple -t gogtk .
	podman run --rm -it \
		-v /tmp/.X11-unix:/tmp/.X11-unix \
		-v /dev/dri:/dev/dri \
		--security-opt=label=type:container_runtime_t \
		-e DISPLAY \
		gogtk