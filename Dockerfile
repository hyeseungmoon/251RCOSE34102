FROM ubuntu:latest

FROM gcc:latest
COPY . /usr/src/COSE341
RUN apt-get update
RUN apt-get install cmake -y
RUN mkdir cmake-build-debug

WORKDIR /usr/src/COSE341/cmake-build-debug
RUN cmake ../
RUN cmake --build .

ENTRYPOINT ["./COSE341"]
