FROM ubuntu:22.04

RUN apt-get update && apt-get install -y g++ gdb gdbserver

WORKDIR /app
COPY . .

RUN g++ -g -o long_running long_running.cpp

EXPOSE 9999

CMD ["gdbserver", ":9999", "./long_running"]