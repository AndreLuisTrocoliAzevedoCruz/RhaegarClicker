// src/ui.cpp

#include "ui.h"
#include <iostream>
#include <windows.h>
#include <limits> // Para numeric_limits
#include <algorithm> // Para std::swap

using namespace std;

// --- Definições de Cores e Handler ---

// Handler para o console (necessário para SetConsoleTextAttribute)
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

// Definições de Cores (Combinações de R, G, B e Intensidade)
#define COLOR_DEFAULT 7 
#define COLOR_PURPLE FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY // Magenta/Purple
#define COLOR_GREEN FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define COLOR_RED FOREGROUND_RED | FOREGROUND_INTENSITY

// Função auxiliar para aplicar a cor
void SetColor(int color) {
    SetConsoleTextAttribute(hConsole, color);
}

// --- Funções Auxiliares ---

// Função para limpar a tela do console e reposicionar o cursor
void ClearScreenAndMoveCursor() {
    // Comando padrão para limpar a tela
    system("cls"); 
    // Mover o cursor para a posição inicial (0, 0)
    COORD cursorPosition = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}


// --- 1. PrintHeader (Roxo) ---
void PrintHeader() {
    ClearScreenAndMoveCursor();
    
    // Define a cor Roxo para o cabeçalho
    SetColor(COLOR_PURPLE); 
    
    // O SEU ASCII ART
    cout << " _____  _                                  _____ _ _      _          __      ____ " << endl;
    cout << "|  __ \\| |                                / ____| (_)    | |         \\ \\    / /_ |" << endl;
    cout << "| |__) | |__   __ _  ___  __ _  __ _ _ __| |    | |_  ___| | _____ _ _\\ \\  / / | |" << endl;
    cout << "|  _  /| '_ \\ / _` |/ _ \\/ _` |/ _` | '__| |    | | |/ __| |/ / _ \\ '__\\ \\/ /  | |" << endl;
    cout << "| | \\ \\| | | | (_| |  __/ (_| | (_| | |  | |____| | | (__|   <  __/ |   \\  /   | |" << endl;
    cout << "|_|  \\_\\_| |_|\\__,_|\\___|\\__, |\\__,_|_|   \\_____|_|_|\\___|_|\\_\\___|_|    \\/    |_|" << endl;
    cout << "                          __/ |                                                   " << endl;
    cout << "                         |___/                                                    " << endl;
    cout << "----------------------------------------------------------------------------------" << endl;
    cout << endl;

    // Reseta a cor para o padrão
    SetColor(COLOR_DEFAULT); 
}


// --- 4. CaptureKey (Auxiliar para GetUserInput) ---
// Função para capturar a próxima tecla pressionada pelo usuário
int CaptureKey() {
    // ">> Pressione a TECLA de ATIVACAO:" em Roxo
    SetColor(COLOR_PURPLE); 
    cout << ">> Pressione a TECLA de ATIVACAO: ";
    SetColor(COLOR_DEFAULT); 
    
    int key = 0;
    while (key == 0) {
        // Itera sobre os códigos de tecla virtual
        for (int i = 0x01; i <= 0xFF; ++i) { 
            // 0x8000 verifica se a tecla está pressionada no momento
            if (GetAsyncKeyState(i) & 0x8000) {
                key = i;
                Sleep(200); // Pequeno delay para evitar repetição acidental
                return key;
            }
        }
        Sleep(10); // Evita alto uso de CPU
    }
    return 0; 
}


