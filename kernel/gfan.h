/*
gfan.h Interface to gfan.cc

$Author: monerjan $
$Date: 2009/11/03 06:57:32 $
$Header: /usr/local/Singular/cvsroot/kernel/gfan.h,v 1.13 2009/11/03 06:57:32 monerjan Exp $
$Id$
*/
#ifdef HAVE_GFAN

#ifndef GFAN_H
#define GFAN_H

#include "intvec.h"

#define p800
#ifdef p800
#include "../../cddlib/include/setoper.h"
#include "../../cddlib/include/cdd.h"
#include "../../cddlib/include/cddmp.h"
#endif
extern int gfanHeuristic;
//ideal getGB(ideal inputIdeal);
// ideal gfan(ideal inputIdeal, int heuristic);
lists gfan(ideal inputIdeal, int heuristic);

//int dotProduct(intvec a, intvec b);
//bool isParallel(intvec a, intvec b);

class facet
{
	private:
		/** \brief Inner normal of the facet, describing it uniquely up to isomorphism */
		intvec *fNormal;
		
		/** \brief An interior point of the facet*/
		intvec *interiorPoint;
		
		/** \brief Universal Cone Number
		 * The number of the cone the facet belongs to, Set in getConeNormals()
		 */
		int UCN;
		
		/** \brief The codim of the facet
		 */
		int codim;
		
		/** \brief The Groebner basis on the other side of a shared facet
		 *
		 * In order not to have to compute the flipped GB twice we store the basis we already get
		 * when identifying search facets. Thus in the next step of the reverse search we can 
		 * just copy the old cone and update the facet and the gcBasis.
		 * facet::flibGB is set via facet::setFlipGB() and printed via facet::printFlipGB
		 */
		ideal flipGB;		//The Groebner Basis on the other side, computed via gcone::flip
		
	public:	
		/** \brief Boolean value to indicate whether a facet is flippable or not
	 	* This is also used to mark facets that nominally are flippable but which do
	 	* not intersect with the positive orthant. This check is done in gcone::getCodim2Normals
		 */	
		bool isFlippable;	//**flippable facet? */
		bool isIncoming;	//Is the facet incoming or outgoing in the reverse search?
		facet *next;		//Pointer to next facet
		facet *prev;		//Pointer to predecessor. Needed for the SearchList in noRevS
		facet *codim2Ptr;	//Pointer to (codim-2)-facet. Bit of recursion here ;-)
		int numCodim2Facets;	//#of (codim-2)-facets of this facet. Set in getCodim2Normals()
		ring flipRing;		//the ring on the other side of the facet
					
		/** The default constructor. */
		facet();
		/** Constructor for lower dimensional faces*/
		facet(int const &n);
		/**  The copy constructor */
		facet(const facet& f);
		
		/** The default destructor */
		~facet();
				
		/** \brief Comparison of facets*/
		inline bool areEqual(facet *f, facet *g);
		/** Stores the facet normal \param intvec*/
		inline void setFacetNormal(intvec *iv);
		/** Hopefully returns the facet normal */
		inline intvec *getFacetNormal();
		/** Method to print the facet normal*/
		inline void printNormal();
		/** Store the flipped GB*/
		inline void setFlipGB(ideal I);
		/** Return the flipped GB*/
		inline ideal getFlipGB();
		/** Print the flipped GB*/
		inline void printFlipGB();
		/** Set the UCN */
		inline void setUCN(int n);
		/** \brief Get the UCN 
		 * Returns the UCN iff this != NULL, else -1
		 */
		inline int getUCN();
		/** Store an interior point of the facet */
		inline void setInteriorPoint(intvec *iv);
		inline intvec *getInteriorPoint();
		/** \brief Debugging function
		 * prints the facet normal an all (codim-2)-facets that belong to it
		 */
		inline void fDebugPrint();
		
		friend class gcone;		
};

