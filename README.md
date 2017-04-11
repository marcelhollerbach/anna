# anna
anna is a greeter for spawny.

## install
Build with : 

cmake ..
make all
sudo make install 

This will install to /usr/local/ you can change the prefix as you like

To make spawny start anna by default you have to change the configuration file <spawny_prefix>/etc/spawny/spawny.ini, and change the property "cmd" to "<anna_prefix>/bin/anna-greeter"

After that the deadmon has to be restarted.
