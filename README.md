# StGen
Statement Generator - удобное общение с базой данных по основным операциям - создание/удаление/обновление таблиц, запросы

# Пример

Создаём подключение к базе с именем `dataBaseName`
```
SqliteInterface b(DataBaseSettings(dataBaseName), false); 
TestBase base(&b); 
StGenGlobal::setBuilder(StGen::createSqlBuilder(&b)); 
using namespace StGenGlobal;
```

Создаём таблицу с двумя полями
```
createTable("table1")
  .addColumn("id", ColumnType::Integer())
  .addColumn("value", ColumnType::String(5))
  .exec();
```
Добавляем данные в таблицу пачкой
```
insert("id", "value")
                .into("table1")
                .values(1, "aaaaa")
                .values(2, "bbbbb")
                .values(3, "ccccc")
                .exec();
```  
Удаляем записи из таблицы
```
deleteFrom("table1").where(in("id", {1,3})).exec();
```
Получаем данные из таблицы
```
const QueryResult result = select("value").from("table1").exec();
if(result.error().isEmpty())
{
  while(result.next())
  {
    auto value = result.value(0).toString(); /// "bbbbb
    ...
  }
}
```
Проверяем, что нет ошибок
```
b.hasNoError()
```
