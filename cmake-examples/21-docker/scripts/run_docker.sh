docker run -it --rm --name cpp-debug \
    -v $(pwd):/app \
    -p 9999:9999 \
    cpp-gtest-debug