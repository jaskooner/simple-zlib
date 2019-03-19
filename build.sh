git clone --recurse-submodules https://github.com/jaskooner/simple-zlib.git
cd zlib-library
sh build-zlib
cd ..

g++ main.cpp -out example