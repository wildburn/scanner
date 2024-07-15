#include <sys/utsname.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <array>
#include <memory>
#include <stdexcept>
#include <sstream>

using namespace std;

void printConfigValue(const string &filePath, const vector<string> &settings) { // проверяет наличие модуля pam_pwquality//5
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Не удалось открыть файл: " << filePath << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
    for (const auto &setting : settings) {
        size_t foundPos = line.find(setting);
        if (foundPos != string::npos && (foundPos == 0 || line[foundPos - 1] != '#')) {
            cout << line << endl;
            break; // Нашли настройку, переходим к следующей строке
        }
    }
}

    file.close();
}





bool isGuestSessionEnabled() {//доп.параметры гостевой вход
    ifstream file("/etc/lightdm/lightdm.conf");
    string line;
    
    if (file.is_open()) {
        while (getline(file, line)) {
            if (line.find("allow-guest=") != string::npos) {
                if (line.find("true") != string::npos) {
                    return true; // Гостевой вход включен
                }
                break;
            }
        }
        file.close();
    }
    
    return false; // Гостевой вход отключен или файл конфигурации не найден
}

bool isPasswordCachingEnabled() {//кэширование паролей
    ifstream file("/etc/sssd/sssd.conf");
    string line;
    bool cache_found = false;
    
    if (file.is_open()) {
        while (getline(file, line)) {
            if (line.find("cache_credentials") != string::npos) {
                stringstream ss(line);
                string key, equals, value;
                ss >> key >> equals >> value;
                if (value == "True" || value == "true") {
                    cache_found = true;
                    break;
                }
            }
        }
        file.close();
    }
    
    return cache_found;
}
int main() {
    
    
    string pwqualityPath = "/etc/security/pwquality.conf";
    string loginDefsPath = "/etc/login.defs";
    
    while(true) {

    int scenario = 0;
    cout << "Выберите операцию:\n";
    cout << "1.Вид операционной системы.:\n";
    cout << "2.Определение имени узла:\n";
    cout << "3.Установленные обновления безопасности:\n";
    cout << "4.Учетные записи администраторов:\n";
    cout << "5.Политика паролей.:\n";
    cout << "6.Аудит системы:\n";
    cout << "7.Сетевые настройки:\n";
    cout << "8.Запущенные сервисы:\n";
    cout << "9.Файловая система:\n";
    cout << "10.Анализ журналов:\n";
    cout << "11.Обнаружение сканирования портов:\n";
    cout << "12.Дополнительные параметры:\n";
    cout << "13.Выход:\n";
    cin >> scenario;


    switch (scenario) {

        case 1: {
            cout << "Версия ядра"<< endl;
            system("uname -r");
            system("lsb_release -a");
   



        }
        break;
        case 2: {
            // Сценарий 2: Выполнение команды hostnamectl
            cout << "Запуск команды hostnamectl:" << endl;
            cout << endl;
            int return_value = system("hostnamectl");
            cout << endl;
            if (return_value != 0) {
                cerr << "Ошибка при выполнении команды hostnamectl." << endl;
                return 1;
            }
            break;
        }
        break;

        case 3:{
            cout << endl;
            cout << "Запуск команды 'sudo dnf list installed --security':" << endl;

            system("sudo dnf list installed --security");
            cout << endl;
        }break;

        case 4: {
            cout << endl;
            cout << "Запуск команды 'getent group wheel': " << endl;

            int result = system("getent group wheel");
            cout << endl;
            if (result != 0) {
                cerr << "Команда завершилась ошибкой. Произошла ошибка" << endl;
            }
            
        }
        break;



        case 5: {

            cout << endl;
            cout << "Настройки pwquality.conf:" << endl;
            printConfigValue(pwqualityPath, {
                "minlen", "minclass", "maxrepeat", "maxclassrepeat",
                "dcredit", "ucredit", "lcredit", "ocredit"
            });
            
        
            cout << "Настройки login.defs:" << endl;
            printConfigValue(loginDefsPath, {
                "PASS_MAX_DAYS", "PASS_MIN_DAYS", "PASS_WARN_AGE", "ENCRYPT_METHOD"
            });

            cout << endl;
        }
     break;

        case 6: {
            cout << endl;
            cout << "Запуск команды аудита Lynis': " << endl;
        // Объявление переменной здесь ограничивает ее область видимости текущим case
        int result = system("sudo lynis audit system");
        cout << endl;
        if (result != 0) {
            cerr << "Произошла ошибка при выполнении команды аудита Lynis." << endl;
        }        
        break;
    }
        break;
    case 7: {
            cout << endl;
            cout << "Запуск команды проверки сети': " << endl;
            cout << endl;
        int result = system("ip address");
        if (result != 0) {
            cerr << "Произошла ошибка при выполнении команды проверки сети" << endl;
        }        
        break;
    }
    break;


    
    case 8: {
            cout << endl;
            cout << "Запуск команды проверки запущенных процессов': " << endl;
           
        int result = system("systemctl list-units --type=service --state=running --no-pager -l");
        cout << endl;

        if (result != 0) {
            cerr << "Запущенные сервисы" << endl;
        }        
        break;
    }
    break;


    case 9: {
        cout << endl;
        cout << "Запуск команды для проверки файловой системы': " << endl;
        
        int result = system("df -Th");
        cout << endl;
        if (result != 0) {
            cerr << "Произошла ошибка при выполнении команды проверки файловой системы" << endl;
        }        
        break;

    }
    break;;
        
    case 10:{
        cout << endl;//изменить 10 задание
        

        
        cout << "Выводим 10 строк логов" << endl;
        system("journalctl | tail -n 10");

        cout << "Общее кол-во логов" << endl;

        system("journalctl | wc -l");
        cout << endl;
        cout << endl;

        cout << "Ошибки, которые не критичны для системы" << endl;
        system("journalctl -p err --no-pager -l | tail -n 10 ");
        system("journalctl -p err --no-pager -l | wc -l");
        cout << endl;

        cout << endl;
        cout << "Фильтр записи с приоритетом 'crit' (критические) за день " << endl;
        system("journalctl -p crit --since '1 day ago'");
        cout << endl;

    }break;


    case 11:{
        cout << endl;
        cout << "Обнаружение сканирования портов" << endl;
        system("netstat -tulpn");
        cout << endl;
    }
    break;


    case 12:{
        cout << endl;

            cout << "Проверка состояния гостевой сессии:" << endl;
    
    if (isGuestSessionEnabled()) {
        cout << "Гостевой вход разрешен." << endl;
    } else {
        cout << "Гостевой вход запрещен или LightDM не используется." << endl;
    }
    
    cout << "Кэширование паролей через sssd:" << endl;
    
    if (isPasswordCachingEnabled()) {
        cout << "Кэширование паролей включено." << endl;
    } else {
        cout << "Кэширование паролей выключено или sssd не используется." << endl;
    }

    

        cout << endl;
    }break;

    case 13:{
        
        cout << "выход"<< endl;
        return 0;
    }

        

        default: {
            cerr << "Выбран неверный сценарий. Выберите цифру" << endl;
        }
        
        }
    }

}
