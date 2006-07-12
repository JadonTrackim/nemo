// -*- C++ -*-                                                                  
////////////////////////////////////////////////////////////////////////////////
///                                                                             
/// \file   src/manip/density.cc                                                
///                                                                             
/// \author Walter Dehnen                                                       
/// \date   2006                                                                
///                                                                             
////////////////////////////////////////////////////////////////////////////////
//                                                                              
// Copyright (C) 2006 Walter Dehnen                                             
//                                                                              
// This is a non-public part of the code.                                       
// It is property of its author and not to be made public without his written   
// consent.                                                                     
//                                                                              
////////////////////////////////////////////////////////////////////////////////
//                                                                              
// history:                                                                     
//                                                                              
// v 0.0    27/04/2006  WD created                                              
// v 0.1    07/07/2006  WD replacing bodyset with flags::ignore                 
////////////////////////////////////////////////////////////////////////////////
#include <public/defman.h>
#include <public/io.h>
#include <proper/neighbours.h>
#include <ctime>

namespace falcON { namespace Manipulate {
  //////////////////////////////////////////////////////////////////////////////
  //                                                                          //
  // auxiliary data and function                                              //
  //                                                                          //
  //////////////////////////////////////////////////////////////////////////////
  real FAC;
  void dens(const bodies*                    B,
	    const NeighbourLister::Leaf*     L,
	    const NeighbourLister::Neighbour*N,
	    int                              K)
  {
    real m(zero);
    const NeighbourLister::Neighbour*NK = N+K;
    for(const NeighbourLister::Neighbour*n=N; n!=NK; ++n)
      m += B->mass(mybody(n->L));
    B->rho(mybody(L)) = FAC*m/cube(max_dist(L));
  }
  // ///////////////////////////////////////////////////////////////////////////
  //                                                                            
  // class density                                                              
  //                                                                            
  /// manipulator: estimates density using nearest neighbour count              
  ///                                                                           
  /// This manipulator finds for each body in_subset (default: all, see         
  /// set_subset) the Kth nearest neighbours (from the same set of bodies) and  
  /// estimates the mass density, which is written into field 'r'.\n            
  /// This procedure is quite expensive (more than computing gravity) and hence 
  /// only done every STEP time units.\n                                        
  ///                                                                           
  /// Meaning of the parameters:\n                                              
  /// par[0]: # nearest neighbours used in density estimate (def: 16)\n         
  /// par[1]: delta time between estimation (which takes long; def: 0)\n        
  ///                                                                           
  /// Usage of pointers: none\n                                                 
  /// Usage of flags:    uses in_subset()\n                                     
  ///                                                                           
  // ///////////////////////////////////////////////////////////////////////////
  class density : public manipulator {
  private:
    int             K;        ///< # neighbours
    double          STEP;     ///< delta time between manipulations
    mutable double  TMAN;     ///< time for next manipulation
    mutable bool    FST;      ///< first call to manipulate() ?
    //--------------------------------------------------------------------------
  public:
    const char* name    () const { return "density"; }
    const char* describe() const {
      return message("estimates density using %dth nearest neighbour",K);
    }
    //--------------------------------------------------------------------------
    fieldset need   () const { return fieldset::f | fieldset::m | fieldset::x; }
    fieldset provide() const { return fieldset::r; }
    fieldset change () const { return fieldset::o; }
    //--------------------------------------------------------------------------
    bool manipulate(const snapshot*) const;
    //--------------------------------------------------------------------------
    density(const double*pars,
	    int          npar,
	    const char  *file) falcON_THROWING
    : K    ( npar>0?     int(pars[0])    : 16 ),
      STEP ( npar>1?         pars[1]     : 0. ),
      FST  ( true )
    {
      if(npar==0 && debug(1) || debug(2))
	std::cerr<<
	  " Manipulator \""<<name()<<"\":\n"
	  " estimates density of bodies in_subset() (default: all)"
	  " and writes it to field r\n parameters:\n"
	  " par[0]: # nearest neighbours used in density estimate (def: 16)\n"
	  " par[1]: delta time between estimation (which takes long; def: 0)\n";
      if(K<=0)
	falcON_THROW("Manipulator \"%s\": "
		     "# neighbours (%d) must be positive",name(),K);
      if(file)
	warning("Manipulator \"%s\": file given but not used\n",name());
      if(npar>2)
	warning("Manipulator \"%s\": skipping parameters beyond 2\n",name());
    }
    //--------------------------------------------------------------------------
    ~density() {}
  };
  //////////////////////////////////////////////////////////////////////////////
  bool density::manipulate(const snapshot*S) const
  {
    // 0. preliminaries
    FAC  = (K-0.5)/(K*FPit);
    // 0.1 first call ever:
    if(FST) {
      TMAN = S->initial_time();
      FST  = false;
    }
    // 0.2 is it time for a manipulation?
    if(S->time() < TMAN) return false;
    // 1. establish tree
    clock_t CPU0 = clock();
    flags FLAG   = flags::empty;
    // 1.1 if limited range of bodies, mark them
    if(S->N_bodies() != S->N_subset()) {
      FLAG = flags::marked;
      if(!S->have(fieldbit::f))
	const_cast<snapshot*>(S)->add_field(fieldbit::f);
      LoopAllBodies(S,b)
	if(in_subset(b)) b.mark(); else b.unmark();
    }
    // 1.2 build tree
    OctTree TREE(S, K+1, 0, Default::MaxDepth, FLAG);
    if(falcON::debug(1)) {
      clock_t CPU1 = clock();
      falcON::debug_info(1,"density::manipulate():"
			 "%f sec needed for tree build\n",
			 (CPU1 - CPU0)/real(CLOCKS_PER_SEC));
      CPU0 = CPU1;
    }
    // 2. find Kth nearest neighbours and estimate density
    NeighbourLister NELI(&TREE,K);
    if(!S->have(fieldbit::r))
      const_cast<snapshot*>(S)->add_field(fieldbit::r);
    NELI.Estimate(&dens,true);
    if(falcON::debug(1)) {
      clock_t CPU1 = clock();
      falcON::debug_info(1,"density::manipulate():"
		 " %f sec needed for density estimation;"
		 " %d neighbour updates\n",
		 (CPU1 - CPU0)/real(CLOCKS_PER_SEC),NELI.N_interact());
    }
    // 3. set new TMAN
    TMAN += STEP;
    return false;
  }
  //////////////////////////////////////////////////////////////////////////////
} }

__DEF__MAN(falcON::Manipulate::density)
