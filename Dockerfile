FROM agesofwarcraft/wowd-base:develop

RUN mkdir /source
RUN mkdir -p /opt/wowd

COPY . /source
RUN mkdir /source/build

WORKDIR /source/build

ENV CXX=/usr/bin/g++-6
ENV CC=/usr/bin/gcc-6

RUN cmake ../ -DWITH_WARNINGS=1 -DWITH_COREDEBUG=0 -DUSE_COREPCH=1 -DUSE_SCRIPTPCH=1 -DTOOLS=1 -DSCRIPTS=static -DSERVERS=1 -DNOJEM=0 -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/opt/wowd/
RUN make -j$(cat /proc/cpuinfo | grep processor | wc -l)
RUN make install

FROM agesofwarcraft/wowd-base:run

COPY build-customdeps.bash /build-customdeps.bash
RUN bash /build-customdeps.bash

RUN mkdir -p /opt/wowd
COPY --from=0 /opt/wowd /opt/wowd

# Verify that our build was successful (no linkage issues)
RUN /opt/wowd/bin/bnetserver -v
RUN /opt/wowd/bin/worldserver -v
