
# adds this directory to the cmake project
add_library(graph "")


# adds the source files to the directory when compiling
target_sources(graph
  PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/Edge.cpp 
  ${CMAKE_CURRENT_LIST_DIR}/Graph.cpp 
  ${CMAKE_CURRENT_LIST_DIR}/Node.cpp
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/Edge.h
  ${CMAKE_CURRENT_LIST_DIR}/Graph.h
  ${CMAKE_CURRENT_LIST_DIR}/Node.h
  )

# adds the directory to the linker step of the compilation process
target_include_directories(graph
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )

