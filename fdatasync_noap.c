#include <sys/mman.h>
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

//#define BUFSIZE 8192

int
main(int argc, char *argv[])
{
	int i_fd, w_fd, a_fd;
	struct timeval start_fsync, end_fsync, start_write, end_write, start_append, end_append; 
	int i, reps, BUFSIZE;
	unsigned long long ts1, ts2;	
	float bw_MB, secs;
	int*  int_buf;

	if (argc != 3) {
		fprintf(stderr, "Usage: %s reps num4kblocks\n", argv[0]);
		exit(1);
	}

	reps = atoi(argv[1]);
	BUFSIZE = atoi(argv[2])*1024; // for example 4*1024 for 4K
	
	
	// open input file 1MB in size with random data;  
	if ((i_fd = open("/mnt/pm0/ifile",  O_RDONLY)) < 0)
		err(1, "open failed");


	// allocate memory for the read buff
	int_buf = malloc(BUFSIZE*sizeof(int));
	
	// read  from the file
	read(i_fd, int_buf,BUFSIZE-1);
	
	close(i_fd);
	// start  buffer appends
	if ((a_fd = open("/mnt/pm0/ofile", O_WRONLY)) < 0)
		err(1, "open failed");
	
	for (i=0; i<reps; i++){

		gettimeofday(&start_append, NULL);	
		write(a_fd,int_buf,BUFSIZE);
		gettimeofday(&start_fsync, NULL);	
		fdatasync(a_fd);
		gettimeofday(&end_fsync, NULL);
		
		// Calculate Append+Fsync latencies
		ts1 = ((end_fsync.tv_sec - start_fsync.tv_sec) * 1000000) + (end_fsync.tv_usec - start_fsync.tv_usec);	
		ts2 = ((end_fsync.tv_sec - start_append.tv_sec) * 1000000) + (end_fsync.tv_usec - start_append.tv_usec);	
	
		// Calculate BW
		secs = (float) ts2/1000000.0;
		bw_MB = (float)BUFSIZE/(secs*1024.0*1024.0);	// 4k
	
 		printf(" fsync time = %lld u; append+ fsync times= %lld u; bandwidth= %f \n:",ts1, ts2, bw_MB);
	}
	/* flush the changes... */
	
close(a_fd);	

printf("done\n");
	exit(0);
}
