Tutorial from Function.fr
-------------------------

### Network

Using wired network for simplicity.

	ip link set enp0s25 up  


Interface name should be enp0s25, but `ip a` should show you all interfaces if necessary.

If you need to go the wifi way even for installation, see [the wiki][1].

### Filesystem setup

We create partitions. The important note is that we need a specific partition for the [UEFI boot loader][2]. Because the X240 has a [UEFI bios][3].

	# erase all partitions, byebye Windows
	sgdisk --zap-all /dev/sda

	# partition your disk
	cgdisk /dev/sda
	# create 3 partitions:
	# 1. EFI system 512.0 MB
	# 2. Linux filesystem 230.0 GiB
	# 3. Linux Swap 8.0 GiB

	# create the filesystems
	mkfs.fat -F32 /dev/sda1 # EFI system partition
	mkfs.ext4 /dev/sda2 # Linux filesystem partition
	mkswap /dev/sda3 && swapon /dev/sda3 # Linux Swap

	# mount partitions
	mount /dev/sda2 /mnt
	mkdir /mnt/boot
	mount /dev/sda1  /mnt/boot


EFI partition needs to be [at least 512.0 MB][4].

### Pacman repository list update

We get a fresh list of European servers for pacman. So that downloads are fast.

	# update pacman mirror list with fresh European servers, then uncomment every `#Server` line to activate the servers
	curl -o /etc/pacman.d/mirrorlist "https://www.archlinux.org/mirrorlist/?country=FR&country=DE&country=IE&country=LU&country=CH&country=GB&protocol=http&ip_version=4&ip_version=6&use_mirror_status=on" && perl -pi -e 's/#Server/Server/g' /etc/pacman.d/mirrorlist

If you are not into the "copy paste this line in your root system" way, you can decompose this command and do eveything by yourself.


### System installation

Now our partition and filesystems are ready. Install!

	# install base-devel group packages
	# https://www.archlinux.org/groups/x86_64/base-devel/
	pacstrap -i /mnt base base-devel
	genfstab -U -p /mnt >> /mnt/etc/fstab
	arch-chroot /mnt /bin/bash

	# activate en_US.UTF-8 as a locale to generate
	perl -pi -e 's/^#en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/g' /etc/locale.gen
	locale-gen
	echo LANG=en_US.UTF-8 > /etc/locale.conf
	export LANG=en_US.UTF-8

	ln -s /usr/share/zoneinfo/Europe/Paris /etc/localtime
	# Change Europe/Paris to something relevant to you

	hwclock --systohc --utc

	# choose a hostname and set it
	echo superarch > /etc/hostname

	# add your hostname at the end of /etc/hosts, right after first line `localhost`
	nano /etc/hosts

	systemctl enable dhcpcd@enp0s25.service

	mkinitcpio -p linux

	# change root password
	passwd

	# install UEFI boot loader
	pacman -S dosfstools refind-efi  
	refind-install

	# exit chroot
	exit

	# unmount and reboot
	umount -r /mnt  
	reboot  

You should now have an empty but working archlinux installation.

Console login should work and you get to a shell.

[1]: https://wiki.archlinux.org/index.php/Beginners%27_guide#Wireless
[2]: https://wiki.archlinux.org/index.php/Beginners%27_guide#For_UEFI_motherboards
[3]: http://fr.wikipedia.org/wiki/Unified_Extensible_Firmware_Interface
[4]: https://wiki.archlinux.org/index.php/Beginners%27_guide#For_UEFI_motherboards

* * *

Arch Linux on Thinkpad X240
---------------------------

What do I need after installation:

  * Installation
    + UEFI works									[DONE]
  	+ Kernel works
  	+ Recognizes hardware
  	+ Swap problems solved							[DONE]
  	+ Intel graphic drivers
  	+ sound drivers
  	+ synaptic packages
  *	Internet access via WiFi
  *	Working hotkeys - Fn, Brightness, Sound
  *	working audio
  * working external monitor
  * keyboard layout recognized						[P-DONE]
  *	Working Touchpad
  * Working USB keyboard/mouse
  *	Support for two batteries
  * Working GUI
  *	Browser
  *	gcc
  *	vim

Dealing with Issues
-------------------

###Installation

####System Requirements####
  - x86_64 machine 		[DONE]
  - minimum 64 MB RAM 	[DONE]
  - 800 MB disk space	[DONE]

