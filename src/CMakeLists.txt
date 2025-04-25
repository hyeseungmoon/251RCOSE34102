add_library(scheduler)

add_executable(COSE341 main.c)
target_link_libraries(COSE341 scheduler)