echo "Hello from Docker bash profile!"

alias b="build"

build() {
    mkdir -p build
    cd build
    cmake ..
    make
}