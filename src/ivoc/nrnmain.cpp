#include <../../nrnconf.h>
#include <stdio.h>
#include <string.h>
#include "../nrncvode/nrnneosm.h"
#include <nrnmpi.h>
#include <errno.h>

#include <caliper/cali.h>

extern "C" {
	int nrn_isdouble(double*, double, double);
	int ivocmain(int, char**, char**);
	extern int nrn_main_launch;
#if NRNMPI_DYNAMICLOAD
	extern void nrnmpi_stubs();
	extern char* nrnmpi_load(int is_python);
#endif
#if BLUEGENE_CHECKPOINT
	void BGLCheckpointInit(char* chkptDirPath);
	// note: get the path from the environment variable BGL_CHKPT_DIR_PATH
	// otherwise from $HOME/checkpoint, otherwise $HOME
#endif	
}

int main(int argc, char** argv, char** env) {
	nrn_isdouble(0,0,0);
	nrn_main_launch = 1;
#if 0
printf("argc=%d\n", argc);
for (int i=0; i < argc; ++i) {
printf("argv[%d]=|%s|\n", i, argv[i]);
}
#endif
#if NRNMPI
#if NRNMPI_DYNAMICLOAD
	nrnmpi_stubs();
	for (int i=0; i < argc; ++i) {
		if (strcmp("-mpi", argv[i]) == 0) {
			char* pmes;
			pmes = nrnmpi_load(0);
			if (pmes) {
				printf("%s\n", pmes);
				exit(1);
			}
			break;
		}
	}
#endif
	nrnmpi_init(1, &argc, &argv); // may change argc and argv
#endif	
#if BLUEGENE_CHECKPOINT
	BGLCheckpointInit((char*)0);
#endif
	errno = 0;
    CALI_MARK_BEGIN("main");
	int status = ivocmain(argc, argv, env);
    CALI_MARK_END("main");
    return status;
}

#if USENCS
extern "C" {
void nrn2ncs_outputevent(int, double){}
}
#endif

// moving following to src/oc/ockludge.c since on
// Darwin Kernel Version 8.9.1 on apple i686 (and the newest config.guess
// thinks it is a i386, but that is a different story)
// including mpi.h gives some errors like:
// /Users/hines/mpich2-1.0.5p4/instl/include/mpicxx.h:26:2: error: #error 
// SEEK_SET is #defined but must not be for the C++ binding of MPI"

#if 0 && NRNMPI && DARWIN
// For DARWIN I do not really know the proper way to avoid
// dyld: lazy symbol binding failed: Symbol not found: _MPI_Init
// when the MPI functions are all used in the libnrnmpi.dylib
// but the libmpi.a is statically linked. Therefore I am forcing
// the linking here by listing all the MPI functions being used.
#include <mpi.h>
static void work_around() {
	MPI_Comm c = MPI_COMM_WORLD;
	MPI_Init(0, 0);
	MPI_Comm_rank(c, 0);
	MPI_Comm_size(c, 0);
	MPI_Wtime();
	MPI_Finalize();
	MPI_Unpack(0, 0, 0, 0, 0, 0, c);
	MPI_Pack(0, 0, 0, 0, 0, 0, c);
	MPI_Pack_size(0, 0, c, 0);
	MPI_Send(0,0,0,0,0,c);
	MPI_Probe(0, 0, c, 0);
	MPI_Get_count(0, 0, 0);
	MPI_Recv(0,0,0,0,0,c,0);
	MPI_Sendrecv(0,0,0,0,0,0,0,0,0,0,c,0);
	MPI_Iprobe(0,0,c,0,0);
	MPI_Address(0,0);
	MPI_Type_struct(0,0,0,0,0);
	MPI_Type_commit(0);
	MPI_Allgather(0,0,0,0,0,0,c);
	MPI_Allgatherv(0,0,0,0,0,0,0,c);
	MPI_Allreduce(0,0,0,0,0,c);
}
#endif
