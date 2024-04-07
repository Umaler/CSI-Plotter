#include "utils.hpp"
#include <iostream>

#if defined(_WIN32)
    #define NULL_DEVICE "NUL"
#else
    #define NULL_DEVICE "/dev/null"
#endif

void ConsoleMuter::mute() {
    if(isMuted())
        return;

    new_stdout = freopen(NULL_DEVICE, "w", stdout);
    //*stdout = *new_stdout;

    new_stderr = freopen(NULL_DEVICE, "w", stderr);
    //*stderr = *new_stderr;
}

void ConsoleMuter::unmute() {
    if(!isMuted())
        return;

    *stdout = default_stdout;
    fclose(new_stdout);
    new_stdout = nullptr;

    *stderr = default_stderr;
    fclose(new_stderr);
    new_stderr = nullptr;
}

bool ConsoleMuter::isMuted() {
    return new_stdout != nullptr;
}

ConsoleMuter::~ConsoleMuter() {
    unmute();
}
