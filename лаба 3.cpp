#include <iostream>
#include <vector>
#include <string>

using namespace std;

// БАЗОВЫЙ КЛАСС: Route (Маршрут)
// Содержит общие для всех транспортных средств атрибуты и методы
class Route {
protected:
    // protected - доступно в этом классе и в классах-наследниках
    string destination;      // Пункт назначения
    string train_number;     // Номер поезда / рейса
    string departure_time;   // Время отправления (формат "HH:MM")
    double travel_hours;     // Время в пути (в часах, может быть дробным: 2.5)

public:
    // Конструктор по умолчанию - инициализирует поля пустыми значениями
    Route() : destination(""), train_number(""), departure_time(""), travel_hours(0) {}

    // Конструктор с параметрами - использует список инициализации для эффективности
    Route(string d, string n, string t, double h)
        : destination(d), train_number(n), departure_time(t), travel_hours(h) {
    }

    // Конструктор копирования - нужен для правильного копирования объектов
    Route(const Route& other) : destination(other.destination), train_number(other.train_number),
        departure_time(other.departure_time), travel_hours(other.travel_hours) {
    }


    // виртуальный декструктор позволяет правильно удалять объекты наследников через указатель на базовый класс
    virtual ~Route() {}

    // Метод подсчета времени прибытия (основной метод по условию 6-го варианта)
    // Возвращает строку в формате "HH:MM"
    string get_arrival_time() {
        // Извлекаем часы из строки "HH:MM" (первые 2 символа)
        int h = stoi(departure_time.substr(0, 2));
        // Извлекаем минуты (символы с позиции 3)
        int m = stoi(departure_time.substr(3, 2));
        // Переводим всё в минуты: текущее время + время в пути
        int total = (h * 60 + m) + (int)(travel_hours * 60);
        // Вычисляем часы прибытия (берем остаток от деления на 24, чтобы не выйти за сутки)
        h = (total / 60) % 24;
        // Вычисляем минуты прибытия
        m = total % 60;
        // Форматируем: добавляем ведущий ноль если нужно
        string hours = (h < 10 ? "0" : "") + to_string(h);
        string minutes = (m < 10 ? "0" : "") + to_string(m);
        return hours + ":" + minutes;
    }

    // Виртуальный метод show() - будет переопределен в классах-наследниках
    // virtual позволяет вызывать нужную версию метода через указатель на базовый класс
    virtual void show() {
        cout << destination << " | " << train_number << " | " << departure_time
            << " | " << travel_hours << "h | Arrival: " << get_arrival_time() << endl;
    }

    // Геттеры (методы доступа) - нужны для фильтрации объектов из main()
    string get_dest() { return destination; }
    string get_number() { return train_number; }
    string get_dep_time() { return departure_time; }
    double get_travel_hours() { return travel_hours; }
};

// ПРОИЗВОДНЫЙ КЛАСС 1: Train (Поезд)
// Наследует от Route, добавляет специфику поезда (места)
class Train : public Route {
private:
    // Новые атрибуты (специфичные только для поездов)
    int total_seats;      // Число общих мест
    int coupe_seats;      // Купейных мест
    int reserved_seats;   // Плацкартных мест

public:
    // Конструктор по умолчанию
    Train() : Route(), total_seats(0), coupe_seats(0), reserved_seats(0) {}

    // Конструктор с параметрами
    Train(string d, string n, string t, double h, int total, int coupe, int reserved)
        : Route(d, n, t, h), total_seats(total), coupe_seats(coupe), reserved_seats(reserved) {
    }

    // Конструктор копирования
    Train(const Train& other) : Route(other), total_seats(other.total_seats),
        coupe_seats(other.coupe_seats), reserved_seats(other.reserved_seats) {
    }

    // НОВЫЙ МЕТОД
    // Вычисляет количество свободных мест после продажи заданного количества билетов
    int free_seats(int sold) {
        return total_seats - sold;
    }

    // ПЕРЕГРУЗКА МЕТОДА show() - ключевое слово override показывает,
    // что мы переопределяем виртуальный метод базового класса
    void show() override {
        cout << "[TRAIN] " << destination << " #" << train_number
            << " | Dep: " << departure_time << " | Travel: " << travel_hours << "h"
            << " | Arrival: " << get_arrival_time()
            << " | Total:" << total_seats << " Coupe:" << coupe_seats
            << " Plat:" << reserved_seats << endl;
    }

    // Геттеры для новых атрибутов
    int get_total() { return total_seats; }
    int get_coupe() { return coupe_seats; }
    int get_reserved() { return reserved_seats; }
};

// ПРОИЗВОДНЫЙ КЛАСС 2: Vehicle (Транспортное средство)
// Второй производный класс по условию задания
class Vehicle : public Route {
private:
    // Новые атрибуты (3)
    string vehicle_type;   // Тип транспортного средства (электричка, экспресс, и т.д.)
    int max_speed;         // Максимальная скорость (км/ч)
    int num_cars;          // Количество вагонов

public:
    // Конструктор по умолчанию
    Vehicle() : Route(), vehicle_type(""), max_speed(0), num_cars(0) {}

