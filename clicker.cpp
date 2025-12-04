// src/clicker.cpp

#include "clicker.h"
#include <windows.h>
#include <chrono> 
#include <cmath> 
#include <thread> 
#include <algorithm> 

// Variáveis globais de sincronização
std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
std::atomic<bool> g_isClicking(false); 
std::thread clickThread;

// Define o tipo de clock que usaremos para medição de tempo
using Clock = std::chrono::high_resolution_clock;
using Duration = std::chrono::duration<double, std::milli>; // Duração em milissegundos

// --- CalculateRandomDelay (SIMPLIFICADO: Apenas retorna o tempo de ciclo necessário) ---
int CalculateRandomDelay(int minCPS, int maxCPS) {
    std::uniform_int_distribution<> distrib(minCPS, maxCPS);
    double randomCPS = distrib(generator);
    std::uniform_real_distribution<> jitter(-0.4, 0.4);
    randomCPS += jitter(generator);

    if (randomCPS <= 0) randomCPS = 1;
    
    // Retorna o tempo total que o ciclo DEVE durar, sem compensação.
    int required_cycle_ms = (int)std::round(1000.0 / randomCPS);
    
    // O mínimo deve ser 1ms para evitar zero-division ou ciclos negativos
    return std::max(1, required_cycle_ms);
}

// --- SimulateClick (USANDO POSTMESSAGE) ---
void SimulateClick() {
    HWND curr_wnd = GetForegroundWindow();
    if (!curr_wnd) return;
    
    POINT pos {};
    if (!GetCursorPos(&pos)) return;

    ScreenToClient(curr_wnd, &pos); 

    PostMessage(curr_wnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(pos.x, pos.y));
    
    std::this_thread::sleep_for(std::chrono::milliseconds(1)); 

    PostMessage(curr_wnd, WM_LBUTTONUP, 0, MAKELPARAM(pos.x, pos.y));
}

// --- CheckToggleKey (DEFINIÇÃO NECESSÁRIA) ---
bool CheckToggleKey(int keyBind) {
    return (GetAsyncKeyState(keyBind) & 0x0001);
}

// --- ClickerLoop (DINÂMICO COM FEEDBACK DE TEMPO) ---
void ClickerLoop(const Config& config) {
    while (g_isClicking.load()) {
        
        int requiredCycleMs = CalculateRandomDelay(config.minCPS, config.maxCPS);
        
        auto start = Clock::now();

        SimulateClick();
        
        auto end = Clock::now();
        Duration executionTime = end - start;
        
        double sleepTimeMs = requiredCycleMs - executionTime.count();

        if (sleepTimeMs > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds((long long)sleepTimeMs));
        } else {
            std::this_thread::yield(); 
        }
    }
}

// --- ToggleClickerThread (DEFINIÇÃO NECESSÁRIA) ---
void ToggleClickerThread(const Config& config) {
    if (!g_isClicking.load()) {
        g_isClicking.store(true);
        clickThread = std::thread(ClickerLoop, std::ref(config)); 
        
    } else {
        g_isClicking.store(false);
        if (clickThread.joinable()) {
            clickThread.join(); 
        }
    }
}