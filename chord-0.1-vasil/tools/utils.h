// Author: Praveen Rao
#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <vector>
#include "poly.h"
#include "nodeparams.h"

//using namespace std;
 std::string cleanString(std::string &);
 double getgtod();
const int MAXKEYLEN = 1028;
const int PAGESIZE = 8192;
const int MAXBUFS = 2000;

#define NODEID int
// Distinct tags
const char *const TAGFILE = ".tags";
const char *const PATHTAGFILE = ".ptags";
const char *const TAGDEPTH = ".tdepth";
const char *const BEGINTAG = "--BEGINTAG--";
const char *const ENDTAG = "--ENDTAG--";
const char *const PATHFILE = ".path";
const char *const HISTFILE = ".hist";
const char *const TEXTHASHFILE = ".texthash";
const char *const VALFILE = ".value";

// Polynomial related
const POLY testPoly = (1 << MAXMODDEGREE);
const POLY ONEFLAG = 0xffffffff;
const POLY lowerBitsMask = ONEFLAG;

 POLY modularMultPoly(POLY, POLY, IRRPOLY);
 POLY findMod(void *, int, IRRPOLY);
 POLY remainder(std::vector<POLY> &, POLY);
	
 int getDegree(IRRPOLY);
 int getDegree(const std::vector<POLY>&);
 void multiplyPoly(std::vector<POLY>&, POLY);
 void multiplyPoly(std::vector<POLY>&, const std::vector<POLY>&, POLY);
 void multiplyPoly(std::vector<POLY>&, const std::vector<POLY>&, const std::vector<POLY>&);

 void addPoly(std::vector<POLY>&, const std::vector<POLY>&);

 void shiftLeft(std::vector<POLY>&, int);
 void shiftRight(std::vector<POLY>&, int);

 void remainder(std::vector<POLY>&, const std::vector<POLY>&, const std::vector<POLY>&);
 void gcd(std::vector<POLY>&, const std::vector<POLY>&, const std::vector<POLY>&);
 void gcdSpecial(std::vector<POLY>&, const std::vector<POLY>&, const std::vector<POLY>&);
 void lcm(std::vector<POLY>&, const std::vector<POLY>&);

// Interval for a node
struct Interval {
	int ln, ld, rn, rd;
	int level;
  int random;
	Interval() {
		ln = 0;
		ld = 1;
		rn = 1;
		rd = 1;
	}

    // == operator
    bool operator==(const Interval& testInt) {
        if (ln == testInt.ln && ld == testInt.ld &&
            rn == testInt.rn && rd == testInt.rd &&
            level == testInt.level) {
            return true;
        }
        else
            return false;
    }

    // != operator
    bool operator!=(const Interval& testInt) {
        if (ln == testInt.ln && ld == testInt.ld &&
            rn == testInt.rn && rd == testInt.rd &&
            level == testInt.level) {
            return false;
        }
        else
            return true;
    }

};

str marshal_block(vec<str>&);
vec<str> get_payload(char *, int);

void getInterval(str&, Interval&);

void getKey(str&, str&);
void getKeyValue(str&, str&, str&);
void getKeyValue(const char *, str&, str&);
void getKeyValue(const char*, str&, std::vector<POLY>&);
void getDocid(const char *, std::vector<POLY>&, str&);

void makeKeyValue(char **, int&, str&, std::vector<POLY>&,
									InsertType);
void makeKeyValue(char **, int&, str&, Interval&,
									InsertType);
void makeKeyValue(char **, int&, vec<str>&, std::vector<int>&,
									InsertType);
void makeKeyValue(char **, int&, str&, Interval&);
void makeKeyValue(char **, int&, str&, std::vector<POLY>&, Interval&,
    InsertType);
void makeKeyValue(char **ptr, int& len, std::vector<POLY>& textHash,
    std::vector<POLY>& sig, int docId, InsertType type);
void makeDocid(char **ptr, int *ptrlen, std::vector<POLY>& sig, char *docid);

void cleanup(char *);
std::string getString(str &);
void readTags(FILE *, std::vector<std::string>&);

NODEID nextNodeID;
NODEID rootNodeID = 1;
char *NEXTNODEID = "nextnodeid";
NODEID COMPUTE_NODEID();
void str2NODEID(str&, NODEID&);

void updateMBR(std::vector<POLY>&, std::vector<POLY>&);
bool isValue(std::string& input);
void lcmSpecial(std::vector<POLY>& t, const std::vector<POLY>& s, std::vector<POLY>& g);
int pSimOpt(std::vector<POLY>&, std::vector<POLY>&, std::vector<POLY>&, bool);
int pSim(std::vector<POLY>&, std::vector<POLY>&,
    std::vector<POLY>&, bool);
str pickChild(vec<str>&, std::vector<POLY>&);
str pickChildV(vec<str>&, std::vector<POLY>&);
int enlargement(std::vector<POLY>&, std::vector<POLY>&);
#endif
