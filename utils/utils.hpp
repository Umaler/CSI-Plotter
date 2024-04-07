#pragma once

#include <stdio.h>

class ConsoleMuter {
public:

    ConsoleMuter() = default;
    void mute();
    void unmute();
    bool isMuted();
    ~ConsoleMuter();

private:
    inline const static FILE default_stdout = *stdout,
                             default_stderr = *stderr;

    FILE *new_stdout = nullptr,
         *new_stderr = nullptr;
};
