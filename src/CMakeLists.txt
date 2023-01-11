set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include_directories(include)

add_executable(${CMAKE_PROJECT_NAME} main.cpp)

add_executable(${CMAKE_PROJECT_NAME}_self_def_opt self_def_option.cpp)

set(CMAKE_INSTALL_PREFIX /usr/local)

install(FILES include/flags.hpp include/arg_type.hpp DESTINATION include/artwork/flags)