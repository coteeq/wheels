include(ProcessorCount)

# TODO: hyper-threading
ProcessorCount(ProcCount)
message(STATUS "Processor count: ${ProcCount}")

add_definitions(-DPROC_COUNT=${ProcCount})

