project(hello_world_cpp)

set(src
  ${src}
  hello_world.cc
  )

find_package (Threads)
add_library(hello_world_cpp SHARED ${src})
target_link_libraries(hello_world_cpp fluent-bit-shared)
target_link_libraries(hello_world_cpp ${CMAKE_THREAD_LIBS_INIT})
