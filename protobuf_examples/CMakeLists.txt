# Minimum CMake required
cmake_minimum_required(VERSION 2.8.12)

# Project
# project(protobuf-examples)
# set(CMAKE_PREFIX_PATH "usr")
# Find required protobuf package

message(STATUS "Using Protocol Buffers ${Protobuf_VERSION}")


set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

foreach(example add_person list_people)
  set(${example}_SRCS ${example}.cc)
  set(${example}_PROTOS addressbook.proto)

  #Code Generation
  if(protobuf_MODULE_COMPATIBLE) #Legacy Support
    protobuf_generate_cpp(${example}_PROTO_SRCS ${example}_PROTO_HDRS ${${example}_PROTOS})
    list(APPEND ${example}_SRCS ${${example}_PROTO_SRCS} ${${example}_PROTO_HDRS})
  endif()

  #Executable setup
  set(executable_name ${example}_cpp)
  add_executable(${executable_name} ${${example}_SRCS} ${${example}_PROTOS})
  if(protobuf_MODULE_COMPATIBLE) #Legacy mode
    target_include_directories(${executable_name} PUBLIC ${PROTOBUF_INCLUDE_DIRS})
    target_link_libraries(${executable_name} ${PROTOBUF_LIBRARIES})
  else()
    target_link_libraries(${executable_name} protobuf::libprotobuf)
    target_include_directories(${executable_name} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
    protobuf_generate(TARGET ${executable_name})
    message(STATUS "CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}")
  endif()

endforeach()


set(PROTOBUF_INCLUDE_DIRS ${PROTOBUF_INCLUDE_DIRS} PARENT_SCOPE)
set(PROTOBUF_LIBRARIES ${PROTOBUF_LIBRARIES} PARENT_SCOPE)
set(PROTO_SRC ${PROTO_SRC} PARENT_SCOPE)
set(PROTO_HEADER ${PROTO_HEADER} PARENT_SCOPE)
