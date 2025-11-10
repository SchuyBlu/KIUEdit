FROM debian:bullseye-slim

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get upgrade -y && \
	apt-get install -y --no-install-recommends apt-utils && \
	apt-get install -y --no-install-recommends sudo ca-certificates pkg-config curl wget bzip2 xz-utils make libarchive-tools doxygen gnupg && \
	apt-get install -y --no-install-recommends git git-restore-mtime && \
	apt-get install -y --no-install-recommends rsync && \
	apt-get install -y --no-install-recommends cmake zip unzip ninja-build && \
	apt-get install -y --no-install-recommends python && \
	apt-get install -y --no-install-recommends locales && \
	apt-get install -y --no-install-recommends patch && \
	sed -i -e 's/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/' /etc/locale.gen && \
	dpkg-reconfigure --frontend=noninteractive locales && \
	update-locale LANG=en_US.UTF-8 && \
	apt-get clean && \
	rm -rf /var/lib/apt/lists/*

RUN ln -s /proc/mounts /etc/mtab && \
	wget https://apt.devkitpro.org/install-devkitpro-pacman && \
	chmod +x ./install-devkitpro-pacman && \
	./install-devkitpro-pacman && \
	rm ./install-devkitpro-pacman && \
	dkp-pacman -S --needed --noconfirm 3ds-dev && \
	yes | dkp-pacman -Scc

# Install project specific dependencies
RUN apt-get update && apt-get install -y gcc libjson-c-dev && \
	yes | dkp-pacman -S 3ds-libjson-c

# Install ssh server dependencies
RUN apt-get install -y openssh-server openssh-sftp-server && \
	mkdir -p /var/run/sshd && ssh-keygen -A

# Set up basic user.
RUN useradd -m -s /bin/bash user && \
	echo "user:password" | chpasswd

RUN mkdir -p /home/user/.ssh

COPY keys/id_rsa_shared.pub /home/user/.ssh/authorized_keys
RUN chown -R user:user /home/user/.ssh && \
    chmod 700 /home/user/.ssh && \
    chmod 600 /home/user/.ssh/authorized_keys

ENV LANG=en_US.UTF-8

ENV DEVKITPRO="/opt/devkitpro"
ENV DEVKITARM="/opt/devkitpro/devkitARM"
ENV DEVKITPPC="/opt/devkitpro/devkitPPC"
ENV PATH=${DEVKITPRO}/tools/bin:$PATH

# Setup port and entrypoint for ssh service
EXPOSE 22
CMD ["/usr/sbin/sshd","-D"]

