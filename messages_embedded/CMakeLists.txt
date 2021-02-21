get_filename_component(ARDUINO_SKETCH_DIR ../Mqtt_Protobuf_Sketch ABSOLUTE)
get_filename_component(THIS_CMAKE_ROOT ../ ABSOLUTE)

#
#  ====================================================================
#  Example:
#
   set(NANOPB_SRC_ROOT_FOLDER "${THIS_CMAKE_ROOT}/nanopb")
   set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${NANOPB_SRC_ROOT_FOLDER}/extra)
   
   find_package( Nanopb REQUIRED )
   include_directories(${NANOPB_INCLUDE_DIRS})
#
   NANOPB_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${ProtoFiles})

   add_custom_target(emdedded_protos ALL 
      DEPENDS ${PROTO_SRCS} ${PROTO_HDRS}
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/addressbook.pb.c ${ARDUINO_SKETCH_DIR}/ 
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/addressbook.pb.h ${ARDUINO_SKETCH_DIR}/
      )

   message(STATUS ${ARDUINO_SKETCH_DIR})
   message(STATUS ${THIS_CMAKE_ROOT})
   message(STATUS ${CMAKE_CURRENT_BINARY_DIR})  

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
