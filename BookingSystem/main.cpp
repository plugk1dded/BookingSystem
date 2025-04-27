#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include "bookingsystem.h"
#include "event.h"
#include "user.h"
#include "ticket.h"
#include "datetime.h"

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void displaySystemInfo() {
    std::cout << "\n=========================================================\n";
    std::cout << "           СИСТЕМА БРОНИРОВАНИЯ БИЛЕТОВ                 \n";
    std::cout << "=========================================================\n";
    std::cout << "Версия: 1.0\n";
    std::cout << "Дата: " << DateTime::now().toDateString() << "\n";
    std::cout << "=========================================================\n\n";
}

void displayStatistics(const BookingSystem& system) {
    std::cout << "\n===================== СТАТИСТИКА =====================\n";
    std::cout << "Общая сумма продаж: " << system.getTotalSales() << " руб.\n";
    std::cout << "Активных билетов: " << system.getActiveTicketsCount() << "\n";
    std::cout << "Отмененных билетов: " << system.getCanceledTicketsCount() << "\n";
    std::cout << "Средняя цена билета: " << system.getAverageTicketPrice() << " руб.\n";
    std::cout << "=======================================================\n\n";
}

void searchEvents(BookingSystem& system) {
    int choice = 0;
    std::string searchQuery;
    std::vector<std::shared_ptr<Event>> results;

    std::cout << "\n================== ПОИСК СОБЫТИЙ ==================\n";
    std::cout << "1. Поиск по названию\n";
    std::cout << "2. Поиск по категории\n";
    std::cout << "3. Поиск по дате\n";
    std::cout << "4. Показать предстоящие события\n";
    std::cout << "5. Сортировка по дате (от ранней к поздней)\n";
    std::cout << "6. Сортировка по дате (от поздней к ранней)\n";
    std::cout << "7. Сортировка по цене (от низкой к высокой)\n";
    std::cout << "8. Сортировка по цене (от высокой к низкой)\n";
    std::cout << "0. Вернуться в главное меню\n";
    std::cout << "Выберите опцию: ";
    std::cin >> choice;

    if (choice == 0) {
        return;
    }

    if (choice >= 1 && choice <= 3) {
        clearInputBuffer();
        std::cout << "Введите запрос для поиска: ";
        std::getline(std::cin, searchQuery);
    }

    switch (choice) {
    case 1:
        results = system.findEventsByName(searchQuery);
        break;
    case 2:
        results = system.findEventsByCategory(searchQuery);
        break;
    case 3:
        results = system.findEventsByDate(searchQuery);
        break;
    case 4:
        results = system.getUpcomingEvents();
        break;
    case 5:
        results = system.getEventsSortedByDate(true);
        break;
    case 6:
        results = system.getEventsSortedByDate(false);
        break;
    case 7:
        results = system.getEventsSortedByPrice(true);
        break;
    case 8:
        results = system.getEventsSortedByPrice(false);
        break;
    default:
        std::cout << "Неверный выбор!\n";
        return;
    }

    if (results.empty()) {
        std::cout << "Не найдено событий по вашему запросу.\n";
    }
    else {
        std::cout << "Найдено " << results.size() << " событий:\n";
        std::cout << "========================================\n";
        for (const auto& event : results) {
            event->display();
            std::cout << "----------------------------------------\n";
        }
    }
}

void manageUserTickets(BookingSystem& system) {
    int userId, ticketId;

    std::cout << "Введите ID пользователя: ";
    std::cin >> userId;

    auto user = system.findUserById(userId);
    if (!user) {
        std::cout << "Пользователь с ID " << userId << " не найден.\n";
        return;
    }

    auto tickets = system.getTicketsByUser(userId);
    if (tickets.empty()) {
        std::cout << "У пользователя " << user->getName() << " нет билетов.\n";
        return;
    }

    std::cout << "Билеты пользователя " << user->getName() << ":\n";
    std::cout << "============================================\n";
    for (const auto& ticket : tickets) {
        ticket->display();
        auto event = system.findEventById(ticket->getEventId());
        if (event) {
            std::cout << "Событие: " << event->getName() << " (" << event->getDate() << ")\n";
        }
        std::cout << "--------------------------------------------\n";
    }

    std::cout << "Хотите отменить бронирование? (1 - Да, 0 - Нет): ";
    int choice;
    std::cin >> choice;

    if (choice == 1) {
        std::cout << "Введите ID билета для отмены: ";
        std::cin >> ticketId;

        if (system.cancelTicket(ticketId)) {
            std::cout << "Бронирование успешно отменено.\n";
        }
        else {
            std::cout << "Не удалось отменить бронирование. Проверьте ID билета.\n";
        }
    }
}

