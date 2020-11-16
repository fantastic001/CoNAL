FROM archlinux

RUN pacman -Syu --noconfirm && \
    pacman -S --noconfirm archlinux-keyring && \
    pacman -S --noconfirm \
    gcc \
    cmake \ 
    boost \
    gtest \
    gmock \
    make \
    bison \
    flex && \
    rm -rf /var/cache/pacman/*

COPY . /usr/src/conal 
WORKDIR /usr/src/conal

RUN cmake . -DCMAKE_INSTALL_PREFIX=/opt/conal/ && \
    make && \ 
    make install && \
    mv docker/setup-env.sh /bin/ && \
    cd .. && rm -rf conal 

ENTRYPOINT ["bash", "--rcfile", "/bin/setup-env.sh"]