    // Конструктор с параметрами
    Vehicle(string d, string n, string t, double h, string type, int speed, int cars)
        : Route(d, n, t, h), vehicle_type(type), max_speed(speed), num_cars(cars) {
    }

    // Конструктор копирования
    Vehicle(const Vehicle& other) : Route(other), vehicle_type(other.vehicle_type),
        max_speed(other.max_speed), num_cars(other.num_cars) {
    }

    //новый метода - вычисляет время в пути на основе расстояния и максимальной скорости
    double calc_time(double distance) {
        return distance / max_speed;
    }

    // перегрузка метода show() - свой формат вывода для транспортных средств
    void show() override {
        cout << "[VEHICLE] " << destination << " #" << train_number
            << " | Dep: " << departure_time << " | Travel: " << travel_hours << "h"
            << " | Arrival: " << get_arrival_time()
            << " | Type: " << vehicle_type << " | Speed: " << max_speed
            << " km/h | Cars: " << num_cars << endl;
    }

    // Геттеры для новых атрибутов
    string get_type() { return vehicle_type; }
    int get_speed() { return max_speed; }
    int get_cars() { return num_cars; }
};

// Гл функция
int main() {
    cout << "=== LAB 3: INHERITANCE (Variant 6) ===\n" << endl;

    // ВЕКТОР УКАЗАТЕЛЕЙ НА БАЗОВЫЙ КЛАСС
    // Храним указатели, а не объекты, чтобы работал полиморфизм
    // Если хранить объекты по значению, произойдет "срезание" (slicing)
    vector<Route*> routes;

    // СОЗДАНИЕ ОБЪЕКТОВ Train (4)
    routes.push_back(new Train("Moscow", "001A", "08:00", 2.5, 500, 200, 300));
    routes.push_back(new Train("Saint Petersburg", "015B", "12:30", 8.0, 800, 300, 500));
    routes.push_back(new Train("Kazan", "089C", "20:15", 11.5, 400, 150, 250));
    routes.push_back(new Train("Novosibirsk", "102D", "06:45", 48.0, 600, 250, 350));
    // СОЗДАНИЕ ОБЪЕКТОВ Vehicle (4)
    routes.push_back(new Vehicle("Sochi", "205E", "09:00", 24.0, "Express", 120, 15));
    routes.push_back(new Vehicle("Ekaterinburg", "312F", "23:30", 26.5, "Electric", 90, 12));
    routes.push_back(new Vehicle("Vladivostok", "789G", "14:20", 168.0, "High-speed", 200, 20));
    routes.push_back(new Vehicle("Rostov-on-Don", "456H", "07:15", 18.0, "Commuter", 80, 10));
    cout << "--- NEW METHODS ---" << endl;

    // dynamic_cast - безопасное преобразование указателя базового класса в указатель производного
    Train* t1 = dynamic_cast<Train*>(routes[0]);
    cout << t1->get_number() << " free seats (100 sold): " << t1->free_seats(100) << endl;

    Vehicle* v1 = dynamic_cast<Vehicle*>(routes[4]);
    cout << v1->get_number() << " time for 500km: " << v1->calc_time(500) << " hours\n" << endl;

    // ПОЛИМОРФНЫЙ ВЫВОД (пункт 5 условия)
    // Через указатель на базовый класс вызывается нужная версия show()
    // Благодаря virtual, объект сам "знает", какой show() ему нужно выполнить
    cout << "--- ALL OBJECTS ---" << endl;
    for (int i = 0; i < routes.size(); i++) {
        cout << i + 1 << ". ";
        routes[i]->show();  // ← ПОЛИМОРФИЗМ в действии!
    }

    // ФИЛЬТРАЦИЯ ОБЪЕКТОВ
    // Используем геттеры базового класса, которые доступны для всех объектов

    // Фильтр 1: по пункту назначения
    cout << "\n--- FILTER: Destination 'Moscow' ---" << endl;
    for (int i = 0; i < routes.size(); i++) {
        if (routes[i]->get_dest() == "Moscow") routes[i]->show();
    }

    // Фильтр 2: по времени в пути
    cout << "\n--- FILTER: Travel time <= 10 hours ---" << endl;
    for (int i = 0; i < routes.size(); i++) {
        if (routes[i]->get_travel_hours() <= 10) routes[i]->show();
    }
    // ОЧИСТКА ПАМЯТИ
    // Удаляем все объекты, созданные через new
    // Виртуальный деструктор ~Route() гарантирует правильное удаление
    for (int i = 0; i < routes.size(); i++) {
        delete routes[i];
    }

    cout << "\nAll objects deleted." << endl;
    return 0;
}