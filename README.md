# Лабораторная работа № 2 — включение и агрегирование компонентов ACOM

**Проект:** `Eco.MaxMin`  
**Язык:** C (Apple Clang, режим `-std=c89`)  
**Платформа для проверки:** macOS ARM64 (Apple Silicon)  
**Ветка лабораторной № 2:** `lab2-extended-1`

## Задача

Продолжить лабораторную № 1: добавить к `Eco.MaxMin` использование готовых компонентов-калькуляторов из `InsideACOM` с механизмами **включения (containment)** и **агрегирования (aggregation)**. Показать арифметические операции и получение интерфейсов через `QueryInterface` в консольном UnitTest.

Первая лабораторная (реализация `Max/Min` и самостоятельные 12 тестов) сохранена в ветке [`main`](https://github.com/andrew52522/lab_EcoInterface/tree/main). Во второй лабораторной соответствующие методы не удалены.

## Архитектура

Внешний компонент `Eco.MaxMin` предоставляет:

- `IEcoMaxMin` — `Max/Min` для `int`, `long`, `float`, `double`, `long double`;
- `IEcoCalculatorX` — сложение `Addition` и вычитание `Subtraction`;
- `IEcoCalculatorY` — умножение `Multiplication` и деление `Division`.

Обращения к `IEcoCalculatorX` и `IEcoCalculatorY` перенаправляются внутренним компонентам. Реализации калькуляторов находятся в учебном репозитории `Eco.Education`.

| Режим | Источник `IEcoCalculatorX` | Источник `IEcoCalculatorY` | Дополнительная демонстрация |
| --- | --- | --- | --- |
| `A` | `Eco.CalculatorA` | `Eco.CalculatorE` | `E` агрегирует `B`; `D` включает `B` в отдельном тесте |
| `C` | `Eco.CalculatorC` | `Eco.CalculatorE` | `C` предоставляет оба интерфейса; `E` агрегирует `B` |
| `D` | `Eco.CalculatorD` | `Eco.CalculatorE` | `D` включает `B`; `E` агрегирует `B` |

**Почему несколько режимов:** в исходных примерах `Eco.CalculatorA` и `Eco.CalculatorC` имеют одинаковый идентификатор компонента (`CID`) и символ статической фабрики. Для избежания конфликта используются отдельные конфигурации сборки.

## Подготовка к сборке

Нужны:

- Mac с ARM64 и **Apple Command Line Tools** (`clang`, `make`, `git`);
- распакованный `Eco.Framework` с библиотеками `Mac/arm64`;
- репозиторий  [`peerf-eco/Eco.Education`](https://github.com/peerf-eco/Eco.Education), расположенный **рядом** с этим проектом.

Пример размещения:

```text
EcoLabs2/
├── lab_EcoInterface/    # эта ветка: lab2-extended-1
└── Eco.Education/       # InsideACOM
```

Для новой машины:

```bash
mkdir -p ~/Documents/EcoLabs2
cd ~/Documents/EcoLabs2
git clone -b lab2-extended-1 https://github.com/andrew52522/lab_EcoInterface.git
git clone https://github.com/peerf-eco/Eco.Education.git
cd lab_EcoInterface
export ECO_FRAMEWORK="/абсолютный/путь/к/Eco.Framework"
```

## Проверка лабораторной № 2

Из корня проекта:

```bash
make -C AssemblyFiles/Mac/clang_arm64 clean
make -C AssemblyFiles/Mac/clang_arm64 test
```

`make test` последовательно проверяет режимы `A`, `C` и `D`. В этих тестах выполняются базовые проверки `Max/Min`, арифметические операции, переходы между интерфейсами через `QueryInterface`, проверка `IEcoUnknown`, а также взаимодействие `D → B` и `E → B`.

Успешный итоговый вывод:

```text
[PASS] mode=A: ...
[PASS] mode=C: ...
[PASS] mode=D: ...
PASS: Lab 2 ACOM / A B C D E / containment + aggregation
PASS: all 3 build variants and their UnitTests
```

По результатам проверки на Mac M4 все три режима завершились с `EXIT CODE: 0`.

## Проверка обеих лабораторных одной командой

```bash
bash scripts/test_all.sh
```

Скрипт использует отдельный временный `git worktree` от `origin/main`, запускает **исходные 12 Unit-тестов лабораторной № 1**, затем полный `make test` лабораторной № 2. Текущая рабочая ветка не переключается. Такой объединённый запуск предоставлен для удобства; исходные файлы лабораторной № 1 остаются в ветке `main`.

## Особенности сборки

- Применяется статическая компоновка (`.a`), полный Xcode не требуется.
- Режим `-std=c89` относится к выбранному стандарту компиляции C (не к году создания проекта).
- Для совместимости старых исходников преподавателя Makefile определяет `ERR_ECO_SUCCESES=ERR_ECO_SUCCESS`.
- Временная копия исходника фабрики заменяет C++-комментарий `//` на комментарий, совместимый с C89. Исходники `Eco.Education` не изменяются.
- В `BuildFiles/` лежат результаты сборки, они не коммитятся.

## Основные файлы

- `SharedFiles/IEcoMaxMin.h` — интерфейс из первой лабораторной;
- `HeaderFiles/CEcoMaxMin.h`, `SourceFiles/CEcoMaxMin.c` — компонент с адаптерами X/Y;
- `SourceFiles/CEcoMaxMinFactory.c` — фабрика ACOM;
- `UnitTestFiles/SourceFiles/EcoMaxMin.c` — тесты лабораторной № 2;
- `AssemblyFiles/Mac/clang_arm64/Makefile` — сборка трёх режимов;
- `scripts/test_all.sh` — общий запуск тестов лабораторных № 1 и № 2;
- `Eco_MaxMin_Lab2_report.pdf` и `.docx` — отчёт по второй лабораторной.

## Источники

- Учебное задание «Задание 2» и лекция 04 по ACOM.
- [Eco.Education — InsideACOM](https://github.com/peerf-eco/Eco.Education/tree/main/Eco.Pro.Training/001.InsideACOM).
