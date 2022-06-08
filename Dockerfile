FROM nvcr.io/nvidia/cuda:11.4.2-devel-ubuntu20.04

RUN apt-get update && \
  DEBIAN_FRONTEND=noninteractive apt-get install -y \
  gpg \
  wget \
  curl \
  flex \
  bison \
  git \
  nano \
  ninja-build \
  python3 \
  python3-pip

RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null \
  | gpg --dearmor - \
  | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null && \
  echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ focal main' \
  | tee /etc/apt/sources.list.d/kitware.list >/dev/null && \
  apt-get update && \
  rm /usr/share/keyrings/kitware-archive-keyring.gpg && \
  DEBIAN_FRONTEND=noninteractive apt-get install -y \
  kitware-archive-keyring \
  cmake

RUN echo "Change me if conan installation is dirty: 6/6/2022 5:39pm"

RUN pip3 install conan && \
  conan profile new default --detect && \
  conan profile update settings.compiler.libcxx=libstdc++11 default && \
  conan config set general.revisions_enabled=1 && \
  conan remote add blackencino-vfx https://blackencino.jfrog.io/artifactory/api/conan/blackencino-vfx-conan && \
  conan install --build=missing fmt/8.0.1@

RUN echo "Change me if conanfile is dirty: 6/6/2022 5:39pm"

RUN mkdir /houndstooth_src && \
    mkdir -p /houndstooth_src/build/Release

COPY conanfile.py /houndstooth_src/
RUN cd /houndstooth_src/build/Release && \
    conan install --build=missing --options build_guis=False ../..

RUN mkdir /houndstooth && \
  mkdir -p /houndstooth_src/cmake && \
  mkdir -p /houndstooth_src/external && \
  mkdir -p /houndstooth_src/houndstooth

RUN echo "Change me if conanfile not dirty but code is: 6/6/2022 5:39pm"

COPY cmake /houndstooth_src/cmake
COPY external /houndstooth_src/external
COPY houndstooth /houndstooth_src/houndstooth
COPY CMakeLists.txt /houndstooth_src/

RUN cd /houndstooth_src/build/Release && \
  cmake -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/houndstooth \
    -DHOUNDSTOOTH_BUILD_GUIS=OFF \
    -GNinja \
    ../..

RUN cd /houndstooth_src/build/Release && \
  ninja && \
  ninja install

RUN rm -rf /houndstooth_src && \
  conan remove "*" --builds --src --force && \
  rm -rf /var/lib/apt/lists/*

CMD [ "/bin/bash" ]
