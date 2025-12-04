FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    cmake python3 python3-dev python3-pip wrk clangd \
    libgtest-dev build-essential git \
    && rm -rf /var/lib/apt/lists/*

RUN apt-get update && apt-get install -y gcc-14 g++-14 && \
    update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-14 100 && \
    update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-14 100

RUN cd /usr/src/gtest && \
    mkdir build && cd build && \
    cmake .. && \
    make && \
    cp lib/libgtest*.a /usr/lib/ && \
    cp -r ../include/gtest /usr/include/

RUN pip3 install numpy --break-system-packages

WORKDIR /workspace

CMD ["/bin/bash"]
