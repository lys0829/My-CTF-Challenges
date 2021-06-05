FROM ubuntu:20.04
MAINTAINER Lys

RUN apt-get update && apt-get install -qy xinetd
RUN useradd -m stdout
RUN chown -R root:root /home/stdout
RUN chmod -R 755 /home/stdout

CMD ["/usr/sbin/xinetd","-dontfork"]
