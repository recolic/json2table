# Quick build static binary: sudo docker build .
from ubuntu:18.04

run DEBIAN_FRONTEND=noninteractive apt update && DEBIAN_FRONTEND=noninteractive apt install -y g++ make curl

copy . /build
workdir /build
run make CXX='g++ -Ilib -static'
run curl https://recolic.net/setup/qu | f=json2table bash

