CREATE DATABASE skyacebase;
USE skyacebase;

CREATE TABLE base (
  id integer PRIMARY KEY AUTO_INCREMENT,
  title VARCHAR(255) NOT NULL,
  contents TEXT NOT NULL,
  created TIMESTAMP NOT NULL DEFAULT NOW()
);

INSERT INTO base (title, contents)
VALUES 
('My First Note', 'A note about something'),
('My Second Note', 'A note about something else');