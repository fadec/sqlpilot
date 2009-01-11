CREATE TABLE Registry (
       path CHAR
       ,key INTEGER		-- integer affinity for sort
       ,value INTEGER
);
create unique index registry_path_key on Registry(path, key);