void editEvent(BookingSystem& system) {
    int eventId;

    std::cout << "Введите ID события для редактирования: ";
    std::cin >> eventId;

    auto event = system.findEventById(eventId);
    if (!event) {
        std::cout << "Событие с ID " << eventId << " не найдено.\n";
        return;
    }

    std::cout << "Текущая информация о событии:\n";
    event->display();

    int choice = 0;
    std::cout << "\nЧто вы хотите изменить?\n";
    std::cout << "1. Название\n";
    std::cout << "2. Дату\n";
    std::cout << "3. Место проведения\n";
    std::cout << "4. Базовую цену\n";
    std::cout << "5. Описание\n";
    std::cout << "6. Категорию\n";
    std::cout << "0. Отмена\n";
    std::cout << "Выберите опцию: ";
    std::cin >> choice;

    std::string strValue;
    double doubleValue;

    switch (choice) {
    case 0:
        return;
    case 1:
        clearInputBuffer();
        std::cout << "Введите новое название: ";
        std::getline(std::cin, strValue);
        event->setName(strValue);
        break;
    case 2:
        std::cout << "Введите новую дату (ГГГГ-ММ-ДД): ";
        std::cin >> strValue;
        event->setDate(strValue);
        break;
    case 3:
        clearInputBuffer();
        std::cout << "Введите новое место проведения: ";
        std::getline(std::cin, strValue);
        event->setVenue(strValue);
        break;
    case 4:
        std::cout << "Введите новую базовую цену: ";
        std::cin >> doubleValue;
        event->setBasePrice(doubleValue);
        break;
    case 5:
        clearInputBuffer();
        std::cout << "Введите новое описание: ";
        std::getline(std::cin, strValue);
        event->setDescription(strValue);
        break;
    case 6:
        clearInputBuffer();
        std::cout << "Введите новую категорию: ";
        std::getline(std::cin, strValue);
        event->setCategory(strValue);
        break;
    default:
        std::cout << "Неверный выбор!\n";
        return;
    }

    event->saveToFile();
    std::cout << "Информация о событии успешно обновлена.\n";
}

