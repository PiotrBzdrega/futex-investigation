# podman build --platform linux/arm/v7 -t futex-investigation .
# podman run --rm -it futex-investigation sh
# podman rmi localhost/futex-investigation
# podman save -o futex-investigation.tar futex-investigation
# scp ./futex-investigation.tar administrator@172.20.73.44:/tmp
# chown container:container /tmp/futex-investigation.tar
# mv /tmp/futex-investigation.tar /mnt/container/
# podman load -i futex-investigation.tar

FROM alpine:3.21.3 AS builder

# Install build dependencies
RUN apk update && \
    apk add --no-cache \
    build-base \
    cmake \
    g++ \
    git \
    linux-headers

# Building
WORKDIR /src/futex-investigation
COPY . .

# reconfigure
RUN /usr/bin/cmake -DCMAKE_BUILD_TYPE:STRING=Release -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE --no-warn-unused-cli -B build

# clean
RUN /usr/bin/cmake --build build --config Release --target clean -j 14 --

# build
RUN /usr/bin/cmake --build build --config Release --target all -j 14 --


# Stage 2: Runtime
FROM alpine:3.21.3 AS runtime

# Install only strictly necessary runtime deps
# For dynamically linked C++ programs:
RUN apk add strace

RUN apk add --no-cache libstdc++ 
# strace
COPY --from=builder /src/futex-investigation/build/futex-investigation .


CMD ["sleep", "infinity"]