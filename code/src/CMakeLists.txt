set(PROJECT_NAME "campusNavigatorBuild")

add_executable(
  ${PROJECT_NAME}
  main.cpp
)

# adding lib directory for
add_subdirectory(lib)


target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE
  lib
)

