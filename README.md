# anna
anna is a greeter for spawny.

## install
Build with :

mkdir build
cd build
meson ..
ninja all
sudo ninja install

This will install to /usr/local/ you can change the prefix as you like

To make spawny start anna by default you have to change the configuration file <spawny_prefix>/etc/spawny/spawny.ini, and change the property "cmd" to "<anna_prefix>/bin/anna-greeter"

After that the deadmon has to be restarted.
