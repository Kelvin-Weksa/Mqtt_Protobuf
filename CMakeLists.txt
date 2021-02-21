cmake_minimum_required(VERSION 3.6 FATAL_ERROR)
project(mosquitto VERSION 0.1.0 LANGUAGES C CXX)

find_package(Protobuf REQUIRED)
find_package(Python REQUIRED)

file(GLOB ProtoFiles "${CMAKE_CURRENT_SOURCE_DIR}/protocol_buffers/*.proto")


add_subdirectory(nanopb)
add_subdirectory(messages)
add_subdirectory(messages_embedded)
add_subdirectory(fmt)
add_subdirectory(protobuf_examples)
add_subdirectory(cpp)


