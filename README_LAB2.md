# Лабораторная работа №2 — включение компонента

Продолжение лабораторной №1 (`Eco.MaxMin`). Внешний компонент поддерживает
`IEcoMaxMin`, а также `IEcoCalculatorX`. Операции сложения и вычитания
не реализуются в `Eco.MaxMin`: он сохраняет указатель на независимый компонент
`Eco.CalculatorA` и перенаправляет вызовы его методам.

Исходник `Eco.CalculatorA` используется из урока Lesson02 репозитория
[Eco.Education](https://github.com/peerf-eco/Eco.Education/tree/main/Eco.Pro.Training/001.InsideACOM/Lessons/Lesson02/Eco.CalculatorA).
Учебный компонент собирается из исходников **отдельно** и связывается с тестом
как самостоятельная статическая библиотека.

## Сборка на Apple Silicon

Нужны Command Line Tools, `Eco.Framework` с библиотеками `Mac/arm64`
и `Eco.Education`. Оба репозитория расположите в одной папке:

```text
EcoLabs/
  lab_EcoInterface/
  Eco.Education/
```

```bash
cd ~/Documents/EcoLabs
# lab_EcoInterface — ваш репозиторий с изменениями лабораторной 2
# если репозиторий Eco.Education ещё не клонирован:
git clone https://github.com/peerf-eco/Eco.Education.git
cd lab_EcoInterface
export ECO_FRAMEWORK="$HOME/Downloads/Eco_labs/Eco.Framework"
make -C AssemblyFiles/Mac/clang_arm64 clean
make -C AssemblyFiles/Mac/clang_arm64 test
```

Если `Eco.Education` лежит в другом месте, передайте `EDUCATION="/путь/Eco.Education"`
в `make` либо установите переменную `EDUCATION` в окружении.

## Проверки

Консольный UnitTest:

- выполняет исходные 12 проверок `Max/Min`;
- регистрирует фабрику `Eco.CalculatorA` и затем фабрику `Eco.MaxMin`;
- получает `IEcoCalculatorX` через `QueryInterface` у **внешнего** компонента;
- проверяет сложение `3 + 5 = 8` и вычитание `3 - 5 = -2`;
- получает `IEcoMaxMin` обратно из `IEcoCalculatorX`;
- проверяет, что запросы `IEcoUnknown` от обоих интерфейсов возвращают один объект.

Код внешнего компонента: `SourceFiles/CEcoMaxMin.c`, интерфейсный адаптер:
`HeaderFiles/CEcoMaxMin.h`, тестовый клиент:
`UnitTestFiles/SourceFiles/EcoMaxMin.c`.

В рамках данного варианта реализовано **включение (containment)** компонента A.
Агрегирование и остальные компоненты A–E не реализованы.
