set(PROJECT_NAME "campusNavigatorBuild")

set(SUB_DIR "db")

add_executable(
  ${PROJECT_NAME}
  main.cpp
  AStar.h
  AStar.cpp
)

# adding lib directory for
add_subdirectory(${SUB_DIR})


target_link_libraries( ${PROJECT_NAME} PRIVATE ${SUB_DIR})

