
include(FetchContent)
FetchContent_Declare(battery
    GIT_REPOSITORY https://github.com/batterycenter/battery
    GIT_TAG        master
)
FetchContent_MakeAvailable(battery)


include(FetchContent)
FetchContent_Declare(banana
    GIT_REPOSITORY https://github.com/Smertig/banana
    GIT_TAG        v0.2.0
)
FetchContent_MakeAvailable(battery)