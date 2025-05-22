add_executable(COSE341 main.c)

add_library(queue DataStructure/priority_queue.c DataStructure/fifo_queue.c)
add_library(component Component/cpu.c Component/process.c)
add_library(scheduler Scheduler/scheduler_interface.c Scheduler/scheduler.c)

target_link_libraries(scheduler PRIVATE queue component)

target_link_libraries(COSE341 queue component scheduler)
