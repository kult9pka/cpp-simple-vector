# cpp-simple-vector
Финальный проект: собственный контейнер вектор
## Разработка простого контейнера 
### simple_vector.h
Разработан шаблонный класс SimpleVector. Это сильно упрощённый аналог стандартного контейнера vector, со сходной структурой и функционалом.

Реализован функционал:
*	Конструкторы.
    * По умолчанию. Создаёт пустой вектор с нулевой вместимостью.
    * Параметризованный конструктор, создающий вектор заданного размера.
    * Конструктор из std::initializer_list.
*	Конструктор копирования. 
*	Оператор присваивания. 
*	Метод **GetSize** для получения количества элементов в векторе.
*	Метод **GetCapacity** для получения вместимости вектора.
*	Метод **IsEmpty**, сообщающий, пуст ли вектор.
*	Оператор **[]** для доступа к элементу вектора по его индексу.
*	Метод **At** для доступа к элементу вектора по его индексу.
*	Метод **Clear** для очистки массива без изменения его вместимости.
*	Метод **Resize** для изменения количества элементов в массиве. 
*	Методы **begin**, **end**, **cbegin** и **cend**, возвращающие итераторы на начало и конец массива.
*	Метод **PushBack**, добавляющий элемент в конец вектора. 
*	Метод **PopBack**, удаляющий последний элемент вектора. 
*	Метод **Insert**, вставляющий элемент в произвольное место контейнера. 
*	Метод **Erase**, удаляющий элемент в произвольной позиции вектора. 
*	Метод **swap**, обменивающий содержимое вектора с другим вектором.
*	Метод **Reserve**, задает ёмкость вектора.
*	Операторы **==** и **!=**.
*	Операторы **<**, **>**, **<=**, **>=**, выполняющие лексикографическое сравнение содержимого двух векторов.
*	Поддержка семантики перемещения
### array_ptr.h
Разработан шаблонный класс ArrayPtr, играющий роль умного указателя на массив в динамической памяти.

Реализован функционал:
*	Удаление массива при разрушении умного указателя;
*	Конструкторы: 
    * По умолчанию.
    * Из указателя на существующий массив, создающий новый массив заданного размера
*	Доступ к элементу массива по индексу;
*	Запрет операций копирования и присваивания;
*	Метод **swap** для обмена содержимым с другим объектом ArrayPtr;
*	Метод **Release**, прекращающий владение массивом и возвращающий значение сырого указателя.
