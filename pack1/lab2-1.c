//близка к идеалу
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>


typedef struct {
    char login[7];
    int pin;
    int counter;
    int requests;
} User;

User users[BUFSIZ];
int num_users = 0;
int loggedIn = 0; // Флаг авторизации пользователя
int def_req = 1000;

void register_user() {
    if (num_users >= BUFSIZ) {
        printf("Максимальное количество пользователей достигнуто.\n");
        return;
    }

    User new_user;
    new_user.requests = def_req;
    printf("Введите логин (не более 6 символов): ");
    scanf("%6s", new_user.login);


    if (strlen(new_user.login) > 6 || strlen(new_user.login) == 0) {
        printf("Логин должен содержать от 1 до 6 символов.\n");
        return;
    }


    for (int i = 0; i < strlen(new_user.login); i++) {
        if (!isalnum(new_user.login[i]) && !isalpha(new_user.login[i])) {
            printf("Логин может содержать только латинские буквы и цифры. Попробуйте снова.\n");
            return;
        }
    }

    for (int i = 0; i < num_users; i++) {
        if (strcmp(new_user.login, users[i].login) == 0) {
            printf("Такой логин уже используется другим пользователем.\n");
            return;
        }
    }

    printf("Введите PIN-код (от 0 до 100000): ");
    if (scanf("%d", &new_user.pin) != 1) {
        printf("Некорректный PIN-код. Введите целое число.\n");
        return;
    }

    users[num_users] = new_user;
    num_users++;

    printf("Пользователь успешно зарегистрирован.\n");
}



void login() {
    char login[7];
    int pin;

    printf("Введите логин: ");
    scanf("%6s", login);

    for (int i = 0; i < num_users; i++) {
        if (strcmp(login, users[i].login) == 0) {
            printf("Введите PIN-код: ");
            if (scanf("%d", &pin) != 1) {
                printf("Некорректный PIN-код. Введите целое число.\n");
                return;
            }

            if (pin == users[i].pin) {
                printf("Авторизация успешна. Добро пожаловать, %s!\n", login);
                loggedIn = 1; // Установка флага авторизации
                return;
            } else {
                printf("Неверный PIN-код для пользователя %s.\n", login);
                return;
            }
        }
    }

    printf("Пользователь %s не найден. Зарегистрируйтесь, чтобы продолжить.\n", login);
}

void show_user_names() {
    printf("Список зарегистрированных пользователей:\n");
    for (int i = 0; i < num_users; i++) {
        printf("%s - %d - %d\n", users[i].login, users[i].pin, users[i].requests);
    }
}

void sanctions(User* user);

void time_command() {
    sanctions(users);
    time_t current_time;
    struct tm* time_info;
    char time_string[9];

    time(&current_time);
    time_info = localtime(&current_time);

    strftime(time_string, sizeof(time_string), "%H:%M:%S", time_info);

    printf("Текущее время: %s\n", time_string);
}


void date_command() {
    sanctions(users);
    time_t current_time;
    struct tm* time_info;
    char date_string[11];

    time(&current_time);
    time_info = localtime(&current_time);

    strftime(date_string, sizeof(date_string), "%d.%m.%Y", time_info);

    printf("Текущая дата: %s\n", date_string);
}

void how_much_command(char* entered_time_str, char* flag) {
    int entered_day, entered_month, entered_year, entered_hour, entered_minute, entered_second;
    struct tm entered_time = {0};
    time_t current_time;
    time_t entered_time_t;
    double diff;

    sscanf(entered_time_str, "%d/%d/%d/%d/%d/%d", &entered_day, &entered_month, &entered_year, &entered_hour, &entered_minute, &entered_second);

    if ((entered_day > 31 || entered_day < 1) || (entered_month > 12 || entered_month < 1) || (entered_year < 1) || (entered_hour > 23
    || entered_hour < 1) || (entered_minute > 59 || entered_minute < 0) || (entered_second > 59 || entered_second <1)){
        printf("Incorect data\n");
        return;
    }

    entered_time.tm_mday = entered_day;
    entered_time.tm_mon = entered_month - 1;
    entered_time.tm_year = entered_year - 1900;
    entered_time.tm_hour = entered_hour;
    entered_time.tm_min = entered_minute;
    entered_time.tm_sec = entered_second;
    entered_time.tm_isdst = -1;

    entered_time_t = mktime(&entered_time);
    time(&current_time);

    if (strcmp(flag, "-s") == 0) {
        diff = difftime(current_time, entered_time_t);
        printf("Разница в секундах: %.0f\n", diff);
    } else if (strcmp(flag, "-m") == 0) {
        diff = difftime(current_time, entered_time_t) / 60.0;
        printf("Разница в минутах: %.2f\n", diff);
    } else if (strcmp(flag, "-h") == 0) {
        diff = difftime(current_time, entered_time_t) / 3600.0;
        printf("Разница в часах: %.2f\n", diff);
    } else if (strcmp(flag, "-y") == 0) {
        diff = difftime(current_time, entered_time_t) / (3600.0 * 24.0 * 365.0);
        printf("Разница в годах: %.2f\n", diff);
    } else {
        printf("Неверный флаг. Попробуйте еще раз.\n");
    }
}

