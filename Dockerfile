FROM debian:buster-slim

SHELL ["/bin/bash", "--login", "-c"]

ENV DEBIAN_FRONTEND noninteractive

# Declare args that MUST be set (in the shell script).
ARG USERNAME

# Declare args that can have a default.
ARG GIT_NAME=none
ARG GIT_EMAIL=none

# Install standard tools used in ubuntu. Also installs requirements to build
# neovim, as well as install all dependencies for neovim lsp features and
# plugins.
RUN apt-get update && \
	apt-get install -y build-essential git curl wget python3 python3-pip \
	luajit ninja-build gettext cmake unzip zsh nodejs npm

# Update node and npm
RUN npm install -g n && \
	n lts && \
	n latest && \
	n prune

# Install gh command line
RUN type -p curl >/dev/null || (apt-get update && apt-get install curl -y) && \
    curl -fsSL https://cli.github.com/packages/githubcli-archive-keyring.gpg | dd of=/usr/share/keyrings/githubcli-archive-keyring.gpg && \
    chmod go+r /usr/share/keyrings/githubcli-archive-keyring.gpg && \
    echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/githubcli-archive-keyring.gpg] https://cli.github.com/packages stable main" | tee /etc/apt/sources.list.d/github-cli.list > /dev/null && \
    apt-get update &&\
    apt-get install gh -y

# Setup user
RUN adduser --disabled-password ${USERNAME} && \
	echo "${USERNAME}:${USERNAME}" | chpasswd

# Switch to user and finish configuration
USER ${USERNAME}

ENV USERNAME=${USERNAME}
ENV EDITOR nvim
ENV MAIN_SHELL zsh
ENV SH zsh
ENV TERM xterm

WORKDIR /home/${USERNAME}/workspace

# Set up global git config if git variables are not none
RUN if [ "${GIT_NAME}" -ne "none" ] && [ "${GIT_EMAIL}" -ne "none" ]; then \
	git config --global user.name "${GIT_NAME}" \
	git config --global user.email "${GIT_EMAIL}" \
	git config --global core.editor "nvim"; \
	fi

# Set up zsh (W.I.P)
RUN echo "" >> ~/.zshrc

# Build neovim
USER root
RUN git clone --depth 1 https://github.com/neovim/neovim.git /tmp/neovim && \
	cd /tmp/neovim && \
	make CMAKE_BUILD_TYPE=Release
RUN cd /tmp/neovim && \
	make install && \
	rm -rf /tmp/neovim
USER ${USERNAME}

# Clone config and install lazy.nvim
# NOTE: According to my nvim config, lazy is supposed to install automatically.
# Installing through the command line allows me to call lazy commands headless
# immediately, justifying installing it externally.
RUN git clone --depth 1 https://github.com/SchuyBlu/nvim.git /home/${USERNAME}/.config/nvim && \
	git clone --filter=blob:none --branch=stable https://github.com/folke/lazy.nvim.git /home/$( whoami )/.local/share/nvim && \
	nvim --headless -c "let &runtimepath.=','.string(\"/home/$( whoami )/.local/share/nvim\") | quit"

# ---------------------------- DEVKITPRO SPECIFIC -----------------------------
# Install requirements for devkitpro
USER root
RUN echo "deb http://deb.debian.org/debian buster-backports main" > /etc/apt/sources.list.d/buster-backports.list && \
    apt-get update && apt-get upgrade -y && \
    apt-get install -y --no-install-recommends apt-utils && \
    apt-get install -y --no-install-recommends sudo ca-certificates pkg-config bzip2 xz-utils make bsdtar doxygen gnupg && \
    apt-get install -y --no-install-recommends cmake/buster-backports zip unzip && \
    apt-get install -y --no-install-recommends locales && \
    apt-get install -y --no-install-recommends patch && \
    sed -i -e 's/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/' /etc/locale.gen && \
    dpkg-reconfigure --frontend=noninteractive locales && \
    update-locale LANG=en_US.UTF-8 && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Install devkitpro-pacman and devkitpro
RUN ln -s /proc/mounts /etc/mtab && \
    wget https://apt.devkitpro.org/install-devkitpro-pacman && \
    chmod +x ./install-devkitpro-pacman && \
    ./install-devkitpro-pacman && \
    rm ./install-devkitpro-pacman && \
    yes | dkp-pacman -S libctru 3dslink devkitARM

# Set devkitpro env variables
ENV DEVKITPRO="/opt/devkitpro"
ENV DEVKITARM="/opt/devkitpro/devkitARM"
ENV DEVKITPPC="/opt/devkitpro/devkitPPC"

USER ${USERNAME}

