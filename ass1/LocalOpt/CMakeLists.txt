set(CMAKE_CXX_STANDARD 14)
add_library(LocalOptPass MODULE
        LocalOpt.cpp
        )

set_target_properties(LocalOptPass PROPERTIES
        COMPILE_FLAGS "-fno-rtti"
        )

if(APPLE)
    set_target_properties(LocalOptPass PROPERTIES
            LINK_FLAGS "-undefined dynamic_lookup"
            )
endif(APPLE)
