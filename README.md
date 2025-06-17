# Waola++
Waola++ is a cross-platform open source **Wa**ke-**o**n-**LA**N (aka [WoL](https://en.wikipedia.org/wiki/Wake-on-LAN)) utility with embedded [ARP](https://en.wikipedia.org/wiki/Address_Resolution_Protocol)-based network scanner.

The project comprises two core libraries: one for enumerating network interfaces and another providing WoL and ARP functionality and includes platform-agnostic command line tool as well as written with [Qt Quick](https://en.wikipedia.org/wiki/Qt_Quick)/[QML](https://en.wikipedia.org/wiki/QML) graphical user interface (GUI) frontend.

The project is designed to be built with a toolset naive for each supported platform, which are currently Linux, macOS and Windows.

## Prerequisites.
To build core libraries and CLI client besides C++23-compliant compiler and platform make system, you only need [CMake](https://cmake.org/) (version 3.16 or higher).

For the GUI frontend, you also need a [Qt](https://www.qt.io/) library (version 6.8 or higher).

## Building.
* Linux and macOS
    ```
    cd waolapp
    cmake -S . -B release
    cmake --build release -j
    ```
    + macOS [Berkeley Packet Filter](https://en.wikipedia.org/wiki/Berkeley_Packet_Filter).

        Long story short: run `./macOsBpf/bpf_test.sh`. If you'll see '`Successfully opened /dev/bpfX for both reading and writing ~> you're all set`' - you're all set, start enjoying Waola. Otherwise, run `sudo ./macOsBpf/install_daemon.sh` and ensure your user belongs to `admin` group.
        
        The matter is that network scanning with ARP requests is performed on macOS through Berkeley Packet Filter devices `/dev/bpf*`, and the OS by default not only restricts access to these devices to the `root` user only, but also restores this restriction on each reboot, so to use Waola scanning feature you need a) to set read-write permission for /dev/bpf* devices and b) restore this permission on each reboot. 

        There is a chance you have the permissions already adjusted (for example, [Wireshark](https://www.wireshark.org/) setup program can do this). To check the state of things, run `bpf_test.sh` from `macOsBpf` subdirectory. If you'll see '`Unable to open any BPF device for both reading and writing ~> you need to adjust /dev/bpf* permissions`', continue reading.

        There is an `install_daemon.sh` script in `macOsBpf` subdirectory. It ensures up to `$BPF_COUNT` `/dev/bpf*` devices, changes their permissions so that users belonging to 'admin' group can open them for both reading and writing and installs startup action which will do this on each reboot.

        To restore default permissions just run `uninstall_daemon.sh` script.  

* Windows
    ```
    cd waolapp
    cmake -S . -B release
    cmake --build release --config Release
    ```

## Installation.
* Linux and macOS
    ```
    sudo cmake --install release
    ```
* Windows

    As `Administrator`:
    ```
    cmake --install release
    ```

## Usage.
* waolac++.

    `waolac++ [mac_addr...]`

    , where `mac_addr...` is zero or more space-delimited MAC addresses. Bytes of MAC address can be delimited by `:`, `-` or can be not delimited at all, i.e. `01:23:45:67:89:AB`, `01-23-45-67-89-AB` and `0123456789AB` are all valid (and equal).

    Being started without arguments `waolac++` will scan network and print information about each discovered host in a form `<mac_addr>\t<ip_addr>\t<hostname>\n`.

    Started with MAC address(es) `waolac++` sends appropriate WoL 'magic packet'(s) through all up and running network interfaces.

    Exit code: 0 if all appropriate 'magic packets' were successfully sent through all up and running network interfaces, otherwise exit code.

* GUI frontend.

    Usage of GUI frontend is hopefully pretty intuitive. Two remarks about maybe not really obvious things:
    + double-click on a host sends it WoL 'magic packet'.
    + Wakeup result `Success` does not ensure that target host neither woke up nor even has received 'magic packet'; it only means that the packet has been successfully put on the wire.

## Uninstallation.
* Linux and macOS
    ```
    sudo cmake --build release --target uninstall
    ```
* Windows
    
    As `Administrator`:
    ```
    cmake --build release --target uninstall
    ```
____

The author of this project is a citizen of Ukraine who developed and maintains this software literally under attacks of russian missiles.

Also, I try to help our army to fight off the invasion as much as I can.

If this tool was useful to you, and you'd like to help us to defend our freedom and independence — feel free to donate any convenient for you amount to PayPal account [egggor@gmail.com](https://www.paypal.com/ua/home).
