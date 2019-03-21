# simple-zlib

## pre-requisites
1. TODO

## build and install --TEST
```bash
git clone --recurse-submodules https://github.com/jaskooner/simple-zlib.git
```

```bash
sh build.sh
```

```bash
g++ main.hpp lib.cpp \
  -I zlib-library/release/include \
  -L zlib-library/release/lib/ -lz \
  -o dist/deflate
 ```

## TODO
1. write up inflate method
