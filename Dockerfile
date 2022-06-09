FROM centos:centos6
MAINTAINER The CentOS Project <cloud-ops@centos.org>

RUN cd /etc/yum.repos.d/ && rm -f *.repo
RUN echo "12:46 am 6/9/2022"
COPY CentOS-Base.repo /etc/yum.repos.d/
RUN yum -y update; yum clean all



RUN yum -y install gcc openssl-devel bzip2-devel wget sqlite-devel && \
    cd /tmp/ && \
    wget https://www.python.org/ftp/python/3.6.6/Python-3.6.6.tgz && \
    tar xzf Python-3.6.6.tgz && \
    cd Python-3.6.6 && \
    ./configure --enable-optimizations --enable-loadable-sqlite-extensions && \
    make altinstall

# RUN pip3.6 install --upgrade pip && \
#     pip3.6 install conan
#    10  make altinstall
#    11  which python
#    12  which python3
#    13  which python3.6
#    14  python3.6 --version
#    15  ls /usr/local/bin/
#    16  pip3.6 install conan
#    17  pip3.6 install --upgrade pip
#    18  pip3.6 install conan
#    19  conan --version
#    20  yum install sqlite-devel
#    21  ./configure --enable-optimizations --enable-loadable-sqlite-extensions
#    22  make altinstall
#    23  python3.6
#    24  pip3.6 install conan
#    25  conan --version
#    26  conan profile new default --detect
#    27  conan profile update settings.compiler.libcxx=libstdc++11 default
#    28  conan set general.revisions_enabled=1
#    29  conan config set general.revisions_enabled=1
#    30  conan install fmt/8.0.1@ --build=missing
#    31  yum install cmake
#    32  conan install fmt/8.0.1@ --build=missing
#    33  history











#RUN yum -y install epel-release; yum clean all


#RUN yum -y install yum-utils; yum clean all



#RUN yum -y install gcc openssl-devel bzip2-devel && \
#    cd /tmp/ && \
#    wget https://www.python.org/ftp/python/3.6.6/Python-3.6.6.tgz


#RUN yum -y install python-pip; yum clean all
#RUN yum -y install python3; yum clean all

RUN echo "blah"
# ADD . /src

# RUN cd /src; pip install -r requirements.txt

# EXPOSE 8080

# CMD ["python", "/src/index.py"]

#RUN yum -y install gcc openssl-devel bzip2-devel wget

# RUN yum install cmake git nano centos-release-sc && \
#     yum install rh-python36 && \
#     scl enable rh-python36 bash && \
#     python --version
# RUN yum -y install python3 python3-pip

# apt-get update && \
#   DEBIAN_FRONTEND=noninteractive apt-get install -y \
#   gpg \
#   wget \
#   curl \
#   flex \
#   bison \
#   git \
#   nano \
#   python3 \
#   python3-pip

# RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null \
#   | gpg --dearmor - \
#   | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null && \
#   echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ focal main' \
#   | tee /etc/apt/sources.list.d/kitware.list >/dev/null && \
#   apt-get update && \
#   rm /usr/share/keyrings/kitware-archive-keyring.gpg && \
#   DEBIAN_FRONTEND=noninteractive apt-get install -y \
#   kitware-archive-keyring \
#   cmake

# RUN echo "Change me if conan installation is dirty: 6/6/2022 5:39pm"

# RUN pip3 install conan && \
#   conan profile new default --detect && \
#   conan profile update settings.compiler.libcxx=libstdc++11 default && \
#   conan config set general.revisions_enabled=1 && \
#   conan remote add blackencino-vfx https://blackencino.jfrog.io/artifactory/api/conan/blackencino-vfx-conan && \
#   conan install --build=missing fmt/8.0.1@

# RUN echo "Change me if conanfile is dirty: 6/6/2022 5:39pm"

# RUN mkdir /houndstooth_src && \
#     mkdir -p /houndstooth_src/build/Release

# COPY conanfile.py /houndstooth_src/
# RUN cd /houndstooth_src/build/Release && \
#     conan install --build=missing --options build_guis=False ../..

# RUN mkdir /houndstooth && \
#   mkdir -p /houndstooth_src/cmake && \
#   mkdir -p /houndstooth_src/external && \
#   mkdir -p /houndstooth_src/houndstooth

# RUN echo "Change me if conanfile not dirty but code is: 6/6/2022 5:39pm"

# COPY cmake /houndstooth_src/cmake
# COPY external /houndstooth_src/external
# COPY houndstooth /houndstooth_src/houndstooth
# COPY CMakeLists.txt /houndstooth_src/

# RUN cd /houndstooth_src/build/Release && \
#   cmake -DCMAKE_BUILD_TYPE=Release \
#     -DCMAKE_INSTALL_PREFIX=/houndstooth \
#     -DHOUNDSTOOTH_BUILD_GUIS=OFF \
#     -GNinja \
#     ../..

# RUN cd /houndstooth_src/build/Release && \
#   ninja && \
#   ninja install

# RUN rm -rf /houndstooth_src && \
#   conan remove "*" --builds --src --force && \
#   rm -rf /var/lib/apt/lists/*

CMD [ "/bin/bash" ]
