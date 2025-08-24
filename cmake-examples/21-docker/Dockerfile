FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    g++ gdb gdbserver cmake git make

#what are UID and GID?
ARG UID=1000
ARG GID=1000

RUN groupadd -g ${GID} devgroup \
    && useradd -m -u ${UID} -g ${GID} devuser

USER devuser

WORKDIR /app

COPY dev_profile.sh /app/dev_profile.sh
RUN echo "source /app/dev_profile.sh" >> /home/devuser/.bashrc

