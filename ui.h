// src/ui.h

#pragma once

#include <string>

struct Config {
    int keyBind = 0;
    int minCPS = 8;
    int maxCPS = 12;
    bool enabled = false;
};

void PrintHeader();
void GetUserInput(Config& config);
void DisplayStatus(const Config& config, int currentCPS);
int CaptureKey();