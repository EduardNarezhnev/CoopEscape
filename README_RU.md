[English version](README.md)

# Co-op Escape

Кооперативная многопользовательская игра для 2–4 игроков на Unreal Engine 5, созданная для портфолио.

## Что это

Два-четыре игрока должны собрать 3 ключа на арене, избегая умных ловушек, и добраться до зоны эвакуации. Победа засчитывается, когда все ключи собраны и все игроки находятся в зоне эвакуации.

## Управление

- WASD — движение
- Мышь — камера
- Space — прыжок
- ESC — пауза

## Что реализовано

- Мультиплеер на 2–4 игроков (Listen Server)
- Репликация персонажа, движения и здоровья
- Сервер-авторитарная система подбора (ключи, аптечки)
- Умные ловушки, которые преследуют ближайшего игрока
- Синхронизация урона и лечения
- HUD у каждого игрока (здоровье, личные ключи, общие ключи, предупреждение об опасности)
- Уведомления («Need X more keys!», «Waiting for other players...»)
- Сетевая пауза (меню у всех игроков)
- Экран победы/поражения у всех игроков
- Рестарт уровня
- Визуальные эффекты (красная вспышка при уроне, зелёная при лечении)
- Главное меню (Host / Join / Exit)

## Технологии

Unreal Engine 5.6, C++ (основная логика), Blueprint (UI), Enhanced Input, Replication, RPC (Server, Client), DataAsset (настройка ловушек), Git.

## Скриншоты и видео

![Главное меню](Docs/screenshot_menu.jpg)
![HUD](Docs/screenshot_hud.jpg)
![Ловушка](Docs/screenshot_hazard.jpg)
![Меню паузы](Docs/screenshot_pause.jpg)
![Экран победы](Docs/screenshot_win.jpg)
![Экран поражения](Docs/screenshot_lose.jpg)

[Видео геймплея на YouTube](https://youtu.be/lmssMdeIZzE) — 2 минуты, показывает Host, Join и основной геймплей.

## Скачать

[Ссылка на Google Drive](https://drive.google.com/file/d/1K9KfCqLFGEE51t5C8LbicqSssrENspqH/view?usp=sharing)

**Платформа:** Windows 64-bit  
Распакуй архив и запусти `CoopEscape.exe`.

**Как играть:**
1. Запусти `CoopEscape.exe` на первом ПК → **Host**.
2. Запусти `CoopEscape.exe` на втором ПК → **Join** → введи IP первого ПК (например, `192.168.1.10`).

## Ограничения

- Только Listen Server (без Dedicated Server).
- Нет Host Migration при выходе хоста.
- Только Windows 64-bit.

## Контакты

GitHub: [EduardNarezhnev](https://github.com/EduardNarezhnev)