#include "header.h"
#include <map>
#include <chrono>
#include <fstream>


random_device rd;
mt19937 gen(rd());

map<char, uint64_t> Public_Key = {};

uint64_t Private_Key;

uint64_t Session_Key;

vector<int> pr_num;



ostream& operator<<(ostream& os, vector<pair<uint64_t, uint64_t>> vc){
    for (pair<uint64_t, uint64_t> pr : vc){
        os << '(' << pr.first << ' ' << pr.second << ')' << ' ';
    }
    return os;
}


vector<int> Sieve(int n) {                 // нахождение простых чисел через решето Эратосфена
    vector<bool> is_prime(n + 1, true);
    
    for (int i = 2; i <= n; i++)
        if (is_prime[i])
            for (int j = 2*i; j <= n; j += i)
                is_prime[j] = false;
    
    for (int i = 2; i <= n; i++){
        if (is_prime[i]){
            pr_num.push_back(i);
        }
    }
    return pr_num;
}


// Быстрое возведение в степень по модулю
uint64_t Mod_pow(uint64_t base, uint64_t exponent, uint64_t modulus) {
    if (modulus == 1) return 0;

    uint64_t result = 1;
    base %= modulus;

    while (exponent > 0) {
        if (exponent & 1) {
            result = (result * base) % modulus;
        }
        base = (base * base) % modulus;
        exponent >>= 1;
    }

    return result;
}



vector<pair<uint64_t, uint64_t>> Decomposition(uint64_t n) {   // каноническое рахложение числа
    vector<pair<uint64_t, uint64_t>> dcompsitn;
    for (int i = 2; i <= sqrt(n); i++) {
        if (n % i == 0) {
            int count = 0;
            while (n % i == 0) {
                count++;
                n /= i;
            }
            dcompsitn.push_back({i, count});
        }
    }
    if (n > 1) {
        dcompsitn.push_back({n, 1});
    }
    return dcompsitn;
}



// Тест Миллера для проверки простоты
bool MillerTest(uint64_t n, int t) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;

    // Факторизуем n-1
    auto factors = Decomposition(n - 1);

    // Генератор случайных чисел
  
    uniform_int_distribution<uint64_t> dis(2, n - 1);

    // Выполняем t проверок
    vector<uint64_t> randNum;
    for (uint64_t test = 0; test < t; ++test) {
        uint64_t a = dis(gen);
        randNum.push_back(a);
    }
    for (auto a : randNum) {
        // Проверка малой теоремы Ферма
        if (Mod_pow(a, n - 1, n) != 1) {
            return false;
        }
    }

    // Дополнительные проверки для каждого простого множителя

    for (const auto& factor : factors) {
        bool all_equal_one = true;
        for (auto a : randNum) {
            uint64_t exp = (n - 1) / factor.first;
            uint64_t result = Mod_pow(a, exp, n);

            if (result != 1) {
                all_equal_one = false;
                break;
            }
        }
        if (all_equal_one) {
            return false;
        }
    }

    return true;
}


uint64_t P_Gen(int k, const int& t) {
    uint64_t n_min = 1 << (k - 2), n_max = (1 << (k - 1)) - 1;
    uint64_t p = 0, m = 1;
    while (!MillerTest(p, t)) {
        m = 1;
        while (!(n_min < m && m < n_max) && (m > 0)) {
            int primeIt = gen() % pr_num.size();
            uint64_t primeExp = gen() % 3;
            m *= pow(pr_num[primeIt], primeExp);
            while (m >= n_max) {
                m /= pow(pr_num[primeIt], primeExp);
                if (primeIt - 1 < 0) {
                    break;
                }
                m *= pow(pr_num[--primeIt], primeExp);
            }
        }
        p = 2 * m + 1;
    }
    
    return p;
}



bool G_is_PrimitiveRoot(uint64_t g, uint64_t p){
    if (gcd(g, p) != 1){return false;}
    if (Mod_pow(g, (p - 1) / 2, p) == 1){return false;}
    
    uint64_t L = 0;
    for (pair<uint64_t, uint64_t> factor : Decomposition(p - 1)){
        L = (p - 1) / factor.first;
        if (Mod_pow(g, L, p) == 1){return false;}
    }
    return true;
}


uint64_t G_Gen(uint64_t p){
    uint64_t g = 0;
    do {
        g = gen() % (p - 1);
    } while (!G_is_PrimitiveRoot(g, p));
    return g;
}

void GenKeys(int k, int t){
    
    uint64_t p = P_Gen(k, t);
    cout << "Число p: " << p << endl;
    
    uint64_t g = G_Gen(p);
    cout << "Число g: " << g << endl;
    
    uint64_t x = 1 + (gen() % (p - 2));
    cout << "Число x: " << x << endl;
    
    uint64_t y = Mod_pow(g, x, p);
    cout << "Число y: " << y << endl;
    
    Public_Key['y'] = y;
    Public_Key['g'] = g;
    Public_Key['p'] = p;
    
    Private_Key = x;
}



