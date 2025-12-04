// src/clicker.h

#pragma once

#include "ui.h" 
#include <random>
#include <thread>
#include <atomic>

extern std::mt19937 generator;
extern std::atomic<bool> g_isClicking;

// Declarando clickThread como externa para que main.cpp a encontre, se necessário.
extern std::thread clickThread; 

int CalculateRandomDelay(int minCPS, int maxCPS);
void SimulateClick(); 
bool CheckToggleKey(int keyBind);

void ClickerLoop(const Config& config);
void ToggleClickerThread(const Config& config);