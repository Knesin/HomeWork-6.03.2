# Домашнее задание к занятию «Асинхронное программирование»

### Задание 2

#### «for_each»

* Реализовать параллельный вариант оператора for_each.<br/>
* Функция должна принимать итераторы на начало и конец контейнера и ссылку обрабатывающую функцию.<br/>
* При реализации необходимо рекурсивно разделить контейнер на несколько блоков и для каждого блока запустить отдельную задачу, применяющую обрабатывающую функцию к части контейнера.
