# Лабораторная работа №1 — вариант 26

## Eco.MaxMin

Компонент ACOM для вычисления максимального и минимального значений из двух чисел. Реализован на языке C.

Интерфейс `IEcoMaxMin` содержит методы `Max` и `Min` для типов `int`, `long`, `float`, `double` и `long double`. Вызов методов выполняется через интерфейс компонента. Для создания компонента используется фабрика ACOM.

## Структура проекта

- `SharedFiles/IEcoMaxMin.h` — интерфейс компонента.
- `SourceFiles/CEcoMaxMin.c` — реализация методов.
- `SourceFiles/CEcoMaxMinFactory.c` — фабрика компонента.
- `UnitTestFiles/SourceFiles/EcoMaxMin.c` — консольный тестовый клиент.
- `AssemblyFiles/Mac/clang_arm64/Makefile` — сборка под macOS ARM64.
- `Eco_MaxMin_report.pdf` - репорт с тестами.

## Сборка и тестирование

Необходимы Apple Command Line Tools и установленный `Eco.Framework` с библиотеками для `Mac/arm64`.

В корне проекта:

```bash
export ECO_FRAMEWORK="$HOME/Downloads/Eco_labs/Eco.Framework"
make -C AssemblyFiles/Mac/clang_arm64 test
```

После успешного выполнения программа выводит:

```text
PASS: 12 Eco.MaxMin UnitTest checks
```

Проверяются операции над пятью числовыми типами, включая отрицательные и равные значения. Тестовый клиент получает компонент через `IEcoInterfaceBus1` и вызывает методы интерфейса `IEcoMaxMin`.

Результаты сборки находятся в `BuildFiles/Mac/arm64/`:

- `StaticRelease/` — статическая библиотека компонента (`.a`);
- `UnitTest/EcoMaxMin` — исполняемый тестовый клиент.

Для очистки результатов сборки:

```bash
make -C AssemblyFiles/Mac/clang_arm64 clean
```
