HTTP/HTTPS Proxy Server на C++
GitHub

Этот проект представляет собой высокопроизводительный многопоточный прокси-сервер, поддерживающий HTTP и HTTPS трафик через CONNECT-туннелирование.

📥 Клонирование репозитория
bash
Copy
git clone git@github.com:KrutoiXatcker/proxy_server.git
cd proxy_server

🌟 Основные возможности
Поддержка HTTP/HTTPS (CONNECT-метод)

Многопоточная обработка запросов

Гибкая система логирования

Модульная архитектура

Кросс-платформенная совместимость (Linux/Unix)

🛠 Сборка и запуск
Требования
Компилятор C++11 (g++/clang++)

Утилита make

Библиотеки pthread

Сборка
bash
Copy
make
Запуск
bash
Copy
./bin/proxy_server [-l log_file] [-e error_file]

Параметры:
-l - файл для стандартного вывода

-e - файл для вывода ошибок

🖥 Пример использования
bash
Copy
# Запуск с логированием
./bin/proxy_server -l proxy.log -e errors.log

# Проверка работы
curl -x http://localhost:8080 http://example.com

📂 Структура проекта

proxy_server/
├── bin/                # Скомпилированный бинарник
├── obj/                # Объектные файлы
├── src/                # Исходный код
│   ├── ClientConnectionHandler.cpp
│   ├── ClientConnectionHandler.h
│   ├── DataTunnel.cpp
│   ├── DataTunnel.h
│   ├── HttpRequestParser.cpp
│   ├── HttpRequestParser.h
│   ├── logger.cpp
│   ├── logger.h
│   ├── main.cpp
│   ├── ProxyServer.h
│   ├── SocketWrapper.cpp
│   ├── SocketWrapper.h
│   ├── TargetServerConnector.cpp
│   └── TargetServerConnector.h
├── makefile            # Файл для сборки
└── README.md           # Этот файл
