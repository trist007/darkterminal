#!/bin/sh
sqlite3 darkterminal.db <<EOF
create table user (id integer primary key,
username text not null,
password test not null);
insert into user (username, password) values ('john','smith');
EOF
