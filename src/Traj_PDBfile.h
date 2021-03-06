#ifndef INC_TRAJ_PDBFILE_H
#define INC_TRAJ_PDBFILE_H
#include "TrajectoryIO.h"
#include "PDBfile.h"
// Class: Traj_PDBfile
/// TrajectoryIO class for reading coordinates from PDB files.
class Traj_PDBfile: public TrajectoryIO {
  public:
    // NOTE: PDBWRITEMODE must remain public for pytraj.
    /** PDBWRITEMODE: Indicate how the pdb should be written.
      *  SINGLE: Writing only a single frame.
      *  MODEL: Multiple frames written to the same file separated with 
      *         the MODEL keyword
      *  MULTI: Each frame written to a different file with name filename.frame
      */
    enum PDBWRITEMODE {NONE = 0, SINGLE, MODEL, MULTI};

    Traj_PDBfile();
    static BaseIOtype* Alloc() { return (BaseIOtype*)new Traj_PDBfile(); }
    static void WriteHelp();
  private:
    // Inherited functions
    bool ID_TrajFormat(CpptrajFile&);
    int setupTrajin(FileName const&, Topology*);
    int setupTrajout(FileName const&, Topology*, CoordinateInfo const&,int, bool);
    int openTrajin();
    void closeTraj();
    int readFrame(int,Frame&);
    int writeFrame(int,Frame const&);
    void Info();
    int processWriteArgs(ArgList&);
    int readVelocity(int, Frame&) { return 1; }
    int readForce(int, Frame&)    { return 1; }
    int processReadArgs(ArgList&) { return 0; }
#   ifdef MPI
    // Parallel functions
    int parallelOpenTrajout(Parallel::Comm const&);
    int parallelSetupTrajout(FileName const&, Topology*, CoordinateInfo const&,
                             int, bool, Parallel::Comm const&);
    int parallelWriteFrame(int, Frame const&);
    void parallelCloseTraj() {}
#   endif
    void WriteDisulfides(Frame const&);
    void WriteBonds();

    typedef std::vector<int> Iarray;
    typedef PDBfile::SSBOND SSBOND;
    enum TER_Mode { BY_MOL = 0, BY_RES, ORIGINAL_PDB, NO_TER };
    enum Radii_Mode { GB = 0, PARSE, VDW };
    enum CONECT_Mode { NO_CONECT = 0, HETATM_ONLY, ALL_BONDS };
    Radii_Mode radiiMode_;   ///< Radii to use if PQR.
    TER_Mode terMode_;       ///< TER card mode.
    CONECT_Mode conectMode_; ///< CONECT record mode.
    PDBWRITEMODE pdbWriteMode_;
    int pdbAtom_;
    int currentSet_;
    int ter_num_;       ///< Amount to increment atom number for TER
    bool dumpq_;        ///< If true print charges/radii in Occupancy column (PQR).
    bool pdbres_;       ///< If true convert Amber res names to PDBV3 style.
    bool pdbatom_;      ///< If true convert Amber atom names to PDBV3 style.
    bool write_cryst1_; ///< If false write CRYST1 for first frame only.
    bool include_ep_;   ///< If true include extra points.
    bool prependExt_;
    bool firstframe_;   ///< Set to false after first call to writeFrame
    std::string space_group_;
    std::vector<double> radii_;  ///< Hold radii for PQR format.
    Iarray TER_idxs_;  ///< TER card indices.
    Iarray atrec_;     ///< Hold ATOM record #s for CONECT
    std::vector<bool> resIsHet_; ///< True if residue needs HETATM records
    std::vector<SSBOND> ss_residues_;
    Iarray ss_atoms_;
    Topology *pdbTop_;
    PDBfile file_;
    std::vector<char> chainID_;      ///< Hold chainID for each residue.
    std::vector<NameType> resNames_; ///< Hold residue names.
    char chainchar_;
};
#endif
