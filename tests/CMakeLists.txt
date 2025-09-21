 function (create_test name file)
    add_executable(${name} ${file})
    target_link_libraries(${name} ${LIB})
    add_test(NAME ${name} COMMAND $<TARGET_FILE:${name}>)
endfunction()

create_test(Valid_c main.c)
create_test(Valid_cpp main.cpp)
