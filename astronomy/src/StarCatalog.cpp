/*
 * StarCatalog.cpp
 *
 *  Created on: 2018Äê5ÔÂ2ÈÕ
 *      Author: caoyuan9642
 */

#include <StarCatalog.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <cctype>
#include <cfloat>
#include "HYGMag8.h"
#include "Messier.h"

QuadTree qt_stars_common;
QuadTree qt_stars;
QuadTree qt_dso;

const size_t NODEFAC_SIZE = STAR_CATALOG_SIZE + STAR_NAMED_STAR_COUNT
		+ MESSIER_CATALOG_SIZE * 2;

__attribute__((section (".sdram"))) QTNode _nodefactory[NODEFAC_SIZE];

__attribute__((section (".sdram"))) HashMap::HashMapNode _hashmapfactory[STAR_CATALOG_SIZE];

static int _nodecount = 0;
static int _hashcount = 0;

HashMap starIndex;

StarCatalog StarCatalog::instance;

static inline float sqr(float x) {
	return x * x;
}

QTNode* QTNode::malloc() {
	if (_nodecount == NODEFAC_SIZE)
		return NULL;
	return &_nodefactory[_nodecount++];
}

StarCatalog::StarCatalog() {
}

SkyObjInfo* StarCatalog::searchByCoordinates(float ra, float dec, float maxdist,
		bool staronly, float maxmag) {
	SkyObjInfo *closest_star = qt_stars.findClosest(ra, dec, maxdist, maxmag);
	SkyObjInfo *closest_dso = qt_dso.findClosest(ra, dec, maxdist, maxmag,
			false);
	if (staronly || !closest_dso)
		return closest_star;
	if (!closest_star)
		return closest_dso;
	// Both found, get closer one

	float d1 = dist_angle(ra, dec, closest_star->RA, closest_star->DEC);
	float d2 = dist_angle(ra, dec, closest_dso->RA, closest_dso->DEC);
	return (d1 < d2) ? closest_star : closest_dso;
}

SkyObjInfo* StarCatalog::searchByID(int id) {
	return starIndex[id];
}

static void addToTree(SkyObjInfo catalog[], QuadTree *tree, int size,
		bool hash = false) { // hash: put star into hash table so it can be indexed by id
	for (int i = 0; i < size; i++) {
		SkyObjInfo &star = catalog[i];
		if (star.id < 0) {
			break;
		}
		if (tree == &qt_stars && (*(star.name) != '\0')) {
			qt_stars_common.insert(&star);
		}

		tree->insert(&star);
		if (hash)
			starIndex[star.id] = &star;
	}
}

void StarCatalog::constructTree() {
	addToTree(star_catalog_hyg, &qt_stars, STAR_CATALOG_SIZE, true);
	addToTree(messier_catalog, &qt_dso, MESSIER_CATALOG_SIZE);
}

void StarCatalog::query_stars_common(void (*cb)(SkyObjInfo*, void*),
		float ramin, float ramax, float decmin, float decmax, void *arg,
		float maxmag) {
	qt_stars_common.query(cb, ramin, ramax, decmin, decmax, arg, maxmag);
}

void StarCatalog::query_stars(void (*cb)(SkyObjInfo*, void*), float ramin,
		float ramax, float decmin, float decmax, void *arg, float maxmag) {
	qt_stars.query(cb, ramin, ramax, decmin, decmax, arg, maxmag);
}

void StarCatalog::query_dso(void (*cb)(SkyObjInfo*, void*), float ramin,
		float ramax, float decmin, float decmax, void *arg, float maxmag) {
	qt_dso.query(cb, ramin, ramax, decmin, decmax, arg, maxmag, false);
}

bool QuadTree::insert(SkyObjInfo *newstar) {
	if (!newstar) {
		return false;
	}
	QTNode *p = &head;
	while (p->star) {
		// Find quadrant and dive in
		p = p->quadrant(newstar->RA, newstar->DEC);
		if (p == NULL)
			return false; // Failed
	}

	p->star = newstar;
	return true;
}

void QuadTree::query(void (*cb)(SkyObjInfo*, void*), float ral, float rar,
		float decl, float decr, void *arg, float maxmag, bool trunc) {
	// Check
	ral = remainderf(ral, 360.0f);
	rar = remainderf(rar, 360.0f);
	if (decl > decr) {
		float temp = decr;
		decr = decl;
		decl = temp;
	}
	if (decl < -90.0)
		decl = -90.0;
	if (decr > 90.0)
		decr = 90.0;

	if (ral > rar) {
		// Crossing 180/-180 RA, divide into two
		if (trunc) {
			_query_trunc(cb, &head, ral, 180.0f, decl, decr, arg, maxmag);
			_query_trunc(cb, &head, -180.0f, rar, decl, decr, arg, maxmag);
		} else {
			_query_notrunc(cb, &head, ral, 180.0f, decl, decr, arg, maxmag);
			_query_notrunc(cb, &head, -180.0f, rar, decl, decr, arg, maxmag);
		}
		return;
	} else {
		if (trunc)
			_query_trunc(cb, &head, ral, rar, decl, decr, arg, maxmag);
		else
			_query_notrunc(cb, &head, ral, rar, decl, decr, arg, maxmag);
	}

}

SkyObjInfo* QuadTree::search(float ra, float dec, float maxdist) {
	return _search(&head, ra, dec, maxdist);
}

