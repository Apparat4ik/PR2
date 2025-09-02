#include "header.h"


vector<int> ContinedFraction(int numerator, int denom){ // числитель и знаменатель дроби
    vector<int> coefficients;
    int whole_part;
    while(denom != 0) {
        whole_part = numerator / denom;
        coefficients.push_back(whole_part);
        
        double reminder = numerator % denom;
        
        numerator = denom;
        denom = reminder;
    }
    return coefficients;
}



int GCD(int a, int b) {
    while (b != 0) {
        int r = b;
        b = a % b;
        a = r;
    }
    
    return a;
}


void Tests_Fraction(){
    vector<int> fr = {1, 2, 14, 9, 1, 2};
    assert(ContinedFraction(1256, 847) == fr);
    assert(GCD(1256, 847) == 1);
}


void Solution(int a, int b, int d){
    Tests_Fraction();
    
    if (GCD(a, b) != 1){
        cerr << "Нет решений" << endl;
    }
    vector<int> cf= ContinedFraction(a, b);
    
    int P_prev = 0, Q_prev = 1;
    int P = 1, Q = 0;
    int P_next, Q_next;
    
    int k = 0;
    for (int q : cf){
        P_next = q * P + P_prev;
        Q_next = q * Q + Q_prev;
        
        P_prev = P; Q_prev = Q;
        P = P_next; Q = Q_next;
        k++;
    }
    int x = Q_prev * pow(-1, k - 1) * d;
    int y = -(P_prev * pow(-1, k - 1) * d);
    cout << "Частное решение: " << "a = " << x << ", b = " << y << endl;
    cout << "Общее решение: " << "a = " << x << " + " << b << "t" << ' '
                              << ",b = " << y << " - " << a << "t" << endl;
    cout << "t принадлежит Z" << endl;
}




void PrintSolution(){
    cout << "Исходное уравнение: " << endl;
    int a = 1256, b = 847, d = 119;
    cout << a << "а" << " + " << b << "б" << " = " << d << endl;
    
    cout << "Разложение дроби " << a << '/' << b << ':' << endl;
    for (int it : ContinedFraction(a, b)){
        cout << it << ' ';
    }
    cout << endl;
    Solution(a, b, d);
}