void showMenu(BookingSystem& system) {
    int choice = 0;

    do {
        std::cout << "\n====== СИСТЕМА БРОНИРОВАНИЯ БИЛЕТОВ ======\n";
        std::cout << "1. Показать все события\n";
        std::cout << "2. Показать всех пользователей\n";
        std::cout << "3. Показать все билеты\n";
        std::cout << "4. Поиск событий\n";
        std::cout << "5. Создать новое событие\n";
        std::cout << "6. Создать нового пользователя\n";
        std::cout << "7. Забронировать билет\n";
        std::cout << "8. Управление билетами пользователя\n";
        std::cout << "9. Редактировать событие\n";
        std::cout << "10. Показать статистику\n";
        std::cout << "0. Выход\n";
        std::cout << "Выберите опцию: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            system.displayAllEvents();
            break;
        case 2:
            system.displayAllUsers();
            break;
        case 3:
            system.displayAllTickets();
            break;
        case 4:
            searchEvents(system);
            break;
        case 5: {
            std::string name, date, venue;
            int totalSeats, eventType;
            double basePrice;

            std::cout << "Введите название события: ";
            clearInputBuffer();
            std::getline(std::cin, name);

            std::cout << "Введите дату (ГГГГ-ММ-ДД): ";
            std::cin >> date;

            std::cout << "Введите место проведения: ";
            clearInputBuffer();
            std::getline(std::cin, venue);

            std::cout << "Введите количество мест: ";
            std::cin >> totalSeats;

            std::cout << "Введите базовую цену: ";
            std::cin >> basePrice;

            std::cout << "Выберите тип события (1 - Концерт, 2 - Театральная постановка): ";
            std::cin >> eventType;

            if (eventType == 1) {
                std::string artist, genre, description, category;
                int duration;

                std::cout << "Введите исполнителя: ";
                clearInputBuffer();
                std::getline(std::cin, artist);

                std::cout << "Введите жанр: ";
                std::getline(std::cin, genre);

                std::cout << "Введите продолжительность (в минутах): ";
                std::cin >> duration;

                std::cout << "Введите описание: ";
                clearInputBuffer();
                std::getline(std::cin, description);

                std::cout << "Введите категорию: ";
                std::getline(std::cin, category);

                system.createConcert(name, date, venue, totalSeats, basePrice,
                    artist, genre, duration, description, category);
                std::cout << "Концерт успешно создан!\n";
            }
            else if (eventType == 2) {
                std::string director, genre, description, category;
                int duration, ageLimit;

                std::cout << "Введите режиссера: ";
                clearInputBuffer();
                std::getline(std::cin, director);

                std::cout << "Введите жанр: ";
                std::getline(std::cin, genre);

                std::cout << "Введите продолжительность (в минутах): ";
                std::cin >> duration;

                std::cout << "Введите возрастное ограничение: ";
                std::cin >> ageLimit;

                std::cout << "Введите описание: ";
                clearInputBuffer();
                std::getline(std::cin, description);

                std::cout << "Введите категорию: ";
                std::getline(std::cin, category);

                system.createTheatrePlay(name, date, venue, totalSeats, basePrice,
                    director, genre, duration, ageLimit,
                    description, category);
                std::cout << "Театральная постановка успешно создана!\n";
            }
            else {
                std::cout << "Неверный тип события!\n";
            }
            break;
        }
        case 6: {
            std::string name, email, phone;

            std::cout << "Введите имя пользователя: ";
            clearInputBuffer();
            std::getline(std::cin, name);

            std::cout << "Введите email: ";
            std::cin >> email;

            std::cout << "Введите телефон: ";
            std::cin >> phone;

            system.createUser(name, email, phone);
            std::cout << "Пользователь успешно создан!\n";
            break;
        }
        case 7: {
            int userId, eventId;

            std::cout << "Введите ID пользователя: ";
            std::cin >> userId;

            std::cout << "Введите ID события: ";
            std::cin >> eventId;

            auto user = system.findUserById(userId);
            auto event = system.findEventById(eventId);

            if (user && event) {
                if (user->bookTicket(event)) {
                    std::cout << "Билет успешно забронирован!\n";
                }
                else {
                    std::cout << "Не удалось забронировать билет!\n";
                }
            }
            else {
                std::cout << "Пользователь или событие не найдены!\n";
            }
            break;
        }
        case 8:
            manageUserTickets(system);
            break;
        case 9:
            editEvent(system);
            break;
        case 10:
            displayStatistics(system);
            break;
        case 0:
            std::cout << "Выход из программы. До свидания!\n";
            break;
        default:
            std::cout << "Неверный выбор, попробуйте снова.\n";
        }
    } while (choice != 0);
}

int main() {
    BookingSystem& system = BookingSystem::getInstance();

    displaySystemInfo();

    std::ifstream test_file("events.txt");
    bool files_exist = test_file.good();
    test_file.close();

    if (files_exist) {
        std::cout << "Обнаружены существующие данные. Загружаем данные из файлов...\n";
        system.loadData();
    }
    else {
        std::cout << "Файлы с данными не найдены. Создаем демонстрационные данные...\n";

        auto concert1 = system.createConcert(
            "Рок-фестиваль", "2023-07-15", "Стадион", 1000, 2500.0,
            "Разные артисты", "Рок", 240, "Большой рок-фестиваль с участием звезд", "Фестиваль"
        );

        auto concert2 = system.createConcert(
            "Джазовый вечер", "2023-08-10", "Джаз-клуб", 200, 1500.0,
            "Джаз-банд", "Джаз", 180, "Вечер классического джаза", "Концерт"
        );

        auto play1 = system.createTheatrePlay(
            "Гамлет", "2023-08-20", "Театр драмы", 200, 1500.0,
            "Иванов И.И.", "Драма", 210, 12, "Классическая постановка Шекспира", "Спектакль"
        );

        auto play2 = system.createTheatrePlay(
            "Чайка", "2023-09-05", "Малый театр", 150, 1800.0,
            "Петров П.П.", "Драма", 180, 16, "Пьеса А.П. Чехова", "Спектакль"
        );

        auto user1 = system.createUser("Иван Петров", "ivan@example.com", "+7-900-123-4567");
        auto user2 = system.createUser("Мария Сидорова", "maria@example.com", "+7-900-765-4321");

        user1->bookTicket(concert1);
        user1->bookTicket(play1);
        user2->bookTicket(concert2);
        user2->bookTicket(play2);
    }

    showMenu(system);

    BookingSystem::destroy();

    return 0;
}