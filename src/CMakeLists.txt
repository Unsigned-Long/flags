set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include_directories(include)

add_executable(${CMAKE_PROJECT_NAME} main.cpp)

set(CMAKE_INSTALL_PREFIX /usr/local)

install(FILES include/flags-v2.hpp DESTINATION include/artwork/flags)