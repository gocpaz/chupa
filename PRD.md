# Вступление #

Техническое задание. Документ может изменяться на начальном этапе, т.к. не все возможности библиотек определены.

# Цели документа #

Цель данного документа - составить неизменяемое техническое задание. Т.к. не все определено (данные, возможности библиотек, человеческий фактор), цель данного документа перевести его (документ) в режим стабильности и редкого изменения.

# Причины #

Данный проект создан как план обучения Nikolya. Учитывая совершенно не "академический" возраст, а также то, что в случае провала проекта, обучение Nikolya уйдёт в небытие, решено делать проект, который хоть как то может иметь "коммерческий" успех - другими словами пригодится людям. Срок реализации 1-го уровня проекта  - 1 год (релиз для инета). Проект должен обладать качественными модулями, которые могут использоваться не зависимо от успеха самого проекта.

# Аудитория #

1-й уровень проекта предназначен для людей, которые хотят в онлайн режиме видеть состояние оборудования (компьютеров, комутационного оборудования сети). Предполагается что кол-во оборудования  присутствуют в количестве 3-х и больше. Задача пользователя - в кратчайшие сроки увидеть проблему (отсутсвие связи) либо просмотреть параметры оборудования на текущий момент (температура, загруженность процессора, и т.д.). Использование программы предполагается эпизодическим, не для проф использования, а для выявления текущих проблем, которые пользователь для себя ставит.

# Масшабируемость #

Проект разбит на 3 этапа.

1 уровень. Создание программы, которая по протоколу SNMP может в онлайн режиме просматривать стадартные параметры устройств. Под стандартными параметрами устройств подразумевается найболее часто ипользуемые показатели. Не зависимо от этого (никаких если, никаких но), разработка должна подразумевать 2-й и 3-й уровень развития. Ни сохранение данных, ни работа в фоновом режиме (режим демона или сервиса) не предусматривается.

2 уровень. Проект имеет серверную (сервис) и клиентскую часть. Причём обязан работать и на уровне 1 - т.е. использование онлайн просмотра не подразумевает использование серверной части. Серверная часть отвечает за: записывание данных в БД для дальнейшего анализа, генерация внешних событий (отправка почты по событию, выполнение внешней программы).

3 уровень. Унификация серверной и клиентской части с существующими технологиями. Схему можно представить в виде SVG WEB сервиса, сохранение схемы должно соответвовать стандартам. И т.д.

Нужно понять, что наличием уровня 3 предполагает глубокие масштабы производительности как внутренних ресурсов так и сетевых. Т.е. от 3-х устройств до тысячи.

Важно учитывать, что уровни 2 и 3 схожи между собой, но переход сразу на 3-й уровень череват затратами на разработку. Уровень 2 нельзя считать промежуточным, учитывая "подводные" камни какие встретятся. Проект разрабатывается на стыке многих технологий (особенно учитывая что знаний 0), поэтому этап 2 - это координиция общения между модулями, классами, унификация сообщений, но в пределах целесообразности проекта. Уровень же 3 - перевод целесообразности в стандарты.

# Человеческое описание #

Тут важно понять, что мы хотим и к чему стремимся. Простым языком - мы сидим на кухне, у нас нарисована примитивная схемка и мы хотим контроллировать весь процесс наших устройств. Сама по себе "телемеханика" подразумевает, что пользователь должен обладать знаниями в предметной области. Кухня это или предприятие - не суть важно. Важно одно - быстро определить наличие, а если и возможно, причину проблемы. Реализация данных возможностей - это графическое отображение устройств на схеме со связями, которые графически показывают состояние объекта (в виде изменение цвета либо числовыми значениями). Другими словами пользователь может поместить устройства на схеме и наблюдать за их поведением.

# Системные требования #
На данном этапе несущественны.

# Выбор языка и среды программирования #
Qt и C++. Не важно почему.

# Модули (противное название, может на пакет?) #
Под модулями в данном проекте подразумевается логическая часть проекта, которая мало завист от остальной части программы (в виде прямой связи хедеров). Обратная зависимоть допускается. Модули не могут быть зависимыми от других модулей циклически, хотя допускается иерархия. Модули должны быть оформлены в отдельном каталоге. Некоторые модули, которые могут быть полностью независимы от проекта (например Qt-SNMP), должны компилироваться в библиотеки, хотя внутри проекта это не обязательно.

# Список модулей и зависимости #
Здесь стоит доделать документ в виде графической зависимости модулей.

1. Внешняя библиотека для работы с SNMP. Она не является непосредственно модулем программы, но есть прямая зависимотсть. Обзор показал, что единсвенный приемлемый вариант использовать библиотеку NET-SNMP. Также стоит определиться (если возможно), использование NET-SNMP в виде статической линковки.

2. Модуль Qt-SNMP (биндинг snmp либы). На самом деле не такая простая задача, как кажется на первый взгляд. Стоит хорошо разобраться как с идеологией SNMP так и с идеологией самой библиотеки NET-SNMP

3. Модуль работы с GUI. Основная задача модуля на первом этапе - векторный графический редактор схем.

4. Ядро системы. Опрос по SNMP всех устройств, отсылка сообщений как к БД (в будущем) так и интерактивной схеме. Не смотря на то, что от модуля зависят все остальные компоненты системы, предпологается сделать модуль максимально независмым.

# Этапы разработки #

Учитывая много неопределённых переменных, этапы разработки будут происходить через тестирование. Сначало тестовая программа, затем разработка дальше (https://ru.wikipedia.org/wiki/%D0%A0%D0%B0%D0%B7%D1%80%D0%B0%D0%B1%D0%BE%D1%82%D0%BA%D0%B0_%D1%87%D0%B5%D1%80%D0%B5%D0%B7_%D1%82%D0%B5%D1%81%D1%82%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D0%B5) (заодно спрячь эту хрень  - я про ссылку и назначь ей какой то текст, в данном случае "через тестирование" и не гребёт, что не знаешь как спрятать, учись быстро искать решения). Но тут нужно учитывать академический момент - невозможно начать такую разработку сразу, не имеея опыта и видинья конечной цели. Поэтому, первый этап, будет наработка кода, после понимания тестирования.

# Что дальше с документом #
Описать все модули отдельными документами.