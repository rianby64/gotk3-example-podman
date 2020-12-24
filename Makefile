
test:
	podman build -f Go-GTK3.base.dockerfile -t go-gtk3 .
	podman build -f Go-GTK3.example.dockerfile -t go-gtk3-example .
	podman run --rm -it \
		-v /tmp/.X11-unix:/tmp/.X11-unix \
		-v /dev/dri:/dev/dri \
		--security-opt=label=type:container_runtime_t \
		-e DISPLAY \
		go-gtk3-example