
INSERT INTO Artist (`id`, `name`) 
	VALUES (1, 'unsorted'), 
		  (2, 'Led Zeppelin'),
		  (3, 'David Koller'),
		  (4, 'Foo Fighters');

INSERT INTO Album (`by_id`, `id`, `title`) 
	VALUES (1, 1, 'unsorted'), 
		  (2, 2, 'Houses of the Holy'), 
		  (2, 3, 'Physical Graffiti'),
		  (2, 4, 'Led Zeppelin IV'),		
		  (3, 5, 'David Koller - Ted a tady (2010)'),
		  (4, 6, 'Arlandria');

INSERT INTO Song (`is_on_id`, `id`, `title`, `length`, `url`) 
	VALUES (6, 1, 'Foo Fighters - Arlandria', 400, '/home/lubos/Desktop/music/Foo Fighters/One by One/Foo Fighters - Arlandria.mp3'),
		  (5, 2, '01 Zacnu s tebou - David Koller', 500, '/home/lubos/Desktop/music/David Koller - Ted a tady (2010)/01 Zacnu s tebou - David Koller.mp3'),
		  (5, 3, '02 Ty a ja - David Koller', 400, '/home/lubos/Desktop/music/David Koller - Ted a tady (2010)/02 Ty a ja - David Koller.mp3'),
		  (5, 4, '03 Ted a tady - David Koller', 400, '/home/lubos/Desktop/music/David Koller - Ted a tady (2010)/03 Ted a tady - David Koller.mp3'),
         	  (4, 5, 'Led Zeppelin - 06 Heartbreaker', 430, '/home/lubos/Desktop/music/Led Zeppelin/Led Zeppelin IV/Led Zeppelin - 06 Heartbreaker.flac'),
	          (4, 6, 'Led Zeppelin - 07 Ramble On', 450, '/home/lubos/Desktop/music/Led Zeppelin/Led Zeppelin IV/Led Zeppelin - 07 Ramble On.flac'),
		  (2, 7, 'Led Zeppelin - 01 Communication Breakdown', 450, '/home/lubos/Desktop/music/Led Zeppelin/Houses of the Holy/Led Zeppelin - 01 Communication Breakdown.flac'),
		  (2, 8, 'Led Zeppelin - 05 Whole Lotta Love', 450, '/home/lubos/Desktop/music/Led Zeppelin/Houses of the Holy/Led Zeppelin - 05 Whole Lotta Love.flac'),
		  (2, 9, 'Led Zeppelin - 15 Stairway To Heaven', 450, '/home/lubos/Desktop/music/Led Zeppelin/Houses of the Holy/Led Zeppelin - 15 Stairway To Heaven.flac'),
		  (3, 10, 'Led Zeppelin - 04 No Quarter', 460, '/home/lubos/Desktop/music/Led Zeppelin/Physical Graffiti/Led Zeppelin - 04 No Quarter.flac'),
		  (3, 11, 'Led Zeppelin - 05 Houses Of The Holy', 460, '/home/lubos/Desktop/music/Led Zeppelin/Physical Graffiti/Led Zeppelin - 05 Houses Of The Holy.flac'),
		  (3, 12, 'Led Zeppelin - 06 Kashmir', 440, '/home/lubos/Desktop/music/Led Zeppelin/Physical Graffiti/Led Zeppelin - 06 Kashmir.flac'),
		  (3, 13, 'Led Zeppelin - 10 All My Love', 440, '/home/lubos/Desktop/music/Led Zeppelin/Physical Graffiti/Led Zeppelin - 10 All My Love.flac'),
		  (1, 14, 'Mana - Nothing particular-04', 440, '/home/lubos/Desktop/music/unsorted/unsorted/Mana - Nothing particular-04.ogg'),
		  (1, 15, 'WhatsThat03', 440, '/home/lubos/Desktop/music/unsorted/unsorted/WhatsThat03.wma');

