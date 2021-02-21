#file(GLOB ProtoFiles "${CMAKE_CURRENT_SOURCE_DIR}/*.proto")
PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders ${ProtoFiles})
add_library(messages STATIC ${ProtoSources} ${ProtoHeaders})
target_link_libraries(messages ${PROTOBUF_LIBRARY})
target_include_directories(messages PUBLIC ${CMAKE_CURRENT_BINARY_DIR})


protobuf_generate_python(PY_SOURCES ${ProtoFiles})
add_custom_target(py_protos ALL DEPENDS ${PY_SOURCES})

#
#  ====================================================================
#  Example:
#
#   set(NANOPB_SRC_ROOT_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/nanopb")
#   set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${NANOPB_SRC_ROOT_FOLDER}/extra)

#   find_package( Nanopb REQUIRED )
#   include_directories(${NANOPB_INCLUDE_DIRS})
#
#   NANOPB_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${ProtoFiles})
#   add_custom_target(emdedded_protos ALL DEPENDS ${PROTO_SRCS} ${PROTO_HDRS})
#
   #include_directories(${CMAKE_CURRENT_BINARY_DIR})
   #add_executable(bar bar.cc ${PROTO_SRCS} ${PROTO_HDRS})
#
#  Example with RELPATH:
#   Assume we have a layout like:
#    .../CMakeLists.txt
#    .../bar.cc
#    .../proto/
#    .../proto/foo.proto  (Which contains: import "sub/bar.proto"; )
#    .../proto/sub/bar.proto
#   Everything would be the same as the previous example, but the call to
#   NANOPB_GENERATE_CPP would change to:
#
#   NANOPB_GENERATE_CPP(PROTO_SRCS PROTO_HDRS RELPATH proto
#                       proto/foo.proto proto/sub/bar.proto)
#
#  ====================================================================