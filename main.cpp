// src/main.cpp

#include <iostream>
#include <windows.h>
#include <mmsystem.h> // NOVO: Para timeBeginPeriod
#include <cmath>
#include <thread> 
#include "ui.h"
#include "clicker.h"

// Garante que a função timeBeginPeriod seja usada
#pragma comment(lib, "winmm.lib") 

using namespace std;

bool g_isMouseHeld = false; 

int main() {
    
    // 🚨 ÚLTIMA SOLUÇÃO INCISIVA: Define a Resolução do Timer do Sistema para 1ms.
    // Isso garante que os sleeps sejam executados com a máxima precisão possível.
    timeBeginPeriod(1);
    
    // Define a prioridade do processo para TEMPO REAL.
    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
    
    // Garante que o timer seja resetado quando o programa terminar (mesmo em caso de erro)
    std::atexit([] {
        timeEndPeriod(1);
    });
    
    Config clickerConfig;
    
    PrintHeader();
    GetUserInput(clickerConfig);

    PrintHeader();
    
    while (true) {
        
        // A) VERIFICAÇÃO DE TOGGLE (Ligar/Desligar)
        if (CheckToggleKey(clickerConfig.keyBind)) {
            clickerConfig.enabled = !clickerConfig.enabled;
            if (!clickerConfig.enabled && g_isClicking.load()) {
                 ToggleClickerThread(clickerConfig);
            }
        }

        // B) VERIFICAÇÃO DE HOLD E INÍCIO DA THREAD
        if (clickerConfig.enabled && (GetAsyncKeyState(VK_LBUTTON) < 0)) {
            
            if (!g_isMouseHeld) {
                g_isMouseHeld = true; 
            }
            
            if (!g_isClicking.load()) {
                 ToggleClickerThread(clickerConfig);
            }
            
            int calculatedDelay = CalculateRandomDelay(clickerConfig.minCPS, clickerConfig.maxCPS);
            DisplayStatus(clickerConfig, (int)round(1000.0 / calculatedDelay));
            
            Sleep(1); 
            
        } else {
            // C) MOUSE SOLTO OU DESLIGADO
            
            if (g_isMouseHeld) {
                g_isMouseHeld = false;
                if (g_isClicking.load()) {
                    ToggleClickerThread(clickerConfig);
                }
            }
            
            DisplayStatus(clickerConfig, 0); 
            
            if (clickerConfig.enabled) {
                Sleep(1); 
            } else {
                Sleep(50);
            }
        }
    }
    
    return 0;
}