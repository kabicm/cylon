FROM --platform=linux/amd64 ubuntu:22.04
MAINTAINER cylondata@googlegroups.com

ARG DEBIAN_FRONTEND=noninteractive

ENV LANG=C.UTF-8 LC_ALL=C.UTF-8
ENV PATH /opt/conda/bin:$PATH

ENV CYLON_HOME=/cylon
ENV CYLON_PREFIX=/cylon/install
ENV UCC_HOME=/ucc
ENV UCX_HOME=/ucx

#give ARG EXPOSE_ENV a default value
ARG EXPOSE_ENV=33000

#assign the EXPOSE_ENV arg to the EXPOSE_ENV ENV so that it can be accessed
#by the subsequent RUN call within the container
ENV EXPOSE_ENV $EXPOSE_ENV


WORKDIR $CYLON_HOME

RUN DEBIAN_FRONTEND=noninteractive apt-get update -y --fix-missing && \
    apt-get install -y wget bzip2 ca-certificates curl git build-essential && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

RUN wget --quiet https://repo.anaconda.com/miniconda/Miniconda3-py310_23.3.1-0-Linux-x86_64.sh -O ~/miniconda.sh && \
    /bin/bash ~/miniconda.sh -b -p /opt/conda && \
    rm ~/miniconda.sh && \
    #/opt/conda/bin/conda clean -a && \
    ln -s /opt/conda/etc/profile.d/conda.sh /etc/profile.d/conda.sh && \
    echo ". /opt/conda/etc/profile.d/conda.sh" >> ~/.bashrc && \
    echo "conda activate cylon_dev" >> ~/.bashrc && \
    echo "export LD_LIBRARY_PATH=$CYLON_PREFIX"/lib >> ~/.bashrc


RUN git clone https://github.com/mstaylor/cylon.git $CYLON_HOME
RUN cd $CYLON_HOME && git checkout -b ucc-ucx-redis-local-ucx origin/ucc-ucx-redis-local-ucx



SHELL ["/bin/bash", "--login", "-c"]

RUN conda env create -f $CYLON_HOME/conda/environments/cylon_NoUCX.yml

RUN . /opt/conda/etc/profile.d/conda.sh && \
    conda activate cylon_dev && \
    conda install -n base conda-libmamba-solver && conda config --set solver libmamba && \
    conda install -c anaconda boto3 -y && pip install cloudmesh-openstack &&\
    conda install -c conda-forge redis-py -y



RUN apt-get update &&  apt-get install autoconf -y &&  apt-get install libtool -y && apt-get install cmake -y && \
    apt-get install libnuma-dev -y

#UCX install
#Note: Includes support for overriding device eth address
RUN git clone --single-branch -b override-remote-address3 https://github.com/mstaylor/ucx.git $UCX_HOME
RUN cd $UCX_HOME && \
   ./autogen.sh && \
   ./configure --prefix=$UCX_HOME/install && \
   make install

#UCC install

RUN git clone --single-branch -b master https://github.com/openucx/ucc.git $UCC_HOME
RUN cd $UCC_HOME && \
   ./autogen.sh && \
   ./configure --prefix=$UCC_HOME/install --with-ucx=$UCX_HOME/install && \
   make install

#REDIS install
RUN git clone https://github.com/redis/hiredis.git /hiredis && \
    cd /hiredis && \
    make && \
    make install && \
    git clone https://github.com/sewenew/redis-plus-plus.git /redis-plus-plus && \
    cd /redis-plus-plus && \
    mkdir build && \
    cd build && \
    cmake -DREDIS_PLUS_PLUS_CXX_STANDARD=11 .. && \
    make && make install


RUN . /opt/conda/etc/profile.d/conda.sh && \
    conda activate cylon_dev && \
    cd $CYLON_HOME && \
    python build.py -cmake-flags="-DCYLON_UCX=1 -DUCX_INSTALL_PREFIX=$UCX_HOME/install -DCYLON_UCC=1 -DUCC_INSTALL_PREFIX=$UCC_HOME/install -DCYLON_USE_REDIS=1 " -ipath="$CYLON_PREFIX" --cpp --python --test --pytest

RUN chmod 0755 /cylon/aws/scripts/runCylon.sh

EXPOSE $EXPOSE_ENV

ENTRYPOINT ["/cylon/aws/scripts/runCylon.sh"]
