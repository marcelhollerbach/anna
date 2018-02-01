# anna instructions
anna is a greeter for spawny.

Build with :

```
mkdir build

cd build

meson ..

ninja all

sudo ninja install
```

To make spawny start anna by default you have to call
```
sp-greeter-set <your-prefix>/bin/anna
```
Your prefix depends on what you have setted, the default is /usr/local
