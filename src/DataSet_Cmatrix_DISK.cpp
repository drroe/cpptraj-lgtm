#include "DataSet_Cmatrix_DISK.h"
#include "CpptrajStdio.h"
#include "StringRoutines.h" // ByteString

int DataSet_Cmatrix_DISK::AllocateCmatrix(size_t sizeIn) {
  if (Meta().Fname().empty()) {
    mprinterr("Internal Error: Cluster matrix file name not set.\n");
    return 1;
  }
  mprintf("\tPairwise cache file: '%s'\n", Meta().Fname().full());
  mprintf("\tEstimated pair-wise matrix disk usage: > %s\n",
          ByteString( ((sizeIn*(sizeIn-1))/2)*sizeof(float), BYTE_DECIMAL).c_str());
  if (file_.CreateCmatrix(Meta().Fname(), sievedFrames_.MaxFrames(), sizeIn,
                          sievedFrames_.Sieve(), MetricDescription()))
    return 1;
  // Write actual frames array if necessary
  if (sievedFrames_.Type() != ClusterSieve::NONE) {
    if (file_.WriteFramesArray( sievedFrames_.Frames() ))
      return 1;
  }
  // Reopen in SHARE mode for random access
  if (file_.ReopenSharedWrite( Meta().Fname() )) return 1;
  return 0;
}
