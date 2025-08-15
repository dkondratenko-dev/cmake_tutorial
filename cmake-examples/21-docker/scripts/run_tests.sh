# docker run --rm \
#     -v $(pwd):/app \
#     cpp-gtest-debug \
#     /bin/bash -c "cd /app/build && ctest --output-on-failure > /app/test_results.log 2>&1"

docker run --rm \
    -v $(pwd):/app \
    cpp-gtest-debug \
    /bin/bash -c "cd /app/build && ctest --output-on-failure"
