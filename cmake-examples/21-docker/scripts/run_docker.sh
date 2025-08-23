#docker run -it --name cpp-debug -v $(pwd):/app -p 9999:9999 cpp-gtest-debug

#Run interactively(i) with pseudo-TTY (t - terminal). Remove (-rm) after it exits - cleans up after running. Attach volume (-v). Use port range(-p).
docker run -it --rm --name cpp-debug-container \
    -v $(pwd):/app \
    -p 9999:9999 \
    cpp-gtest-debug-image