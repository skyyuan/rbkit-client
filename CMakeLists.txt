cmake_minimum_required(VERSION 3.2.2)

add_subdirectory(msgpack-c)
set (ZMQ_BUILD_TESTS OFF CACHE BOOLE "Turning off tests for ZeroMQ" FORCE)
add_subdirectory(zeromq-4.0.4)
add_subdirectory(nzmqt)
add_subdirectory(rbkit-lib)
add_subdirectory(rbkit-app)
add_subdirectory(tests)
