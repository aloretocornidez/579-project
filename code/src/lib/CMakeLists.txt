# adds this directory to the cmake project
add_library(lib "")


# adds the source files to the directory when compiling
target_sources(lib
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/account.cpp
    ${CMAKE_CURRENT_LIST_DIR}/edge.cpp
    ${CMAKE_CURRENT_LIST_DIR}/node.cpp
    ${CMAKE_CURRENT_LIST_DIR}/trafficCondition.cpp
    ${CMAKE_CURRENT_LIST_DIR}/vehicle.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/account.hpp
    ${CMAKE_CURRENT_LIST_DIR}/edge.hpp
    ${CMAKE_CURRENT_LIST_DIR}/node.hpp
    ${CMAKE_CURRENT_LIST_DIR}/trafficCondition.hpp
    ${CMAKE_CURRENT_LIST_DIR}/vehicle.hpp
  )

# adds the directory to the linker step of the compilation process
target_include_directories(lib
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )

