In order to get this to work:

1. Follow the guide below to install D2XX for Linux. Install the shared library

http://www.ftdichip.com/Drivers/D2XX.htm


Installing the D2XX shared library and static library.
------------------------------------------------------

1.  tar xfvz libftd2xx-x86_64-1.4.8.tgz

This unpacks the archive, creating the following directory structure:

    build
        libftd2xx        (re-linkable objects)
        libusb           (re-linkable objects)
        libftd2xx.a      (static library)
        libftd2xx.so.1.4.8   (dynamic library)
        libftd2xx.txt    (platform-specific information)
    examples
    libusb               (source code)
    ftd2xx.h
    WinTypes.h

2.  cd build

3.  sudo -s 
  or, if sudo is not available on your system: 
    su

Promotes you to super-user, with installation privileges.  If you're
already root, then step 3 (and step 7) is not necessary.

4.  cp libftd2xx.* /usr/local/lib

Copies the libraries to a central location.

5.  chmod 0755 /usr/local/lib/libftd2xx.so.1.4.8

Allows non-root access to the shared object.

6.  ln -sf /usr/local/lib/libftd2xx.so.1.4.8 /usr/local/lib/libftd2xx.so

Creates a symbolic link to the 1.4.8 version of the shared object.

7.  exit

Ends your super-user session.

2. Run "Make" in the "unix" folder

2a. You may need to copy ftd2xx.h and WinTypes.h from the "release" folder in libftd2xx-x86_64-1.4.8 to the "unix" folder in order to compile

3. Run "sudo ./spiTest (VID) (PID)" Note: You MUST be a super user for the program to properly run.


