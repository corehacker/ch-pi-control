#!/bin/bash

PWD=`pwd`

LOG_FILE="$PWD/install.log"
TEMP_DIR="$PWD/temp"
INSTALLATION_TARGET="/usr/local"

LIBEVENT_DOWNLOAD="https://github.com/libevent/libevent/releases/download/release-2.1.8-stable/libevent-2.1.8-stable.tar.gz"
GLOG_GIT_DOWNLOAD="https://github.com/google/glog.git"
GLOG_GIT_TAG="v0.4.0"
GPERFTOOLS_GIT_DOWNLOAD="https://github.com/gperftools/gperftools.git"
GPERFTOOLS_GIT_TAG="gperftools-2.7"
CH_CPP_UTILS_GIT_DOWNLOAD="https://github.com/corehacker/ch-cpp-utils.git"
CH_STORAGE_CLIENT_GIT_DOWNLOAD="https://github.com/corehacker/ch-pi-control.git"

function cleanup {
  sudo rm -rf $TEMP_DIR ltmain.sh
}

function end_installation {
  echo "----------------------------------------------------------------------------" >> $LOG_FILE
  echo "                        Ending Installation" >> $LOG_FILE
  echo "----------------------------------------------------------------------------" >> $LOG_FILE
  log "----------------------------------------------------------------------------"
  log "                        Ending Installation"
  log "----------------------------------------------------------------------------"
}

function log {
  echo "[$0] $1"
}

function init {
  echo "" > $LOG_FILE
  echo "----------------------------------------------------------------------------" >> $LOG_FILE
  echo "                        Starting Installation" >> $LOG_FILE
  echo "----------------------------------------------------------------------------" >> $LOG_FILE
  log "----------------------------------------------------------------------------"
  log "                        Starting Installation"
  log "----------------------------------------------------------------------------"

  log "[init] mkdir $TEMP_DIR" && \
  mkdir -p $TEMP_DIR && cd $TEMP_DIR
  echo "Temporary Dir   : $TEMP_DIR"
  echo "Installation Dir: $INSTALLATION_TARGET"
}

function tools_install {	
    log "[tools] Installing tools: vim build-essential autoconf libtool htop curl wget git ..."
    sudo apt -y install vim build-essential autoconf libtool htop curl wget git &>> $LOG_FILE
    log "[tools] Installing tools: vim build-essential autoconf libtool htop curl wget git ... done"
}

function configure_make_make_install {
  log "[$1] configuring..." && \
  ./configure --prefix=$INSTALLATION_TARGET &>> $LOG_FILE && \
  log "[$1] make..." && \
  make -j8 &>> $LOG_FILE && \
  log "[$1] make install..." && \
  sudo make -j8 install &>> $LOG_FILE
}

function remove_existing_libevent {
  if apt list --installed | grep libevent; then
    log "[libevent] Already installed through apt. Uninstalling..."
    uninstall_package=`apt list --installed | grep libevent | cut -d'/' -f1`
    sudo apt -y remove $uninstall_package &>> $LOG_FILE
    log "[libevent] Already installed through apt. Uninstalled."
  else
    log "[libevent] No libevent installation found through apt."
  fi
}

function install_libevent {

  remove_existing_libevent  

  if ldconfig -p | grep libevent; then
    log "[libevent] Already installed based on ldconfig."
  elif test -f /usr/local/lib/libevent.so; then
    log "[libevent] Already installed @ /usr/local/lib/libevent.so."
    ls -lh /usr/local/lib/libevent.so
  else
    log "[libevent] Downloading $LIBEVENT_DOWNLOAD..."
    wget -O libevent.tar.gz $LIBEVENT_DOWNLOAD &>> $LOG_FILE && \
    mkdir -p libevent && \
    log "[libevent] Untar libevent..."
    tar xf libevent.tar.gz -C libevent --strip-components=1 && \
    cd libevent && \
    configure_make_make_install "libevent" && \
    cd ..
  fi
}

