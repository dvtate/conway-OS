FROM ubuntu:latest

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && \
    apt-get install -y \
    bat \
    binutils \
    csh \
    clang \
    curl \
    cscope \
    ctags \
    curl \
    emacs \
    fd-find \
    flex \
    g++ \
    gcc \
    gdb \
    git \
    grub-pc-bin \
    grub2-common \
    locate \
    make \
    neovim \
    netcat \
    perl \
    python \
    python3 \
    python3-pip \
    qemu-system-x86 \
    silversearcher-ag \
    valgrind \
    vim \
    xorriso
