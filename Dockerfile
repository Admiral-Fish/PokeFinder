# Use ubuntu base
FROM ubuntu:18.04

# Setup workdir
WORKDIR /app

# Install dependencies
RUN apt-get update
RUN apt install -y build-essential qtbase5-dev qt5-qmake qt5-default