####Prepare Installation Medium####
  - Check MD5 for download integrity	[DONE]
  - 3.0 USB Pendrive					[DONE]

Information

  - Current Release: 2015.01.01
  - Included Kernel: 3.17.6
  - ISO Size: 587.0 MB

####Booting Installation Medium####

In boot menu, select "Arch Linux archiso x86_64 UEFI"

####Testing if booted in UEFI####

`# efivar -l` displays UEFI variables. This means you have booted into UEFI mode.

####Establishing Internet Connection####

Use wifi-menu to setup the internet connection
To use with proxy server, export the `http_proxy` and `ftp_proxy` environment variables.

####Disk Partitioning####

First recreate the partition table using the following commands.

	# parted /dev/sdx
	(parted) mklabel gpt

Now decide the partition scheme to use. Since we are using the UEFI mode, we must have an ESP (EFI System Partition).
The recommended size of an ESP is `512 MB`.  
Note that atleast a partition for `/` (root) directory *must* be made.

We will be using the *UEFI/GPT* partitioning scheme.

The partition scheme I'll use is:
  - 512 MB EFI partition
  - 6 GB swap partition
  - Rest for `/` root partition

	# parted /dev/sdx
	(parted) mkpart ESP fat32 1M 513M
	(parted) set 1 boot on
	(parted) mkpart primary linux-swap 513M 6.6G
	(parted) mkpart primary ext4 6.6G 100%

####Create Filesystems####

To view the partitions on your device, use the following command
	
	# lsblk /dev/sdx

Create the filesystems

	# mkfs.vfat -F32 /dev/sdxY
	# mkfs.ext4 /dev/sdxY
	# mkswap /dev/sdxY
	# swapon /dev/sdxY

#### Mount the partitions ####

The `/` (root) partition must be mounted first: this is because any directories such as `/boot`
or `/home` that have separate partitons will have to be created in the root file system.
The /mnt directory of the live system will be used to mount the root partition, and consequently
all the other partitions will stem from there. If the root partition's name is `sdxR`, do:

	# mount /dev/sdxR /mnt

Create mount point and mount the other partitions.

	# mkdir -p /mnt/boot
	# mount /dev/sdxB /mnt/boot


### Installation ###

#### Select mirror ####

I've already checked the mirrorlist and as of 17.01.2015, it is updated and working as desired.


#### Install & configure base system ####

The base system is installed using the *pacstrap* script. If you want to install every package without being prompted,
the -i switch should be omitted. The base-devel package is installed if you wish to build packages from AUR or with ABS.

	# pacstrap -i /mnt base base-devel

#### Generate an fstab ####

UUID's are used because they have certain advantages. Also note that the fstab file must always be checked after
generating it.

	# genfstab -U -p /mnt >> /mnt/etc/fstab
	# nano /mnt/etc/fstab

The `<pass>` field shoudl have `1` for the root partition and `2` for other partitions. Normally, you will also want
your swap partition to have `0`.

Example fstab file:

	# <file system>		<dir> 	<type> <options>                                                                                            <dump> <pass>
	UUID				/boot 	vfat   rw,relatime,fmask=0022,dmask=0022,codepage=437,iocharset=iso8859-1,shortname=mixed,errors=remount-ro 0      2
	UUID				/	 	ext4   rw,relatime,discard,data=ordered																		0      1
	UUID				none  	swap   defaults																								0      0

#### Chroot and configure the base system ####

	# arch-chroot /mnt /bin/bash

#### Locale ####

	# perl -pi -e 's/^#en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/g' /etc/locale.gen
	# locale-gen
	# echo LANG=en_US.UTF-8 > /etc/locale.conf
	# export LANG=en_US.UTF-8

#### Time Zone ####

ln -s /usr/share/zoneinfo/Asia/Kolkata /etc/localtime

#### Hardware clock ####

# hwclock --systohc --utc

#### Kernel Modules ####

  - To check your current kernel version, use the command `uname -r` to get your current kernel release version.
  - To show what kernel modules are currently loaded `lsmod`
  - To show information about a module `modinfo module_name`
  - to list the options that are set for a loaded module `systool -v -m module_name`
  - to display the comprehensive configuration of all modules `modprobe -c | less`

#### Hostname ####

Set the hostname to your liking
	
	# echo altairmn > /etc/hostname

