include(../FindMosquittopp.cmake)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Werror -c )

add_executable(cpp main.cpp cpp.cpp)
target_link_libraries(cpp ${MOSQUITTOPP_LIBRARY} fmt messages )
target_include_directories(cpp PRIVATE ${INCLUDE_DIRECTORY} ${MOSQUITTOPP_INCLUDE_DIR})


add_executable(add_person add_person.cc )
target_link_libraries(add_person ${MOSQUITTOPP_LIBRARY} fmt messages  )
target_include_directories(add_person PRIVATE ${INCLUDE_DIRECTORY} ${MOSQUITTOPP_INCLUDE_DIR})
