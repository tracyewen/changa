#ifndef GRAVITYPARTICLE_H
#define GRAVITYPARTICLE_H

#include "SFC.h"
#include <vector>

class TreeWalk;
class Compute;
class Opt;
class State;

// Object to record a type of active walk. Contains pointers to TreeWalk/Compute/Opt (T/C/O) combinations
class ActiveWalk {
  public:
  TreeWalk *tw;
  Compute *c;
  Opt *o;
  State *s;
  
  ActiveWalk(TreeWalk *_tw, Compute *_c, Opt *_o, State *state) : 
      tw(_tw), c(_c), o(_o), s(state){}
  ActiveWalk(){}
};

// Object to bookkeep a Bucket Walk.
class BucketGravityRequest {
public:
		
	unsigned int numAdditionalRequests;
	int finished;

	BucketGravityRequest(unsigned int bucketSize = 0) :
	  numAdditionalRequests(0), finished(0) {
	}
	
};

// Information needed to calculate gravity

class ExternalGravityParticle {
 public:

  double mass;
  double soft;
  Vector3D<double> position;
  double fDensity;

  void pup(PUP::er &p) {
    p | position;
    p | mass;
    p | soft;
    p | fDensity;
  }
};

class GravityParticle : public ExternalGravityParticle {
public:
	SFC::Key key;
	Vector3D<double> velocity;
	Vector3D<double> treeAcceleration;
	double potential;
	double dtGrav;
	double fBall;
	int iOrder;		/* input order of particles */
        int rung;  ///< the current rung (greater means faster)
	unsigned int iType;	// Bitmask to hold particle type information
	//unsigned int numCellInteractions;
	//unsigned int numParticleInteractions;
	//unsigned int numMACChecks;
	//unsigned int numEntryCalls;

#if COSMO_STATS > 1
	double intcellmass;
	double intpartmass;
	double extcellmass;
	double extpartmass;
#endif
	
	GravityParticle(SFC::Key k = 0) : ExternalGravityParticle() {
          key = k;
          rung = 0;
        }

	inline bool operator<(const GravityParticle& p) const {
		return key < p.key;
	}

	void pup(PUP::er &p) {
          ExternalGravityParticle::pup(p);
          p | key;
          p | velocity;
	  p | fBall;
          p | iOrder;
          p | rung;
	  p | iType;
        }
};

/* Particle Type Masks */

#define TYPE_GAS               (1<<0)
#define TYPE_DARK              (1<<1)
#define TYPE_STAR              (1<<2)
#define TYPE_PHOTOGENIC        (1<<3)

inline int TYPETest(GravityParticle *a, unsigned int b) {
    return a->iType & b;
    }
inline int TYPESet(GravityParticle *a, unsigned int b) {
    return a->iType |= b;
    }

#endif