function install_glog {
  if ldconfig -p | grep libglog; then
    log "[glog] Already installed."
  else
    log "[glog] Cloning $GLOG_GIT_DOWNLOAD..." && \
    git clone $GLOG_GIT_DOWNLOAD && \
    cd glog && \
    log "[glog] Checkout $GLOG_GIT_TAG..." && \
    git checkout $GLOG_GIT_TAG
    log "[glog] Running autogen.sh..." && \
    ./autogen.sh &>> $LOG_FILE
    log "[glog] Running autogen.sh..." && \
    ./autogen.sh &>> $LOG_FILE && configure_make_make_install "glog" && \
  cd ..
  fi
}

function install_gperftools {
  if ldconfig -p | grep libtcmalloc; then
    log "[gperftools] Already installed."
  else
    log "[gperftools] Cloning $GPERFTOOLS_GIT_DOWNLOAD..." && \
    git clone $GPERFTOOLS_GIT_DOWNLOAD && \
    cd gperftools && \
    log "[gperftools] Checkout $GPERFTOOLS_GIT_TAG..." && \
    git checkout $GPERFTOOLS_GIT_TAG 
    log "[gperftools] Running autogen.sh..." && \
    ./autogen.sh &>> $LOG_FILE
    log "[gperftools] Running autogen.sh..." && \
    ./autogen.sh &>> $LOG_FILE && configure_make_make_install "gperftools" && \
    cd ..
  fi
}

function install_ch_cpp_utils {
  if test -f /usr/local/lib/libch-cpp-utils.so; then
    log "[ch-cpp-utils] Already installed @ /usr/local/lib/libch-cpp-utils.so."
    ls -lh /usr/local/lib/libch-cpp-utils.so
  else
    log "[ch-cpp-utils] Cloning $CH_CPP_UTILS_GIT_DOWNLOAD..." && \
    git clone $CH_CPP_UTILS_GIT_DOWNLOAD &>> $LOG_FILE && \
    cd ch-cpp-utils && \
    log "[ch-cpp-utils] Running autogen.sh..." && \
    ./autogen.sh &>> $LOG_FILE && configure_make_make_install "ch-cpp-utils" && \
    cd ..
  fi
}

function install_ch_pi_control {
  if test -f /usr/local/bin/ch-pi-control; then
    log "[ch-pi-control] Already installed @ /usr/local/bin/ch-pi-control."
    ls -lh /usr/local/bin/ch-pi-control
  else
    log "[ch-pi-control] Cloning $CH_STORAGE_CLIENT_GIT_DOWNLOAD..." && \
    git clone $CH_STORAGE_CLIENT_GIT_DOWNLOAD &>> $LOG_FILE && \
    cd ch-pi-control && \
    log "[ch-pi-control] Running autogen.sh..." && \
    ./autogen.sh &>> $LOG_FILE && configure_make_make_install "ch-pi-control" && \
    cd ..
  fi
}

function install_supervisor {
  if apt list --installed | grep supervisor; then
    log "[supervisor] Already installed through apt."
  else
    log "[supervisor] Not found. Installing..."
    sudo apt -y install supervisor &>> $LOG_FILE
    log "[supervisor] Installed..."
  fi
}

function configure_supervisor {
  if test -f /etc/supervisor/conf.d/ch-pi-control.conf; then
    log "[supervisor] ch-pi-control.conf found."
    ls -l /etc/supervisor/conf.d/ch-pi-control.conf
  else
    log "[supervisor] ch-pi-control.conf not found. Copying..."
    sudo cp -v ../config/supervisor/ch-pi-control.conf /etc/supervisor/conf.d
    sudo supervisorctl reread && sudo supervisorctl update
  fi
}

cleanup && init && tools_install

# install_libevent
# install_glog
# install_gperftools
# install_ch_cpp_utils
# install_ch_pi_control
# install_supervisor
# configure_supervisor

install_libevent && \
	install_glog && \
	install_gperftools && \
	install_supervisor && \
	install_ch_cpp_utils
	install_ch_pi_control && \
	install_supervisor && \
	configure_supervisor && \
	cleanup && \
	end_installation

