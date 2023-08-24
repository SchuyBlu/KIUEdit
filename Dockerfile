FROM debian:buster-slim

ARG USERNAME

ENV DEBIAN_FRONTEND=noninteractive
ENV TERM=xterm
ENV LANG en_US.UTF-8

# Install C development tools, git, and gh CLI tools
RUN apt-get update && \
    apt-get install -y build-essential && \
    apt-get install -y curl && \
    type -p curl >/dev/null || (apt-get update && apt-get install curl -y) && \
    curl -fsSL https://cli.github.com/packages/githubcli-archive-keyring.gpg | dd of=/usr/share/keyrings/githubcli-archive-keyring.gpg && \
    chmod go+r /usr/share/keyrings/githubcli-archive-keyring.gpg && \
    echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/githubcli-archive-keyring.gpg] https://cli.github.com/packages stable main" | tee /etc/apt/sources.list.d/github-cli.list > /dev/null && \
    apt-get update &&\
    apt-get install gh -y

# Install requirements for devkitpro
RUN echo "deb http://deb.debian.org/debian buster-backports main" > /etc/apt/sources.list.d/buster-backports.list && \
    apt-get update && apt-get upgrade -y && \
    apt-get install -y --no-install-recommends apt-utils && \
    apt-get install -y --no-install-recommends sudo ca-certificates pkg-config curl wget bzip2 xz-utils make bsdtar doxygen gnupg && \
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

# Install nodejs, vim, and neovim
RUN apt-get install -y npm nodejs && \
	wget https://github.com/neovim/neovim/releases/download/stable/nvim.appimage && \
	chmod +x nvim.appimage && \
	mv nvim.appimage /usr/bin/

# Set up user and copy files.
ENV HOME "/${USERNAME}/home"
WORKDIR "${HOME}"
COPY . ./KIUEdit/
RUN git clone https://github.com/ThrowTheSwitch/Unity.git "${HOME}/KIUEdit/unity" && \
    ln -s /bin/bash /usr/bin/bash && \
    addgroup --gid 1000 "${USERNAME}" && \
    adduser --disabled-password --gecos "" --uid  1000 --gid 1000 "${USERNAME}"

# Set up bashrc
RUN echo 'PS1='"'"'\[\e[1;32m\]\u@\h \[\e[1;33m\][\W]\[\e[1;32m\]$\[\e[0;37m\] '"'" >> "${HOME}/.bashrc" && \
    echo 'if [ -x /usr/bin/dircolors ]; then' >> "${HOME}/.bashrc" && \
    echo '    test -r ~/.dircolors && eval "$(dicolors -b ~/.dircolors)" || eval "$(dircolors -b)"' >> "${HOME}/.bashrc" && \
    echo '    alias ls="ls --color=auto"' >> "${HOME}/.bashrc" && \
    echo '    alias grep="grep --color=auto"' >> "${HOME}/.bashrc" && \
    echo '    alias fgrep="fgrep --color=auto"' >> "${HOME}/.bashrc" && \
    echo '    alias egrep="egrep --color=auto"' >> "${HOME}/.bashrc" && \
    echo 'fi' >> "${HOME}/.bashrc" && \
	echo 'alias nvim="nvim.appimage --appimage-extract-and-run"' >> "${HOME}/.bashrc"

# Change ownership of home directory and all it's contents from root
# to the user and user group.
RUN chgrp -R 1000 ${HOME} && \
chown -R ${USERNAME} ${HOME}

ENV DEVKITPRO=/opt/devkitpro
ENV DEVKITARM=/opt/devkitpro/devkitARM
ENV PATH=${DEVKITPRO}/tools/bin:$PATH

USER "${USERNAME}"
ENV TERM=xterm-256color

