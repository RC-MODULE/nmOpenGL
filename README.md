# nmOpenGL

# Сборка
Для примера рассматривается проект 2nmc-demo-gcc, вариация для mc12101

## Makefile 

Чтобы собрать весь проект, необходимо вызвать `make`. Чтобы собрать какой-то один компонент, можно вызвать `make nm0`, `make nm1`, `make host`. Чтобы очистить весь проект необходимо вызвать `make clean`

TODO
Сделать отдельную очистку для каждого компонента


## Scons 

- Для работы надо установить [Scons 4.0.1](https://www.scons.org/doc/production/HTML/scons-user.html#chap-build-install): `pip install scons==4.0.1`
- Собирать в папке 2nmc-demo-gcc,
- Собрать цель target: `scons target`
- Создать решение для Visual Studio: `scons vs2015`
- Очистить сборку target: `scons target -c`

Можно так же использовать ключ -j4

### Сборка

Основные цели:
- nm0 - собирает программу для процессора с плавающей точкой NeuroMatrix
- nm1 - собирает программу для целочисленного процессора NeuroMatrix
- host - собирает хостовую часть
- vs2015 - собирает решение для Visual Studio





