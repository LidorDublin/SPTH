FROM ubuntu:latest
MAINTAINER lidord02@gmail.com

RUN apt update -yqq

RUN apt install software-properties-common -yqq
RUN add-apt-repository ppa:ubuntu-toolchain-r/test
RUN apt update -yqq
RUN apt install gcc-9 g++-9 -yqq

RUN apt install make cmake libcurlpp0 libcurlpp-dev libcurl4-openssl-dev -yqq

RUN mkdir /spth
COPY . /spth

RUN ln /usr/bin/g++-9 /usr/bin/g++

RUN cd /spth/ && cmake -DCMAKE_BUILD_TYPE=Release . && make 

WORKDIR /spth

ENTRYPOINT ./shortest-path-to-hitler
