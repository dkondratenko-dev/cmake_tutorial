FROM ubuntu:22.04

#Install g++, gdb, and gdbserver
RUN apt-get update && apt-get install -y g++ gdb gdbserver

WORKDIR /app
COPY . .

#Run g++ to compile long_running.cpp and build the executable long-runnning
RUN g++ -g -o long_running long_running.cpp     

EXPOSE 9999

CMD ["gdbserver", ":9999", "./long_running"]