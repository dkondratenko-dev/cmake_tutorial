echo "Hello from Docker bash profile!"

alias b="build"

build() {
    #delete existing build folder, if it exists
    rm -rf build    

    #Create and build
    mkdir -p build
    cd build
    cmake ..
    make
}