Add the same hostname to `/etc/hosts`:

	#<ip-address> <hostname.domain.org> <hostname>
	127.0.0.1 localhost.localdomain localhost altairmn
	::1   localhost.localdomain localhost altairmn


#### Configure Wireless ####


##### Check the driver status #####

To check if the driver for your card has been loaded, check the output of the `lspci -k` for PCI(e). You should see that some kernel driver is in use, for example:

	$ lspci -k

	06:00.0 Network controller: Intel Corporation WiFi Link 5100
	        Subsystem: Intel Corporation WiFi Link 5100 AGN
	        Kernel driver in use: iwlwifi
	        Kernel modules: iwlwifi

Also check the output of `ip link` command to see if a wireless interface (usually it starts with the letter "w", e.g. `wlp2s1`) was created. Then bring the interface up with `ip link set _interface_ up`. For example, assuming the interface is `wlan0`:

	# ip link set wlan0 up

If you get this error message: `SIOCSIFFLAGS: No such file or directory`, it most certainly means that your wireless chipset requires a firmware to function.

Check kernel messages for firmware being loaded:

	$ dmesg | grep firmware
	[   7.148259] iwlwifi 0000:02:00.0: loaded firmware version 39.30.4.1 build 35138 op_mode iwldvm


If there is no relevant output, check the messages for the full output for the module you identified earlier (`iwlwifi` in this example) 
to identify the relevant message or further issues:

	$ dmesg | grep iwlwifi

	[   12.342694] iwlwifi 0000:02:00.0: irq 44 for MSI/MSI-X
	[   12.353466] iwlwifi 0000:02:00.0: loaded firmware version 39.31.5.1 build 35138 op_mode iwldvm
	[   12.430317] iwlwifi 0000:02:00.0: CONFIG_IWLWIFI_DEBUG disabled
	...
	[   12.430341] iwlwifi 0000:02:00.0: Detected Intel(R) Corporation WiFi Link 5100 AGN, REV=0x6B


If the kernel module is successfully loaded and the interface is up, you can skip the next section.

**Note**: Security used by IITH is `WPA2-Personal`


#### Setting up the network ####

Assuming that the required driver and firmware is installed and the wireless network card is working, we can proceed to configure the network for automatic connect.


First install *iw* and *wpa_supplicant* which will be needed to connect to a network `# pacman -S iw wpa_supplicant`

We will use manual netctl profiles. Edit the profile as needed (modify `Interface`, `ESSID`, `Key`)

	# cd /etc/netctl
	# cp examples/wireless-wpa my-network
	# nano my-network
	# netctl enable my-network

The last line enables the profile to start at every boot.


#### Create and Initial Ramdisk Environment ####

Most users can skip this step. If edits need to be made, edit `/etc/mkinitcpio.conf` as needed and re-generate the initramfs image with:

	# mkinitcpio -p linux

#### Set the Root Password ####

Set the password with

	# passwd

#### Install and Configure a Bootloader ####

**For UEFI motheboards**

Using *gummiboot*, we do the following:
	
	# pacman -S dosfstools efibootmgr
	# pacman -S gummiboot
	# gummiboot --path=$esp install

Gummiboot will automatically be detected by firmware that requires that the bootable `bootx64.efi` stub be placed in `$esp/EFI/boot`, 
and will in turn automatically detect the presence of any other installed operating systems using *.efi* stubs. 
However, it will still be necessary to manually create a configuration file for Gummiboot.

First, create `$esp/loader/entries/arch.conf` and add the following, replacing `/dev/sdaX `with your root partition (e.g. `/dev/sda1`):

 	# nano $esp/loader/entries/arch.conf

	title          Arch Linux
	linux          /vmlinuz-linux
	initrd         /initramfs-linux.img
	options        root=/dev/sdaX rw

Second, create `$esp/loader/loader.conf` and add the following, replacing the timeout value (in seconds) with your own choice:

	# nano $esp/loader/loader.conf

	default  arch
	timeout  5

**Note:** In our install, we have set `$esp` to `/boot`.

#### Unmount the partitions and reboot ####

Exit from the chroot environment:

	# exit

Optionally unmount the partition, even though systemd automatically does it on shutdown. Unmount manually with `umount -R /mnt` as a safety measure.

Then reboot the computer:

	# reboot