void Encryption(string& plain_text, const string& filename){
    ofstream file(filename, ios::binary);
    
    if (!file.is_open()) {
            throw runtime_error("Ошибка: Не удалось открыть файл для записи: " + filename);
    }
    
    for (uint8_t ltr : plain_text){
        uint64_t a = Mod_pow(Public_Key['g'], Session_Key, Public_Key['p']);
        uint64_t b1 = Mod_pow(Public_Key['y'], Session_Key, Public_Key['p']);
        uint64_t b2 = Mod_pow(ltr, 1, Public_Key['p']);
        uint64_t b = Mod_pow(b1 * b2, 1, Public_Key['p']);

        file << a << ' ' << b << ' ';
    }
    plain_text.clear();
    
    file.close();
 }




string M_to_text(uint64_t& m, string& plain_text){
    string m_str = to_string(m);
    for (int i = 0; i < m_str.size(); i += 3){
        
        string ltr;
        ltr.push_back(m_str[i]);
        ltr.push_back(m_str[i + 1]);
        ltr.push_back(m_str[i + 2]);
        
        plain_text += stoi(ltr);
    }
    return plain_text;
}


vector<uint64_t> ReadCypher(const string& filename){
    ifstream file_cph(filename, ios::binary);
    
    if (!file_cph.is_open()) {
            throw runtime_error("Ошибка: Не удалось открыть файл для чтения: " + filename);
    }
    
    vector<uint64_t> cyph_nums;
    uint64_t number;
    while (file_cph >> number){
        cyph_nums.push_back(number);
    }
    file_cph.close();
    return cyph_nums;
}

void Decryption(string& plain_text, const string& filename){
    vector<uint64_t> c_text = ReadCypher(filename);
    
    for (int i = 0; i < c_text.size(); i += 2){
        uint64_t m1 = Mod_pow(c_text[i], Public_Key['p'] - 1 - Private_Key, Public_Key['p']);
        uint64_t m2 = Mod_pow(c_text[i + 1], 1, Public_Key['p']);
        uint64_t m = Mod_pow(m1 * m2, 1, Public_Key['p']);
        plain_text += m;
    }
}


void DemonstrateAttack() {
    cout << "\n=== АТАКА НА ЗАКРЫТЫЙ КЛЮЧ ===" << endl;
    
    uint64_t p = Public_Key['p'];
    uint64_t g = Public_Key['g'];
    uint64_t y = Public_Key['y'];
    
    // Решаем y = g^x mod p
    cout << "Решаем задачу дискретного логарифмирования..." << endl;
    uint64_t found_x = 0;
    
    
    clock_t t_start = clock();
    for (uint64_t x_candidate = 1; x_candidate < p; x_candidate++) {
        if (Mod_pow(g, x_candidate, p) == y) {
            found_x = x_candidate;
            break;
        }
    }
    clock_t t_end = clock();
    double time = (double)(t_end - t_start) / CLOCKS_PER_SEC;
    cout << "Затраченное время: " << time << " c." << endl;
    
    cout << "Найденный x: " << found_x << endl;
    cout << "Истинный x: " << Private_Key << endl;
    
    if (found_x == Private_Key) {
        cout << "УСПЕХ! Атакующий теперь может расшифровать любые сообщения!" << endl;
    }
}


void Test_Sypher(){
    vector<int> sv = {2,3,5,7,11,13,17,19,23};
    assert(Sieve(25) == sv);
    assert(Mod_pow(3, 100, 7) == 4);
    vector<pair<uint64_t, uint64_t>> dc = {{3, 2}};
    assert(Decomposition(9) == dc);
    assert(MillerTest(15, 5) == false);
    assert(G_is_PrimitiveRoot(3, 7));
}


void LaunchSypher(){
    Test_Sypher();
    
    Sieve(500);
    GenKeys(30, 10);
    Session_Key = 1 + (gen() % (Public_Key['p'] - 2));
    string plain_text;
    
    cout << "Введите текст, который хотите зашифровать" << endl;
    
    getline(cin, plain_text);
    getline(cin, plain_text);


    Encryption(plain_text, "/Users/vladislav/Documents/PR2/PR2/cypher.txt");
   
    cout << "Зашифрованное сообшение записано в файл cypher.txt: " << endl;
    
    cout << "Расшифрованное сообщение: " << endl;
    
    Decryption(plain_text, "/Users/vladislav/Documents/PR2/PR2/cypher.txt");
    cout << plain_text << endl;

}



void LaunchWithAtack(){
    Test_Sypher();
    
    Sieve(500);
    GenKeys(25, 10);
    Session_Key = 1 + (gen() % (Public_Key['p'] - 2));
    string plain_text;
    cout << "Введите текст, который хотите зашифровать" << endl;
    
    getline(cin, plain_text);
    getline(cin, plain_text);
    
    
    Encryption(plain_text, "/Users/vladislav/Documents/PR2/PR2/cypher.txt");
    
    DemonstrateAttack();
   
    cout << "Зашифрованное сообшение записано в файл cypher.txt: " << endl;
    
    cout << "Расшифрованное сообщение: " << endl;
    
    Decryption(plain_text, "/Users/vladislav/Documents/PR2/PR2/cypher.txt");
    cout << plain_text << endl;
}


