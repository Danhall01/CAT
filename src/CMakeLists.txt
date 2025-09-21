file(GLOB_RECURSE SOURCE_FILES "*.c" "${CMAKE_CURRENT_SOURCE_DIR}/src")

message("Paring src files automatically:")
foreach(f ${SOURCE_FILES})
    message("\t>${f}")
endforeach()
target_sources(${LIB}
    PRIVATE
        ${SOURCE_FILES}
)
