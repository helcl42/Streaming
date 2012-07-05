
Create table Artist (
      id Integer Not Null,
      name VarChar(255) Not Null,
      Constraint PK_Artist Primary Key (id)
) ENGINE=INNODB;
Create table Album (
      by_id Integer Null,
      id Integer Not Null,
      title VarChar(255) Not Null,
      Constraint PK_Album Primary Key (id)
) ENGINE=INNODB;
Create table Song (
      is_on_id Integer Null,
      id Integer Not Null,
      title VarChar(255) Not Null,
      length Integer Not Null,
      url VarChar(255) Not Null,
      Constraint PK_Song Primary Key (id)
) ENGINE=INNODB;

Alter table Album add (
      Constraint FK_Album_1 Foreign Key (by_id) References Artist(id)
);
Alter table Song add (
      Constraint FK_Song_1 Foreign Key (is_on_id) References Album(id)
);


