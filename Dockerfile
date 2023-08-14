FROM ubuntu:20.04

# Copy all files into container
COPY . .

ARG DEBIAN_FRONTEND=noninteractive

# Install base dependencies
RUN apt-get update && \
apt-get install -y gcc make wget vim git && \
git clone https://github.com/ThrowTheSwitch/Unity && \
mv Unity unity

# Install devkitpro
RUN wget https://apt.devkitpro.org/install-devkitpro-pacman && \
echo "$(head -n -2 ./install-devkitpro-pacman)" > ./install-devkitpro-pacman && \
chmod +x ./install-devkitpro-pacman && \
./install-devkitpro-pacman && \
apt-get update && \
apt-get install -y devkitpro-pacman && \
ln -s /proc/mounts /etc/mtab && \
dkp-pacman --noconfirm -S libctru 3dslink

ENV DEVKITPRO=/opt/devkitpro
ENV PATH=${DEVKITPRO}/tools/bin:$PATH

