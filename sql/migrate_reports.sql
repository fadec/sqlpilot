CREATE TABLE Reports (
       id INTEGER PRIMARY KEY AUTOINCREMENT
       ,Title CHAR
       ,SQL TEXT
);
create unique index reports_title on Reports(title);
