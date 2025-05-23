# Эксперименты с приоритетами в RT-планировщиках

Для лаконичности представлены сокращённые логи эксперимента, полные данные доступны в *chrt_experement.log*

## Общая настройка
Для анализа работы политик реального времени (SCHED_RR и SCHED_FIFO) использовалась следующая конфигурация:
- Docker-контейнер с ограничением CPU до 1 ядра (скрипт для запуска: [*`container.sh`*](./container.sh))
- Скрипт [*`spin.sh`*](./spin.sh) с busy-loop для имитации CPU-bound задач
- Утилиты `chrt` и `top` для управления приоритетами и мониторинга

## Стартовое состояние
После запуска контейнера наблюдаем стандартные процессы, далее в выводе top они будут исключены из отображения:
``` 
➜ priorities sudo ./container.sh 
[root@731a47df0af1 /]# cd src/

    PID USER      PR  NI    VIRT    RES    SHR S  %CPU %MEM     TIME+ COMMAND
      1 root      20   0   11844   2980   2596 S   0.0  0.0   0:00.02 bash
     92 root      20   0   56068   3704   3320 R   0.0  0.0   0:00.00 top
``` 

## Эксперимент 1: Два процесса с политикой CFS
Запускаем 2 экземпляра spin.sh:
``` 
[root@731a47df0af1 src]# ./spin.sh & ./spin.sh &

    PID USER      PR  NI    VIRT    RES    SHR S  %CPU %MEM     TIME+ COMMAND
     93 root      20   0   11704   2560   2432 R  53.3  0.0   0:37.32 spin.sh
     94 root      20   0   11704   2624   2496 R  46.7  0.0   0:37.31 spin.sh
```
CFS распределяет ~50% CPU между каждым из них.

## Эксперимент 2: SCHED_RR с одинаковыми приоритетами
Меняем политику на Round-Robin (приоритет 30):
```
[root@731a47df0af1 src]# chrt -rr -p 30 93
[root@731a47df0af1 src]# chrt -rr -p 30 94

    PID USER      PR  NI    VIRT    RES    SHR S  %CPU %MEM     TIME+ COMMAND
     94 root     -31   0   11704   2624   2496 R  50.0  0.0   1:43.96 spin.sh
     93 root     -31   0   11704   2560   2432 R  45.0  0.0   1:54.77 spin.sh
```
Распределение ~50% CPU сохраняется благодаря квантованию в RR.

## Эксперимент 3: Иерархия приоритетов в SCHED_RR
Повышаем приоритет процесса 94 до 50:
```
[root@731a47df0af1 src]# chrt -rr -p 50 94

    PID USER      PR  NI    VIRT    RES    SHR S  %CPU %MEM     TIME+ COMMAND
     94 root     -51   0   11704   2624   2496 R  95.0  0.0   2:28.59 spin.sh
     93 root     -31   0   11704   2560   2432 R   0.0  0.0   2:18.54 spin.sh
```
Процесс 94 монополизирует CPU из-за более высокого приоритета.

Инвертируем приоритеты (процессу 93 задаем 60):
```
[root@731a47df0af1 src]# chrt -rr -p 60 93

    PID USER      PR  NI    VIRT    RES    SHR S  %CPU %MEM     TIME+ COMMAND
     93 root     -61   0   11704   2560   2432 R  95.0  0.0   2:28.99 spin.sh
     94 root     -51   0   11704   2624   2496 R   0.0  0.0   2:53.29 spin.sh
```
Теперь процесс 93 получает весь CPU.

ℹ️ ***Примечание**: приоритетное планирование в SCHED_RR реализует строгую иерархию: процесс с высшим приоритетом монополизирует CPU, полностью исключая выполнение низкоприоритетных задач. Механизм ротации активируется только для процессов с равными приоритетами, распределяя между ними кванты времени. На двухпроцессной модели данное поведение демонстрируется ограниченно - более детальная визуализация будет представлена далее.*

## Эксперимент 4: SCHED_FIFO с одинаковыми приоритетами
Устанавливаем политику FIFO (приоритет 30):
```
[root@504a3ac9d3df src]# chrt -f -p 30 21
[root@504a3ac9d3df src]# chrt -f -p 30 20

    PID USER      PR  NI    VIRT    RES    SHR S  %CPU %MEM     TIME+ COMMAND
     21 root     -31   0   11704   2564   2436 R  95.0  0.0   3:53.50 spin.sh
     20 root     -31   0   11704   2520   2392 R   0.0  0.0   3:17.29 spin.sh
```
Процесс 21 блокирует CPU полностью из-за non-cooperative природы FIFO, следовательно, процесс 20 никогда не получит CPU ресурс.