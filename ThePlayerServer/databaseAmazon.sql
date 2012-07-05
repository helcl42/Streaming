
INSERT INTO Artist (`id`, `name`) VALUES (1, 'unsorted');
INSERT INTO Artist (`id`, `name`) VALUES (2, 'Led Zeppelin');
INSERT INTO Artist (`id`, `name`) VALUES (3, 'Foo Fighters');
INSERT INTO Artist (`id`, `name`) VALUES (4, 'David Koller');

INSERT INTO Album (`by_id`, `id`, `title`) VALUES (1, 1, 'unsorted');

INSERT INTO Album (`by_id`, `id`, `title`) VALUES (2, 2, 'Houses of the Holy');
INSERT INTO Album (`by_id`, `id`, `title`) VALUES (2, 3, 'Led Zeppelin IV');
INSERT INTO Album (`by_id`, `id`, `title`) VALUES (2, 4, 'Led Zeppelin II');
INSERT INTO Album (`by_id`, `id`, `title`) VALUES (2, 5, 'Led Zeppelin I');
INSERT INTO Album (`by_id`, `id`, `title`) VALUES (2, 6, 'Physical Graffiti');
INSERT INTO Album (`by_id`, `id`, `title`) VALUES (2, 7, 'In Through the Out Door');

INSERT INTO Album (`by_id`, `id`, `title`) VALUES (3, 8, 'One by One');

/* Led Zeppelin */
INSERT INTO Song (`is_on_id`, `id`, `title`, `length`, `url`) VALUES (2, 1, 'Led Zeppelin - 04 No Quarter', 460, '/home/ubuntu/streamer/uploads/Led Zeppelin/Houses of the Holy/Led Zeppelin - 04 No Quarter.flac');

INSERT INTO Song (`is_on_id`, `id`, `title`, `length`, `url`) VALUES (3, 2, 'Led Zeppelin - 15 Stairway To Heaven', 460, '/home/ubuntu/streamer/uploads/Led Zeppelin/Led Zeppelin IV/Led Zeppelin - 15 Stairway To Heaven.flac');

INSERT INTO Song (`is_on_id`, `id`, `title`, `length`, `url`) VALUES (4, 3, 'Led Zeppelin - 05 Whole Lotta Love', 460, '/home/ubuntu/streamer/uploads/Led Zeppelin/Led Zeppelin II/Led Zeppelin - 05 Whole Lotta Love.flac');
INSERT INTO Song (`is_on_id`, `id`, `title`, `length`, `url`) VALUES (4, 4, 'Led Zeppelin - 06 Heartbreaker', 460, '/home/ubuntu/streamer/uploads/Led Zeppelin/Led Zeppelin II/Led Zeppelin - 06 Heartbreaker.flac');
INSERT INTO Song (`is_on_id`, `id`, `title`, `length`, `url`) VALUES (4, 5, 'Led Zeppelin - 07 Ramble On', 460, '/home/ubuntu/streamer/uploads/Led Zeppelin/Led Zeppelin II/Led Zeppelin - 07 Ramble On.flac');

INSERT INTO Song (`is_on_id`, `id`, `title`, `length`, `url`) VALUES (5, 6, 'Led Zeppelin - 01 Communication Breakdown', 460, '/home/ubuntu/streamer/uploads/Led Zeppelin/Led Zeppelin I/Led Zeppelin - 01 Communication Breakdown.flac');

INSERT INTO Song (`is_on_id`, `id`, `title`, `length`, `url`) VALUES (6, 7, 'Led Zeppelin - 05 Houses Of The Holy', 460, '/home/ubuntu/streamer/uploads/Led Zeppelin/Physical Graffiti/Led Zeppelin - 05 Houses Of The Holy.flac');
INSERT INTO Song (`is_on_id`, `id`, `title`, `length`, `url`) VALUES (6, 8, 'Led Zeppelin - 06 Kashmir', 460, '/home/ubuntu/streamer/uploads/Led Zeppelin/Physical Graffiti/Led Zeppelin - 06 Kashmir.flac');

INSERT INTO Song (`is_on_id`, `id`, `title`, `length`, `url`) VALUES (7, 9, 'Led Zeppelin - 10 All My Love', 460, '/home/ubuntu/streamer/uploads/Led Zeppelin/In Through the Out Door/Led Zeppelin - 10 All My Love.flac');

/* Foo Fighters */
INSERT INTO Song (`is_on_id`, `id`, `title`, `length`, `url`) VALUES (8, 10, 'Foo Fighters - Arlandria', 460, '/home/ubuntu/streamer/uploads/Foo Fighters/One by One/Foo Fighters - Arlandria.mp3');

