Requirements Post-Installation
------------------------------

  * Intel graphic drivers
  * sound drivers
  * synaptic packages
  *	Internet access via WiFi
  *	Working hotkeys - Fn, Brightness, Sound
  *	working audio
  * working external monitor
  * keyboard layout recognized
  *	Working Touchpad
  * Working USB keyboard/mouse
  *	Support for two batteries
  * Working GUI
  *	Browser
  *	gcc
  *	vim



Post-Installation
-----------------

###User Management ###

The following has commands to add a user and to grant sudo permissions to the user. A sample sudoers file is also provided
so that it may used.

#### Create a user ####

	# useradd -m -g [initial_group] -G [additional_groups] -s [login_shell] [username]

	# useradd -m -s /bin/bash sahil

While logged in as root, specify user password using `# passwd sahil`

#### Add sudo permissions ####

Run `sudo -ll` to print out the current sudo configuration

**Using visudo**
The configuration file for sudo is `/etc/sudoers`. It should always be edited with the `visudo` command. `visudo` locks the `sudoers` file, saves edits to a temporary file, and checks that file's grammar before copying it to `/etc/sudoers`.

**Sudoers File**

	##
	# Override Defaults
	##

	Defaults visiblepw, path_info, insults, lecture=always
	
	# Disable tty_tickets if you don't wish to login everytime you switch terminals.
	Defaults !tty_tickets

	# The following ensures that environment variables are passed to the root account
	# otherwise use sudo with -E flag set.
	Defaults env_keep += "ftp_proxy http_proxy https_proxy no_proxy"


	##
	# User specification
	##
	#to gain full user privileges
	USER_NAME ALL = (ALL) ALL
	

*find the rest of the sudoers file in the `www.gitub.com/altairmn/archinstall`*


** Sudoers default file permissions **

The owner and group for the sudoers file must both be 0. The file permissions must be set to 0440. These permissions are set by default, but if you accidentally change them, they should be changed back immediately or sudo will fail.

	# chown -c root:root /etc/sudoers
	# chmod -c 0440 /etc/sudoers


### Pacman Configuration ###

Enable color output in pacman. Uncomment the "Color" line in `pacman.conf`. 

You can also use alises for commonly used pacman commands. The alises can entered in the `.bashrc` file. An example is `alias pacupg='sudo pacman -Syu'`. To learn more
about pacman visit the [pacman][1] page in the [Arch Wiki][wiki]. You may also refer to the [Pacman Tips][2] page in the [Wiki][wiki].

### Graphics Driver Installation ###





[1]: https://wiki.archlinux.org/index.php/Pacman "Pacman"
[2]: https://wiki.archlinux.org/index.php/Pacman_tips
[wiki]: https://wiki.archlinux.org/	"Arch wiki"
