set(CMAKE_CXX_STANDARD 14)
add_library(FunctionPass MODULE
        FunctionInfo.cpp
        )

set_target_properties(FunctionPass PROPERTIES
        COMPILE_FLAGS "-fno-rtti"
        )

if(APPLE)
    set_target_properties(FunctionPass PROPERTIES
            LINK_FLAGS "-undefined dynamic_lookup"
            )
endif(APPLE)
