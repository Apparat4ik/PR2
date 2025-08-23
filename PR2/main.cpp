#include "header.h"


void ParseCommand(const string& cmd, Sellection& choice) {
    if (cmd == "AXMODP") choice = Sellection::AXMODP;
    
    else if (cmd == "CLASSIC") choice =  Sellection::CLASSIC;
    else if (cmd == "LOG") choice = Sellection::LOG;
    
    else if (cmd == "EUCLID") choice = Sellection::EUCLID;
    
    else if (cmd == "ELGAMAL") choice = Sellection::ELGAMAL;
    
    else if (cmd == "ATACK") choice = Sellection::ATACK;
    else if (cmd == "NOATACK") choice =  Sellection::NOATTACK;
    
    else if (cmd == "CONFRACT") choice =  Sellection::CONFRACT;
    
    else if (cmd == "BACK") choice = Sellection::BACK;
    else if (cmd == "EXIT") choice = Sellection::EXIT;
    else choice = Sellection::UNKNOWN;
}



int main() {
    Sellection choice;
    string cmd;
    
    cout << "Что вы хотите сделать?" << endl;
    cout << "AXMODP - Сравнение чисел вида a^x mod p" << endl;
    cout << "EUCLID - Алгоритм Евклида для вычисления обртного элемента" << endl;
    cout << "ELGAMAL - Алгоритм шифрования текста Эль-Гамаля" << endl;
    cout << "CONFRACT - Решение уравнения через цепную дробь" << endl;
    cout << "EXIT - Выход из программы" << endl;
    
    bool ch1 = true, ch2 = true;
    
    while (ch1) {
        cout << "Выберите програаму" << endl;
        
        cout << ">>> ";
            
        cin >> cmd;
            
        ParseCommand(cmd, choice);
            
        cout << endl;

        switch (choice) {
            case Sellection::AXMODP:
                cout << "Каким образом вы хотите сравнить числа?" << endl;
                cout << "CLASSIC - Классический способ" << endl;
                cout << "LOG - Раздложение через логарифм" << endl;
                cout << "BACK - Назад к выбору программ" << endl;
               
                
                while(ch2){
                    cout << "Введите версию выбранной программы" << endl;
                    cout << ">>> ";
                                   
                    cin >> cmd;
                                   
                    ParseCommand(cmd, choice);
                                   
                    cout << endl;
                    
                    switch (choice) {
                        case Sellection::CLASSIC:
                            Classic_Solve();
                            break;
                            
                        case Sellection::LOG:
                            Log_solve();
                            break;
                            
                        case Sellection::BACK:
                            ch2 = false;
                            break;
                            
                        default:
                            cerr << "Неверная команда" << endl;
                            break;
                    }
                }
                ch2 = true;
                break;
                
                
            case Sellection::EUCLID:
                EuclidSolve();
                break;
                
            case Sellection::ELGAMAL:
                cout << "Как вы хотите запустить программу шифрования?" << endl;
                cout << "ATACK - С демонстрацией атаки" << endl;
                cout << "NOATACK - Без атаки" << endl;
               
                while (ch2) {
                    cout << "Выберите версию программы" << endl;
                    cout << ">>> ";
                                   
                    cin >> cmd;
                                   
                    ParseCommand(cmd, choice);
                                   
                    cout << endl;
                    
                    switch (choice) {
                        case Sellection::ATACK:
                            LaunchWithAtack();
                            break;
                            
                        case Sellection::NOATTACK:
                            LaunchSypher();
                            break;
                            
                        case Sellection::BACK:
                            ch2 = false;
                            break;
                            
                        default:
                            cerr << "Неверная команда" << endl;
                            break;
                    }
                }
                ch2 = true;
                break;
                
            case Sellection::CONFRACT:
                PrintSolution();
                break;
                
            case Sellection::EXIT:
                ch1 = false;
                break;
                
            default:
                cerr << "Неверная команда" << endl;
                break;
        }
    }
    
    return 0;
}
