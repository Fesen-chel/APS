# АРХИТЕКТУРА ПРОГРАММНЫХ СИСТЕМ
## Исходные данные
### Вариант 12
- ### Источники
  - ИБ — источник бесконечный
  - ИЗ2 — равномерный закон генерации заявок
- ### Приборы
  - ПЗ1 — экспоненциальный закон обслуживания заявок
- ### Дисциплина буферизации
  - Д10З3 — заполнение буфера на свободное место
- ### Дисциплина отказа
  - Д10О4 — последняя поступившая в буфер заявка получает отказ в случае переполнения
- ### Дисциплина выбора прибора
  - Д2П2 — выбор прибора для обслуживания по кольцу
- ### Дисциплина выбора заявки на обслуживание
  - Д2Б3 — выбор заявки на обслуживание по кольцу
- ### Динамическое отражение результатов
  - ОД3 — временные диаграммы, текущее состояние;
- ### Отображение результатов после сбора статистики
  - ОР1 — сводная таблица результатов
 
## Описание системы
### Бизнес-домен
Онлайн-система оценки предварительной стоимости ремонта автомобиля после ДТП
### Маппинг бизнес-домена на компоненты СМО
1.	Источники – владельцы автомобилей, участники ДТП, которые хотят рассчитать предварительную стоимость ремонта
2.	Заявка – запрос, содержащий серию фото, для оценки ущерба
3.	Диспетчер постановки – система управления очередью, которая заполняет буфер. Если буфер заполнен, то удаляет последнюю поступившую в буфер заявку 
4.	Буфер ¬¬– виртуальная очередь заявок, ожидающих обработки. Новая заявка ставится на первое с начала свободное место, если такое найдется, сдвига очереди не происходит
5.	Диспетчер выборки – система распределения заявок, которая выбирает заявку из буфера по кольцу и распределяет задачи приборам по кольцу.
6.	Прибор – специалист-оценщик, который выносит вердикт по заявке

## Диаграма классов
![classes_diagram.png](https://github.com/Fesen-chel/APS/blob/main/classes_diagram.png)