/**
 *\brief Implements the cone structure
 *
 * A cone is represented by a linked list of facet normals
 * @see facet
 */

class gcone
{
	private:		
// 		ring rootRing;		//good to know this -> generic walk
		ideal inputIdeal;	//the original
		ring baseRing;		//the basering of the cone				
		intvec *ivIntPt;	//an interior point of the cone
		int UCN;		//unique number of the cone
		int pred;		//UCN of the cone this one is derived from
 		static int counter;
		
	public:	
		/** \brief Pointer to the first facet */
		facet *facetPtr;	//Will hold the adress of the first facet; set by gcone::getConeNormals
		
		/** # of variables in the ring */
		int numVars;		//#of variables in the ring
		
		/** # of facets of the cone
		 * This value is set by gcone::getConeNormals
		 */
		int numFacets;		//#of facets of the cone
		
		/**
		 * At least as a workaround we store the irredundant facets of a matrix here.
		 * Otherwise, since we throw away non-flippable facets, facets2Matrix will not 
		 * yield all the necessary information
		 */
		dd_MatrixPtr ddFacets;	//Matrix to store irredundant facets of the cone
		
		/** Contains the Groebner basis of the cone. Is set by gcone::getGB(ideal I)*/
		ideal gcBasis;		//GB of the cone, set by gcone::getGB();
		gcone *next;		//Pointer to next cone
		gcone *prev;
		
		gcone();
		gcone(ring r, ideal I);
		gcone(const gcone& gc, const facet &f);
		~gcone();
		inline int getCounter();
		inline ring getBaseRing();
		inline void setIntPoint(intvec *iv);
		inline intvec *getIntPoint();
		inline void showIntPoint();
		inline void setNumFacets();
		inline int getNumFacets();
		inline int getUCN();
		inline int getPredUCN();		
		inline void showFacets(short codim=1);
		inline volatile void showSLA(facet &f);
		inline void idDebugPrint(const ideal &I);
		inline void invPrint(const ideal &I);
		inline bool isMonomial(const ideal &I);
		inline intvec *ivNeg(const intvec *iv);
		inline int dotProduct(const intvec &iva, const intvec &ivb);
		inline bool isParallel(const intvec &a, const intvec &b);
		inline bool areEqual(const intvec &a, const intvec &b);
		inline bool areEqual(facet *f, facet *g);
		inline int intgcd(int a, int b);
		inline void writeConeToFile(const gcone &gc, bool usingIntPoints=FALSE);
		inline void readConeFromFile(int gcNum, gcone *gc);
		inline intvec f2M(gcone *gc, facet *f, int n=1);
		
		//The real stuff
		inline void getConeNormals(const ideal &I, bool compIntPoint=FALSE);
		inline void getCodim2Normals(const gcone &gc);
		inline void flip(ideal gb, facet *f);
		inline void computeInv(ideal &gb, ideal &inv, intvec &f);
// 		poly restOfDiv(poly const &f, ideal const &I); removed with r12286
		inline ideal ffG(const ideal &H, const ideal &G);
		inline void getGB(ideal const &inputIdeal);		
		inline void interiorPoint(const dd_MatrixPtr &M, intvec &iv);
		ring rCopyAndAddWeight(const ring &r, const intvec *ivw);
		ring rCopyAndChangeWeight(const ring &r, intvec *ivw);		
// 		void reverseSearch(gcone *gcAct); //NOTE both removed from r12286
// 		bool isSearchFacet(gcone &gcTmp, facet *testfacet);
		void noRevS(gcone &gcRoot, bool usingIntPoint=FALSE);
		inline void makeInt(const dd_MatrixPtr &M, const int line, intvec &n);
		inline void normalize();
		facet * enqueueNewFacets(facet *f);
		dd_MatrixPtr facets2Matrix(const gcone &gc);		
// 		static void gcone::idPrint(ideal &I);		
		friend class facet;	
};
lists lprepareResult(gcone *gc, int n);
#endif
#endif