SkyObjInfo* QuadTree::_search(QTNode *p, float ra, float dec, float maxdist) {
	if (!p->star)
		return NULL;
	if (fabsf(remainderf(p->star->DEC - dec, 360)) < maxdist
			&& fabsf(remainderf(p->star->RA - ra, 360)) < maxdist) {
		return p->star;
	}
	if (!p->isleaf) {
		// Search daughters in turn
		for (int i = 0; i < 4; i++) {
			if (p->daughters[i]
					&& p->daughters[i]->intersects(ra - maxdist, ra + maxdist,
							dec - maxdist, dec + maxdist)) {
				SkyObjInfo *ret = _search(p->daughters[i], ra, dec, maxdist);
				if (ret)
					return ret;
			}
		}
	}
	return NULL; // Nothing found
}

SkyObjInfo* QuadTree::findClosest(float ra, float dec, float maxdist,
		float maxmag, bool trunc) {
	SkyObjInfo *soi = _find(&head, ra, dec, maxdist, maxmag, trunc);
	if (dec + maxdist > 88) { // Search north pole
		SkyObjInfo *soi_pole = _find(&head, ra, dec, maxdist, maxmag, trunc, 1);
		if (soi_pole)
			soi = soi_pole;
	} else if (dec - maxdist < -88) { // Search south
		SkyObjInfo *soi_pole = _find(&head, ra, dec, maxdist, maxmag, trunc,
				-1);
		if (soi_pole)
			soi = soi_pole;
	}
	return soi;
}

SkyObjInfo* QuadTree::_find(QTNode *p, float ra, float dec, float &maxdist,
		float maxmag, bool trunc, int pole) {
	if (!p->star)
		return NULL;
	SkyObjInfo *closest = NULL;
	if (p->star->magnitude <= maxmag) {
		float d = dist_angle(ra, dec, p->star->RA, p->star->DEC);
		if (d < maxdist) {
			closest = p->star;
			maxdist = d;
		}
	} else if (trunc)
		return NULL; // Stop searching if truncating

	if (!p->isleaf) {
		// Search daughters in turn
		for (int i = 0; i < 4; i++) {
			if (!p->daughters[i])
				continue;
			switch (pole) {
			case 0:
				if (!p->daughters[i]->intersects(ra - maxdist, ra + maxdist,
						dec - maxdist, dec + maxdist)
						&& !(ra - maxdist < -180
								&& p->daughters[i]->intersects(
										ra - maxdist + 360, 180, dec - maxdist,
										dec + maxdist))
						&& !(ra + maxdist > 180
								&& p->daughters[i]->intersects(-180,
										ra + maxdist - 360, dec - maxdist,
										dec + maxdist)))
					continue;
				break;
			case 1: // North pole
				if (!p->daughters[i]->intersects(-180, 180, 90 - maxdist, 90))
					continue;
				break;
			case -1: // South pole
				if (!p->daughters[i]->intersects(-180, 180, -90, -90 + maxdist))
					continue;
				break;
			}
			float m = maxdist;
			SkyObjInfo *ret = _find(p->daughters[i], ra, dec, m, maxmag, trunc, pole);
			if (ret && m < maxdist) {
				maxdist = m;
				closest = ret;
			}
		}
	}
	return closest;
}

void QuadTree::_query_trunc(void (*cb)(SkyObjInfo*, void*), QTNode *p,
		float ral, float rar, float decl, float decr, void *arg, float maxmag) {

// Check star at current node
	if (p->star && p->star->magnitude < maxmag) {
		if (p->star->RA <= rar && p->star->RA >= ral && p->star->DEC <= decr
				&& p->star->DEC >= decl) {
			cb(p->star, arg);
		}
	} else {
		// If current node is below magnitude threshhold, then no more searching
		// Because its children must be even lower magnitude
		return;
	}

// Now check daughters
	if (!p->isleaf) {
		for (int i = 0; i < 4; i++) {
			if (p->daughters[i]
					&& p->daughters[i]->intersects(ral, rar, decl, decr)) {
				_query_trunc(cb, p->daughters[i], ral, rar, decl, decr, arg,
						maxmag);
			}
		}
	}
}

void QuadTree::_query_notrunc(void (*cb)(SkyObjInfo*, void*), QTNode *p,
		float ral, float rar, float decl, float decr, void *arg, float maxmag) {

	// Check star at current node
	if (p->star) {
		if ((p->star->magnitude < maxmag) && p->star->RA <= rar
				&& p->star->RA >= ral && p->star->DEC <= decr
				&& p->star->DEC >= decl) {
			cb(p->star, arg);
		}
	} else {
		return;
	}
	// Will search subtree even if magnitude is low

	// Now check daughters
	if (!p->isleaf) {
		for (int i = 0; i < 4; i++) {
			if (p->daughters[i]
					&& p->daughters[i]->intersects(ral, rar, decl, decr)) {
				_query_notrunc(cb, p->daughters[i], ral, rar, decl, decr, arg,
						maxmag);
			}
		}
	}
}

HashMap::HashMap() {
	for (int i = 0; i < HASH_SIZE; i++) {
		list[i] = NULL;
	}
}

HashMap::~HashMap() {
	for (int i = 0; i < HASH_SIZE; i++) {
		HashMapNode *p = list[i];
		while (p) {
			HashMapNode *q = p;
			delete p;
			p = q;
		}
	}
}

SkyObjInfo*& HashMap::operator [](int id) {
	int k = id % HASH_SIZE;
	HashMapNode *p = list[k], *q = NULL;
	while (p && p->id != id)
		p = (q = p)->next;
	if (!p) {
		p = malloc();
		p->id = id;
//		p->next = list[k];
//		list[k] = p; // Insert at the beginning of the list
		p->next = NULL;
		if (q)
			q->next = p;
		else
			list[k] = p; // Insert at the end of the list
	}
	return p->star;
}

HashMap::HashMapNode* HashMap::malloc() {
	if (_hashcount == STAR_CATALOG_SIZE) {
		return NULL;
	}
	return &_hashmapfactory[_hashcount++];
}