// --- 2. GetUserInput (COM MENU DE OPÇÕES) ---
void GetUserInput(Config& config) {
    
    // 1. Tecla de Ativação
    config.keyBind = CaptureKey();
    
    // Mensagens em Roxo
    SetColor(COLOR_PURPLE);
    cout << ">> Tecla capturada: " << config.keyBind << endl;
    
    // --- NOVO MENU ---
    cout << endl << "Selecione a opcao que deseja:" << endl;
    // O texto deve refletir o CPS aplicado: 15-18
    cout << "1. Config Exe Legit" << endl; 
    cout << "2. Selecionar Manualmente" << endl;
    cout << "3. Config Exe Low KB" << endl; 
    cout << ">> Opcao (1, 2 ou 3): ";
    
    int choice = 0;
    SetColor(COLOR_DEFAULT);
    
    // CORREÇÃO 1: Adiciona a opção 2 e 3 à validação de escolha
    while (!(cin >> choice) || (choice < 1 || choice > 3)) {
        SetColor(COLOR_RED);
        cout << "Opcao invalida. Digite 1, 2 ou 3: ";
        SetColor(COLOR_DEFAULT);
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    // --- Lógica de Opções ---
    
    // CORREÇÃO 2: A condição lógica agora é correta para 1 ou 3
    if (choice == 1 || choice == 3) {
        // Opção 1 e 3: Configurações unidas, como solicitado
        config.minCPS = 15;
        config.maxCPS = 18;
        
        SetColor(COLOR_GREEN);
        cout << endl << "PERFIL SELECIONADO: " << (choice == 1 ? "Exe Legit" : "Exe Low KB") << " (15-18 CPS)" << endl;
        
    } else {
        // Opção 2: Selecionar Manualmente
        
        SetColor(COLOR_PURPLE);
        cout << endl << ">> Digite o CPS MINIMO (ex: 8): ";
        SetColor(COLOR_DEFAULT);
        
        // Loop de validação para CPS Mínimo
        while (!(cin >> config.minCPS) || config.minCPS <= 0) {
            SetColor(COLOR_RED);
            cout << "Entrada invalida. Digite um numero positivo: ";
            SetColor(COLOR_DEFAULT);
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        SetColor(COLOR_PURPLE);
        cout << ">> Digite o CPS MAXIMO (ex: 12): ";
        SetColor(COLOR_DEFAULT);

        // Loop de validação para CPS Máximo
        while (!(cin >> config.maxCPS) || config.maxCPS < config.minCPS) {
            SetColor(COLOR_RED);
            cout << "Entrada invalida. O Max deve ser maior ou igual ao Min (" << config.minCPS << "): ";
            SetColor(COLOR_DEFAULT);
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        
        SetColor(COLOR_PURPLE);
        cout << endl << "PERFIL SELECIONADO: Manual!" << endl;
    }

    // --- Finalização ---
    SetColor(COLOR_PURPLE);
    cout << endl << "CONFIGURACAO CONCLUIDA! Pressione qualquer tecla para INICIAR..." << endl;
    SetColor(COLOR_DEFAULT);
    
    // Aguarda o usuário para iniciar o loop principal
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpa o buffer de entrada
    cin.get();
}


// --- 3. DisplayStatus (Colorido) ---
void DisplayStatus(const Config& config, int currentCPS) {
    
    // Move o cursor para uma posição fixa (ex: linha 15) para evitar piscar o cabeçalho
    COORD statusPosition = {0, 15};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), statusPosition);
    
    // 1. Label "Status: " em Roxo
    SetColor(COLOR_PURPLE);
    cout << "Status: ";
    
    // 2. Valor **ON** (Verde) ou **OFF** (Vermelho)
    if (config.enabled) {
        SetColor(COLOR_GREEN);
        cout << "[ON]"; 
    } else {
        SetColor(COLOR_RED); // OFF em vermelho
        cout << "[OFF]"; 
    }
    
    // 3. Label " | CPS: " em Roxo
    SetColor(COLOR_PURPLE);
    cout << " | CPS: ";
    
    // 4. Valor CPS e Faixa em Roxo
    cout << currentCPS << " (" << config.minCPS << "-" << config.maxCPS << ")          " << endl;
    
    // 5. Label "Tecla: " em Roxo
    SetColor(COLOR_PURPLE);
    cout << "Tecla: " << config.keyBind << "          " << endl;
    
    // Reseta a cor para o padrão
    SetColor(COLOR_DEFAULT); 
}