void logout() {
    printf("Выход из аккаунта. Возвращение в меню авторизации.\n");
    loggedIn = 0; // Сброс флага авторизации
}

void sanctions_command(char* username, int number) {
    char login[7];
    int max_requests;
    char code[6];

    printf("Введите логин: ");
    scanf("%6s", login);

    int user_index = -1;
    for (int i = 0; i < num_users; i++) {
        if (strcmp(login, users[i].login) == 0) {
            user_index = i;
            break;
        }
    }

    if (user_index == -1) {
        printf("Пользователь %s не найден. Зарегистрируйтесь, чтобы продолжить.\n", login);
        return;
    }


    printf("Введите максимальное количество запросов: ");
    scanf("%d", &max_requests);

    if (max_requests <= 0) {
        printf("Максимальное количество запросов должно быть положительным числом.\n");
        return;
    }

    printf("Введите код подтверждения: ");
    scanf("%5s", code);

    if (strcmp(code, "12345") != 0) {
        printf("Неверный код подтверждения.\n");
        return;
    }

    int num_requests = 0;
    while (num_requests < max_requests) {
        char command[BUFSIZ];

        printf("Введите команду (3 - time, 4 - date, 5 - howmuch, 6 - logout, 7 - sanctions, 8 - show_user_names): ");
        scanf("%9s", command);

        if (strcmp(command, "3") == 0) {
            time_command();
        } else if (strcmp(command, "4") == 0) {
            date_command();
        } else if (strcmp(command, "5") == 0) {
            char entered_time_str[20];
            char flag[2];

            printf("Введите время в формате ДД/ММ/ГГГГ/ЧЧ/ММ/СС: ");
            scanf("%19s", entered_time_str);

            printf("Введите флаг (0 или 1): ");
            scanf("%1s", flag);

            how_much_command(entered_time_str, flag);
        } else if (strcmp(command, "6") == 0) {
            printf("Выход из системы.\n");
            return;
        } 
        else if (strcmp(command, "7") == 0){
            char username[7];
            int number;
            printf("Введите логин пользователя: ");
            scanf("%6s", username);
            printf("Введите количество ограничений: ");
            scanf("%d", &number);
            sanctions_command(username, number);
        }
        else if (strcmp(command, "8") == 0){
            show_user_names();
        }
        else {
            printf("Некорректная команда.\n");
        }

        num_requests++;
    }

    printf("Достигнуто максимальное количество запросов.\n");

    users->requests = num_requests;
    loggedIn = 0;

}


void sanctions(User* user){
    if (user->requests < def_req){
        printf("Достигнуто максимальное количество запросов.\n");
        loggedIn = 0;
        return;
    }
}


int main() {
    int command;
    char input[BUFSIZ];

    while (1) {
        if (!loggedIn) {
            printf("Введите команду (1 - register, 2 - login): ");
            scanf("%9s", input);

            if (strcmp(input, "1") == 0) {
                register_user();
            } else if (strcmp(input, "2") == 0) {
                login();
            } else {
                printf("Неверная команда. Попробуйте еще раз.\n");
                while (getchar() != '\n'); // Очистка буфера ввода
            }
        } else {
            printf("Введите команду (3 - time, 4 - date, 5 - howmuch, 6 - logout, 7 - sanctions, 8 - show_user_names): ");
            scanf("%9s", input);

            if (strcmp(input, "3") == 0) {
                time_command();
            } else if (strcmp(input, "4") == 0) {
                date_command();
            } else if (strcmp(input, "5") == 0) {
                char entered_time[20];
                char flag[3];
                printf("Введите дату (день/месяц/год/часы/минуты/секунды): ");
                scanf("%19s", entered_time);
                printf("Введите флаг (-s в секундах, -m в минутах, -h в часах, -y в годах): ");
                scanf("%2s", flag);
                how_much_command(entered_time, flag);
            } else if (strcmp(input, "6") == 0) {
                logout();
            } else if (strcmp(input, "7") == 0) {
                char username[7];
                int number;
                sanctions_command(username, number);
            } else if (strcmp(input, "8") == 0) {
                show_user_names();
            } else {
                printf("Неверная команда. Попробуйте еще раз.\n");
                while (getchar() != '\n'); // Очистка буфера ввода
            }
        }
    }

    return 0;
}
