/*DATABASE FOR AUTOMATA*/

CREATE DATABASE Automata;
USE Automata;

CREATE TABLE FA(
	id INT PRIMARY KEY AUTO_INCREMENT ,
	description TEXT
);
CREATE TABLE alphabet(
	fa_id INT NOT NULL,
	symbol VARCHAR(5) NOT NULL ,
    PRIMARY KEY (fa_id,symbol),
	FOREIGN KEY (fa_id) REFERENCES fa(id) ON DELETE CASCADE 
);
CREATE TABLE states(
	fa_id	INT NOT NULL,
	state INT NOT NULL,
	start_state	INT NOT NULL,	#only 0 & 1 where 1 is true & 0 is not
	final_state INT NOT NULL,	#only 0 & 1 where 1 is true & 0 is not
    PRIMARY KEY (fa_id,state),
	FOREIGN KEY (fa_id) REFERENCES fa(id) ON DELETE CASCADE
);
CREATE TABLE transitions(
	fa_id 	INT NOT NULL,
	state	INT NOT NULL,
	symbol VARCHAR(5) NOT NULL ,
	next_state INT NOT NULL,
	PRIMARY KEY (fa_id,state,symbol,next_state),
    FOREIGN KEY (fa_id) REFERENCES fa(id),
	FOREIGN KEY (fa_id,state) REFERENCES states(fa_id,state),
	FOREIGN KEY (fa_id,symbol) REFERENCES alphabet(fa_id,symbol),
    FOREIGN KEY (fa_id,next_state) REFERENCES states(fa_id,state)
);
INSERT INTO fa (description) VALUES 
	('L={ab^n| n>=0 }, X={a,b}'), #fa 1automata
	('L={W|W is start with a}, X={a,b}'), #fa 2
	('L={W|W begin with ab and end in ba}, X={a,b}'), #fa 3
	('L={(10)^n|n>=0}'), #fa4
   ('L={W|W of all over {0,1} that end with 0110, 010, 00}'), #fa5
   ('L={(a|b)^n, n is odd}'); #fa6
INSERT INTO alphabet (fa_id, symbol) VALUES 
	(1,'a'), #for fa 1
	(1,'b'),
	(2,'a'),#for fa 2
	(2,'b'),
	(3,'a'),#for fa 3
	(3,'b'),
	(4,'a'), #for fa 4
	(4,'b'),
    (4,'e'),
    (5,'0'), #for fa 5
	(5,'1'),
	(5,'e'),
    (6,'a'), #for fa 6
	(6,'b');
INSERT INTO states (fa_id, state, start_state ,final_state) VALUES 
	(1, '0', 1, 0), #for fa 1
	(1, '1', 0, 1),
	(1, '2', 0, 0),
	(2, '0', 1, 0), #for fa 2
	(2, '1', 0, 1),
	(2, '2', 0, 0),
	(3, '0', 1, 0), #for fa 3
	(3, '1', 0, 0),
	(3, '2', 0, 0),
	(3, '3', 0, 0),
	(3, '4', 0, 1),
    (4, '0', 1, 0), #for fa 4
	(4, '1', 0, 0),
	(4, '2', 0, 1),
    (5, '0', 1, 0), #for fa 5
	(5, '1', 0, 0),
	(5, '2', 0, 0),
	(5, '3', 0, 0),
	(5, '4', 0, 1),
    (6, '0', 1, 0), #for fa 6
	(6, '1', 0, 1),
	(6, '2', 0, 0),
	(6, '3', 0, 1),
	(6, '4', 0, 0);
    
INSERT INTO transitions (fa_id, state ,symbol, next_state) VALUES 
	(1, 0, 'a',0),	#for fa 1
	(1, 0,'b', 1),
	(1, 1,'a', 0),
	(1, 1,'b', 2),
	(1, 2,'a', 0),
	(1, 2,'b', 2),
	(2, 0,'a',1),  #for fa 2
	(2, 0,'b', 2),
	(2, 1,'a', 1),
	(2, 1,'b', 1),
	(2, 2,'a', 2),
	(2, 2,'b', 2),
    (3, 0,'a', 1),	 #for fa 3
	(3, 1,'b', 2),
	(3, 1,'b', 3),
	(3, 2,'a', 2),
	(3, 2,'b', 2),
	(3, 2,'b', 3),
	(3, 3,'a', 4),
    (4, 0,'b', 1),	#for fa 4
	(4, 0,'e', 2),
	(4, 1,'a', 2),
	(4, 2,'b', 1),
    (5, 0,'0', 0),	#for fa 5
	(5, 0,'0', 1),
	(5, 0,'1', 0),
	(5, 1,'1', 2),
	(5, 1,'e', 2),
	(5, 2,'1', 3),
	(5, 2,'e', 3),
	(5, 0,'0', 4),
    (6, 0,'a', 1),  #for fa 6
	(6, 0,'b', 1),
	(6, 1,'a', 2),
	(6, 1,'b', 2),
	(6, 2,'a', 3),
	(6, 2,'b', 3),
	(6, 3,'a', 2),
	(6, 3,'b', 2),
	(6, 4,'a', 3),
	(6, 4,'b', 2);

