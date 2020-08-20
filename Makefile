
all: fsync fsync_noap fdatasync fdatasync_noap

fsync: fsync.c
	gcc -o fsync fsync.c -O2

fsync_noap: fsync_noap.c
	gcc -o fsync_noap fsync_noap.c -O2

fdatasync: fdatasync.c
	gcc -o fdatasync fdatasync.c -O2

fdatasync_noap: fdatasync_noap.c
	gcc -o fdatasync_noap fdatasync_noap.c -O2

clean:
	rm fsync fsync_noap fdatasync fdatasync_noap
