# Copyright (c) 2020, TeCIP, Scuola Superiore Sant'Anna.
# astro7x@yandex.com
#
# This Dockerfile creates an image
# for CI: Continuous Integration
# and building for TI MCU
# AWRxxxx Family for mmWave FMCW
# RADAR development

# Starting on top of circleci's Ubuntu server
FROM circleci/ubuntu-server

LABEL maintainer="Abdelrahman Astro (astro7x@yandex.com)"

# Cache renew
RUN apt update

# Requires libc6
RUN dpkg --add-architecture i386

# Cache renew
RUN apt update

# Add for Java 1.8
RUN add-apt-repository ppa:openjdk-r/ppa -y && \
    apt update

# wget and curl for download
RUN apt install -y libc6:i386 \
    libusb-0.1-4 \
    libgconf-2-4 \
    build-essential \
    wget curl

# Required for CCS
RUN apt install -y libc6:i386 \
    libx11-6:i386 \
    libasound2:i386 \
    libatk1.0-0:i386 \
    libcairo2:i386 \
    libcups2:i386 \
    libdbus-glib-1-2:i386 \
    libgconf-2-4:i386 \
    libgcrypt20:i386 \
    libgdk-pixbuf2.0-0:i386 \
    libgtk-3-0:i386 \
    libice6:i386 \
    libncurses5:i386 \
    libncurses5:amd64 \
    libsm6:i386 \
    liborbit2:i386 \
    libudev1:i386 \
    libusb-0.1-4:i386 \
    libusb-0.1-4:amd64 \
    libstdc++6:i386 \
    libstdc++6 \
    libxt6 \
    libxt6:i386 \
    libxtst6:i386 \
    libgnomeui-0:i386 \
    libusb-1.0-0-dev:i386 \
    libcanberra-gtk-module:i386 \
    libcanberra-gtk-module:amd64 \
    gtk2-engines-murrine:i386 \
    libpython2.7 \
    openjdk-8-jdk \
    openjdk-8-jre \
    openjdk-8-jdk-headless \
    openjdk-8-jre-headless \
    unzip \
    libc6:amd64 \
    libexpat1:amd64 \
    libtinfo5:amd64 \
    zlib1g:amd64 \
    libgcc1:amd64 \
    base-files

# Get offline installer for version 9.
RUN wget https://software-dl.ti.com/ccs/esd/CCSv9/CCS_9_2_0/exports/CCS9.2.0.00013_linux-x64.tar.gz

# Move installation file to home
RUN mv $(ls | grep CCS) /home

# Change directory to home
WORKDIR /home

# Extract
RUN tar xvzf $(ls | grep CCS)

# Go to extracted dir and execute install bin
RUN cd $(ls | grep -v ".tar") && \
    chmod a+x $(ls | grep .bin) && \
   ./$(ls | grep .bin) --mode unattended --prefix /home/ti \
        --enable-components PF_MSP430,PF_CC2X, PF_C64MC, PF_TM4C, PF_MMWAVE \
        --install-BlackHawk false

# Set path including CCS eclipse
ENV PATH="/home/ti/ccs/eclipse:${PATH}"

# Install SDK
RUN wget https://cc2640-sdk.s3.amazonaws.com/simplelink_cc2640r2_sdk_3_10_00_15.run && \
    chmod a+x simplelink_cc2640r2_sdk_3_10_00_15.run && \
    ./simplelink_cc2640r2_sdk_3_10_00_15.run --mode unattended --prefix /home/ti && \
    rm simplelink_cc2640r2_sdk_3_10_00_15.run

# These environmental variables are required for Algebra's custom builds
ENV WORKSPACE_LOC="/home/workspace_angel"

ENV SIMPLELINK_SDK_LOC="/home/ti/simplelink_cc2640r2_sdk_3_10_00_15"

ENV CC2640_CUSTOM_FOLDER_LOC="${SIMPLELINK_SDK_LOC}/source/ti/blestack/boards"
ENV CC2640_CUSTOM_C_LOC="${SIMPLELINK_SDK_LOC}/source/ti/blestack/target/board.c"
ENV CC2640_CUSTOM_H_LOC="${SIMPLELINK_SDK_LOC}/source/ti/blestack/target/board.h"

ENV STACK_LOC="${SIMPLELINK_SDK_LOC}/examples/rtos/CC2640R2_LAUNCHXL/blestack/simple_peripheral_oad_offchip/tirtos/ccs/simple_peripheral_cc2640r2lp_oad_offchip_stack.projectspec"
ENV BIM_LOC="${SIMPLELINK_SDK_LOC}/examples/rtos/CC2640R2_LAUNCHXL/blestack/simple_peripheral_oad_offchip/tirtos/ccs/bim_oad_offchip_cc2640r2lp_app.projectspec"

# Expose bash
ENTRYPOINT /bin/bash
