FROM fedora AS go-gtk3

ENV GOPATH=/go
ENV GOBIN=${GOPATH}/bin

RUN yum install -y golang.x86_64 libcanberra-gtk3.x86_64 glib2-devel.x86_64 cairo-gobject-devel.x86_64 pango-devel.x86_64 gtk3-devel.x86_64 git.x86_64 unzip.x86_64 mesa-libGL.x86_64 mesa-dri-drivers.x86_64 libXrender.x86_64 libXtst.x86_64 xorg-x11-fonts-100dpi.noarch xorg-x11-fonts-75dpi.noarch xorg-x11-fonts-ISO8859-1-100dpi.noarch xorg-x11-fonts-ISO8859-1-75dpi.noarch xorg-x11-fonts-ISO8859-14-100dpi.noarch xorg-x11-fonts-ISO8859-14-75dpi.noarch xorg-x11-fonts-ISO8859-15-100dpi.noarch xorg-x11-fonts-ISO8859-15-75dpi.noarch xorg-x11-fonts-ISO8859-2-100dpi.noarch xorg-x11-fonts-ISO8859-2-75dpi.noarch xorg-x11-fonts-ISO8859-9-100dpi.noarch xorg-x11-fonts-ISO8859-9-75dpi.noarch xorg-x11-fonts-Type1.noarch xorg-x11-fonts-cyrillic.noarch xorg-x11-fonts-ethiopic.noarch xorg-x11-fonts-misc.noarch wget.x86_64

RUN mkdir /go

RUN go get github.com/gotk3/gotk3/gtk
