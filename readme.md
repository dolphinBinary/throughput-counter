# Throughput — Counter

## Счётчик пропускной способности
Приложение считает пропускную способность трафика на принимающей и передающей стороне канального уровня

### Требования
g++, ns-3, waf, cmake

### Запуск
#### Запуск симулятора
Из папки /throughput-counter/*your-ns-allinone*/ns-3.x

Запустить команду:
> ./waf --run scratch/lte-scenario.cc

#### Запуск вычислений
1) Положить в папку ./output файлы DlRLcStats.txt и UlRlcStats.txt
2) Запустить следующие команды:
> mkdir build

> cd build

> cmake ../

> make

> ./throughput_counter filename1 filename2

#### Пример
> ./throughput_counter DlRlcStats.txt UlRlcStats.txt

Вывод в консоль:
Передающая пропускная способность: 1684.53 Кб/с
Принимающая пропускная способность: 1683.71 Кб/с
 
# Методы
## calculateThroughput
*возвращает данные в килобайтах в секундах*

---

## Simple throughput counter 
This app calculates throughput dl and ul for two users on Rlc level

### Reqs
g++, ns-3, waf, cmake 

### Running
#### Run scenario
from directory /throughput-counter/*your-ns-allinone*/ns-3.x
> ./waf --run scratch/lte-scenario.cc

#### Run counter
1) Put your DlRLcStats.txt и UlRlcStats.txt files to ./output
2) Follow this commands:
> mkdir build

> cd build
 
> cmake ../

> make

> ./throughput_counter filename1 filename2

#### Example
> ./throughput_counter DlRlcStats.txt UlRlcStats.txt

Output:
Передающая пропускная способность: 1684.53 Кб/с
Принимающая пропускная способность: 1683.71 Кб/с
 
# Methods
## calculateThroughput
*returns data in Kbps*