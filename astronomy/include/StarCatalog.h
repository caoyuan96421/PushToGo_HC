/*
 * StarCatalog.h
 *
 *  Created on: 2018Äê5ÔÂ2ÈÕ
 *      Author: caoyuan9642
 */

#ifndef _STARCATALOG_H_
#define _STARCATALOG_H_

#include <cstdlib>
#include <cmath>

struct SkyObjInfo
{
	int id;
	char catname[12];
	float RA;
	float DEC;
	char name[24];
	float distance;
	float magnitude;
	float absmagnitude;
	float color;
	enum {
		SKYOBJ_STAR=0,
		SKYOBJ_MESSIER,
		SKYOBJ_NGC,
		SKYOBJ_IC,
		SKYOBJ_SOLARSYSTEM
	} type;
};

static const float RADIAN_F = 180.0f / M_PI;
static const float DEGREE_F = M_PI / 180.0f;
static const double RADIAN = 180.0 / M_PI;
static const double DEGREE = M_PI / 180.0;

inline float min(float x, float y)
{
	return (x < y) ? x : y;
}

inline float max(float x, float y)
{
	return (x > y) ? x : y;
}

// Returns geodesic distance
inline float dist_angle(float ra1, float dec1, float ra2, float dec2){
	float s1 = sinf(dec1 * DEGREE_F);
	float s2 = sinf(dec2 * DEGREE_F);
	float c1 = sqrtf(1.0f - s1*s1);
	float c2 = sqrtf(1.0f - s2*s2);
	float cc = cosf((ra1 - ra2) * DEGREE_F);
	return acosf(s1*s2 + c1*c2*cc) * RADIAN_F;
}

// Returns geodesic distance
inline float dist_angle_hr(float ra1, float dec1, float ra2, float dec2){
	float s1 = sinf(dec1 * DEGREE_F);
	float s2 = sinf(dec2 * DEGREE_F);
	float s3 = sinf(ra1 * DEGREE_F);
	float s4 = sinf(ra2 * DEGREE_F);
	float c1 = sqrtf(1.0f - s1*s1);
	float c2 = sqrtf(1.0f - s2*s2);
	float c3 = cosf(ra1 * DEGREE_F);
	float c4 = cosf(ra2 * DEGREE_F);
	float x1 = c1 * c3;
	float y1 = c1 * s3;
	float z1 = s1;
	float x2 = c2 * c4;
	float y2 = c2 * s4;
	float z2 = s2;
	float xc = y1*z2 - y2*z1;
	float yc = z1*x2 - z2*x1;
	float zc = x1*y2 - x2*y1;
	float cross = sqrtf(xc*xc+yc*yc+zc*zc);
	float dot = x1*x2+y1*y2+z1*z2;
	return atan2f(cross, dot) * RADIAN_F;
}

struct HashMap
{
	HashMap();
	~HashMap();
	SkyObjInfo* &operator [](int id);
	struct HashMapNode
	{
		int id;
		SkyObjInfo *star;
		HashMapNode *next;
		HashMapNode()
		{
			id = 0;
			star = NULL;
			next = NULL;
		}
	};
private:
	static const int HASH_SIZE = 1024;
	HashMapNode *list[HASH_SIZE];

	HashMapNode *malloc();
};

struct QTNode
{
	SkyObjInfo *star;
	QTNode *daughters[4];
	float ral, rar;
	float decl, decr;
	bool isleaf;

	QTNode(float xl = 0, float xr = 0, float yl = 0, float yr = 0) :
			star(NULL), ral(xl), rar(xr), decl(yl), decr(yr), isleaf(true)
	{
		daughters[0] = NULL;
		daughters[1] = NULL;
		daughters[2] = NULL;
		daughters[3] = NULL;
	}

	~QTNode()
	{
		for (int i = 0; i < 4; i++)
		{
			if (daughters[i])
				daughters[i]->~QTNode();
		}
	}

	QTNode *malloc();

	QTNode *quadrant(float ra, float dec)
	{
		if (ra < ral || ra > rar || dec < decl || dec > decr)
		{
			// Not found
			return NULL;
		}
		float ram = (ral + rar) * 0.5;
		float decm = (decl + decr) * 0.5;

		int q;
		if (dec >= decm)
		{
			if (ra >= ram)
				q = 0;
			else
				q = 1;
		}
		else
		{
			if (ra >= ram)
				q = 2;
			else
				q = 3;
		}
		if (daughters[q])
		{
			// Found
			return daughters[q];
		}
		else
		{
			// Create
			isleaf = false;
			float rl = (q & 1) ? ral : ram;
			float rr = (q & 1) ? ram : rar;
			float dl = (q & 2) ? decl : decm;
			float dr = (q & 2) ? decm : decr;
			daughters[q] = this->malloc();
			*daughters[q] = QTNode(rl, rr, dl, dr);
			return daughters[q];
		}
	}

	bool intersects(float rl, float rr, float dl, float dr)
	{
		if (max(rl, rr) <= ral || min(rl, rr) >= rar)
			return false;
		if (max(dl, dr) <= decl || min(dl, dr) >= decr)
			return false;
		return true;
	}
};

struct QuadTree
{
	QTNode head;

	QuadTree() :
			head(-180.0f, 180.0f, -90.0f, 90.0f)
	{
	}

	bool insert(SkyObjInfo *newstar);
	// truc=true: Query the quadtree with max magnitude, above which the entire subtree will not be searched anymore
	// trunc=false:  Query the quadtree with optional max magnitude, will search entire subtree within the window
	void query(void (*cb)(SkyObjInfo *, void *), float ral, float rar, float decl, float decr, void *arg, float maxmag, bool trunc=true);
	SkyObjInfo *search(float ra, float dec, float maxdist);
	SkyObjInfo *findClosest(float ra, float dec, float maxdist, float maxmag=100, bool trunc=true);
private:
	void _query_trunc(void (*cb)(SkyObjInfo *, void *), QTNode *p, float ral, float rar, float decl, float decr, void *arg, float maxmag);
	void _query_notrunc(void (*cb)(SkyObjInfo *, void *), QTNode *p, float ral, float rar, float decl, float decr, void *arg, float maxmag);
	SkyObjInfo *_search(QTNode *p, float ra, float dec, float maxdist);
	SkyObjInfo *_find(QTNode *p, float ra, float dec, float &maxdist, float maxmag=100, bool trunc=true, int pole = 0);
};

class StarCatalog
{
public:
	static StarCatalog &getInstance()
	{

		return instance;
	}

	void query_stars_common(void (*cb)(SkyObjInfo *, void *), float ramin, float ramax, float decmin, float decmax, void *arg, float maxmag);
	void query_stars(void (*cb)(SkyObjInfo *, void *), float ramin, float ramax, float decmin, float decmax, void *arg, float maxmag);
	void query_dso(void (*cb)(SkyObjInfo *, void *), float ramin, float ramax, float decmin, float decmax, void *arg, float maxmag);

	SkyObjInfo *searchByCoordinates(float ra, float dec, float maxdist = 0.01f, bool staronly = false, float maxmag = 100);
	SkyObjInfo *searchByID(int id);
	void constructTree();

private:
	static StarCatalog instance;
	StarCatalog();
	virtual ~StarCatalog()
	{
	}


};

#endif /* _STARCATALOG_H_ */
