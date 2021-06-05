FROM ubuntu:20.04
MAINTAINER Lys

RUN apt-get update && apt-get install -qy xinetd
RUN useradd -m gotplt
RUN chown -R root:root /home/gotplt
RUN chmod -R 755 /home/gotplt

CMD ["/usr/sbin/xinetd","-dontfork"]
