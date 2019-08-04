/*
 * SkyCulture.cpp
 *
 *  Created on: 2018��5��9��
 *      Author: caoyuan9642
 */

#include "SkyCulture.h"
#include <cstring>
#include <cstdlib>
#include <cmath>

// Westurn SkyCulture
#define NUM_CONSTELL 88
#ifdef SIMULATOR
char *strtok_r(char *s, const char *d, char **save_ptr){
	return strtok(s, d);
}
#endif

//static const struct
//{
//	const SkyCulture::Constellation constell;
//	const char* short_name;
//	const char* name;
//} constellation_names[NUM_CONSTELL] =
//{
//{ SkyCulture::Aql, "Aql", "Aquila" },
//{ SkyCulture::And, "And", "Andromeda" },
//{ SkyCulture::Scl, "Scl", "Sculptor" },
//{ SkyCulture::Ara, "Ara", "Ara" },
//{ SkyCulture::Lib, "Lib", "Libra" },
//{ SkyCulture::Cet, "Cet", "Cetus" },
//{ SkyCulture::Ari, "Ari", "Aries" },
//{ SkyCulture::Pyx, "Pyx", "Pyxis" },
//{ SkyCulture::Boo, "Boo", "Bootes" },
//{ SkyCulture::Cae, "Cae", "Caelum" },
//{ SkyCulture::Cha, "Cha", "Chamaeleon" },
//{ SkyCulture::Cnc, "Cnc", "Cancer" },
//{ SkyCulture::Cap, "Cap", "Capricornus" },
//{ SkyCulture::Car, "Car", "Carina" },
//{ SkyCulture::Cas, "Cas", "Cassiopeia" },
//{ SkyCulture::Cen, "Cen", "Centaurus" },
//{ SkyCulture::Cep, "Cep", "Cepheus" },
//{ SkyCulture::Com, "Com", "Coma Berenices" },
//{ SkyCulture::Cvn, "Cvn", "Canes Venatici" },
//{ SkyCulture::Aur, "Aur", "Auriga" },
//{ SkyCulture::Col, "Col", "Columba" },
//{ SkyCulture::Cir, "Cir", "Circinus" },
//{ SkyCulture::Crt, "Crt", "Crater" },
//{ SkyCulture::CrA, "CrA", "Corona Australis" },
//{ SkyCulture::CrB, "CrB", "Corona Borealis" },
//{ SkyCulture::Crv, "Crv", "Corvus" },
//{ SkyCulture::Cru, "Cru", "Crux" },
//{ SkyCulture::Cyg, "Cyg", "Cygnus" },
//{ SkyCulture::Del, "Del", "Delphinus" },
//{ SkyCulture::Dor, "Dor", "Dorado" },
//{ SkyCulture::Dra, "Dra", "Draco" },
//{ SkyCulture::Nor, "Nor", "Norma" },
//{ SkyCulture::Eri, "Eri", "Eridanus" },
//{ SkyCulture::Sge, "Sge", "Sagitta" },
//{ SkyCulture::For, "For", "Fornax" },
//{ SkyCulture::Gem, "Gem", "Gemini" },
//{ SkyCulture::Cam, "Cam", "Camelopardalis" },
//{ SkyCulture::CMa, "CMa", "Canis Major" },
//{ SkyCulture::UMa, "UMa", "Ursa Major" },
//{ SkyCulture::Gru, "Gru", "Grus" },
//{ SkyCulture::Her, "Her", "Hercules" },
//{ SkyCulture::Hor, "Hor", "Horologium" },
//{ SkyCulture::Hya, "Hya", "Hydra" },
//{ SkyCulture::Hyi, "Hyi", "Hydrus" },
//{ SkyCulture::Ind, "Ind", "Indus" },
//{ SkyCulture::Lac, "Lac", "Lacerta" },
//{ SkyCulture::Mon, "Mon", "Monoceros" },
//{ SkyCulture::Lep, "Lep", "Lepus" },
//{ SkyCulture::Leo, "Leo", "Leo" },
//{ SkyCulture::Lup, "Lup", "Lupus" },
//{ SkyCulture::Lyn, "Lyn", "Lynx" },
//{ SkyCulture::Lyr, "Lyr", "Lyra" },
//{ SkyCulture::Ant, "Ant", "Antlia" },
//{ SkyCulture::Mic, "Mic", "Microscopium" },
//{ SkyCulture::Mus, "Mus", "Musca" },
//{ SkyCulture::Oct, "Oct", "Octans" },
//{ SkyCulture::Aps, "Aps", "Apus" },
//{ SkyCulture::Oph, "Oph", "Ophiuchus" },
//{ SkyCulture::Ori, "Ori", "Orion" },
//{ SkyCulture::Pav, "Pav", "Pavo" },
//{ SkyCulture::Peg, "Peg", "Pegasus" },
//{ SkyCulture::Pic, "Pic", "Pictor" },
//{ SkyCulture::Per, "Per", "Perseus" },
//{ SkyCulture::Equ, "Equ", "Equuleus" },
//{ SkyCulture::CMi, "CMi", "Canis Minor" },
//{ SkyCulture::LMi, "LMi", "Leo Minor" },
//{ SkyCulture::Vul, "Vul", "Vulpecula" },
//{ SkyCulture::UMi, "UMi", "Ursa Minor" },
//{ SkyCulture::Phe, "Phe", "Phoenix" },
//{ SkyCulture::Psc, "Psc", "Pisces" },
//{ SkyCulture::PsA, "PsA", "Piscis Austrinus" },
//{ SkyCulture::Vol, "Vol", "Volans" },
//{ SkyCulture::Pup, "Pup", "Puppis" },
//{ SkyCulture::Ret, "Ret", "Reticulum" },
//{ SkyCulture::Sgr, "Sgr", "Sagittarius" },
//{ SkyCulture::Sco, "Sco", "Scorpius" },
//{ SkyCulture::Sct, "Sct", "Scutum" },
//{ SkyCulture::Ser, "Ser", "Serpens" },
//{ SkyCulture::Sex, "Sex", "Sextans" },
//{ SkyCulture::Men, "Men", "Mensa" },
//{ SkyCulture::Tau, "Tau", "Taurus" },
//{ SkyCulture::Tel, "Tel", "Telescopium" },
//{ SkyCulture::Tuc, "Tuc", "Tucana" },
//{ SkyCulture::Tri, "Tri", "Triangulum" },
//{ SkyCulture::Tra, "Tra", "Triangulum Australe" },
//{ SkyCulture::Aqr, "Aqr", "Aquarius" },
//{ SkyCulture::Vir, "Vir", "Virgo" },
//{ SkyCulture::Vel, "Vel", "Vela" }, };

// @formatter:off
static const char *constellation_lines_raw[NUM_CONSTELL] = {
	"Aql 8 98036 97649 97649 97278 97649 95501 95501 97804 99473 97804 95501 93747 93747 93244 95501 93805",
	"And 5 677 3092 3092 5447 9640 5447 5447 4436 4436 3881",
	"Scl 3 116231 4577 4577 115102 115102 116231",
	"Ara 7 88714 85792 85792 83081 83081 82363 82363 85727 85727 85267 85267 85258 85258 88714",
	"Lib 5 77853 76333 76333 74785 74785 72622 72622 73714 73714 76333",
	"Cet 20 10324 11484 8102 3419 3419 1562 3419 5364 5364 6537 6537 8645 8645 11345 11345 12390 12390 12770 12770 11783 11783 8102 10826 12390 10826 12387 12387 12706 12706 14135 14135 13954 13954 12828 12828 11484 11484 12093 12093 12706",
	"Ari 3 13209 9884 9884 8903 8903 8832",
	"Sct 5 92175 92202 92202 92814 92814 90595 90595 91117 91117 92175",
	"Pyx 2 42515 42828 42828 43409",
	"Boo 9 71795 69673 69673 72105 72105 74666 74666 73555 73555 71075 71075 71053 71053 69673 69673 67927 67927 67459",
	"Cae 2 21060 21770 21770 21861",
	"Cha 2 40702 51839 51839 60000",
	"Cnc 5 43103 42806 42806 40843 42806 42911 42911 40526 42911 44066",
	"Cap 9 100064 100345 100345 104139 104139 105515 105515 106985 106985 107556 105515 105881 105881 104139 100345 102485 104139 102978",
	"Car 14 45238 50099 50099 52419 52419 52468 52468 54463 54463 53253 53253 51232 51232 50371 50371 45556 42568 41037 41037 30438 45080 45556 45080 42568 30438 31685 41037 39429",
	"Cas 4 8886 6686 6686 4427 4427 3179 3179 746",
	"Cen 16 71683 68702 68702 66657 66657 68002 68002 68282 68282 67472 67472 67464 67464 65936 65936 65109 67464 68933 67472 71352 71352 73334 68002 61932 61932 60823 60823 59196 59196 56480 56480 56561",
	"Cep 6 109492 112724 112724 106032 106032 105199 105199 109492 112724 116727 116727 106032",
	"Com 2 64241 64394 64394 60742",
	"Cvn 1 61317 63125",
	"Aur 6 28380 28360 28360 24608 24608 23453 23453 23015 25428 23015 25428 28380",
	"Col 6 30277 29807 29807 28199 28199 27628 27628 28328 27628 26634 26634 25859",
	"Cir 2 71908 75323 71908 74824",
	"Crt 9 53740 54682 54682 55705 55705 55282 55282 53740 55282 55687 55687 56633 56633 58188 58188 57283 57283 55705",
	"CrA 9 91875 92989 92989 93174 93174 93825 93825 94114 94114 94160 94160 94005 94005 93542 93542 92953 91875 90887",
	"CrB 6 76127 75695 75695 76267 76267 76952 76952 77512 77512 78159 78159 78493",
	"Crv 6 61174 60965 60965 59803 59803 59316 59316 59199 59316 61359 61359 60965",
	"Cru 2 61084 60718 62434 59747",
	"Cyg 9 94779 95853 95853 97165 97165 100453 100453 102098 100453 102488 102488 104732 104732 107310 100453 98110 98110 95947",
	"Del 5 101421 101769 101769 101958 101958 102532 102532 102281 102281 101769",
	"Dor 5 27100 27890 27890 26069 26069 27100 26069 21281 21281 19893",
	"Dra 15 87585 87833 87833 85670 85670 85829 85829 87585 87585 94376 94376 97433 97433 94648 94648 89937 89937 83895 83895 80331 80331 78527 78527 75458 75458 68756 68756 61281 61281 56211",
	"Nor 5 79509 80000 80000 80582 80582 78639 78639 80000 78639 79509",
	"Eri 27 7588 9007 9007 10602 10602 11407 11407 12413 12413 12486 12486 13847 13847 15510 15510 17797 17797 17874 17874 20042 20042 20535 20535 21393 21393 17651 17651 16611 16611 15474 15474 14146 14146 12843 12843 13701 13701 15197 15197 16537 16537 17378 17378 21444 21444 22109 22109 22701 22701 23875 23875 23972 23972 21594",
	"Sge 4 96837 97365 97365 96757 97365 98337 98337 98920",
	"For 1 13147 14879",
	"Gem 16 31681 34088 34088 35550 35550 35350 35350 32362 35550 36962 36962 37740 36962 37826 36962 36046 36046 34693 34693 36850 34693 33018 34693 32246 32246 30883 32246 30343 30343 29655 29655 28734",
	"Cam 5 16228 18505 18505 22783 16228 17959 17959 22783 17959 25110",
	"CMa 17 33160 34045 34045 33347 33347 32349 32349 33977 33977 34444 34444 35037 35037 35904 33579 33856 33856 34444 33856 33165 33165 31592 31592 31416 31592 30324 31592 32349 33579 32759 30122 33579 33347 33160",
	"UMa 19 67301 65378 65378 62956 62956 59774 59774 54061 54061 53910 53910 58001 58001 59774 58001 57399 57399 54539 54539 50372 54539 50801 53910 48402 48402 46853 46853 44471 46853 44127 48402 48319 48319 41704 41704 46733 46733 54061",
	"Gru 9 114131 110997 110997 109268 109268 112122 112122 114421 114421 114131 112122 113638 112122 112623 109268 109111 109111 108085",
	"Her 18 86414 87808 87808 85112 85112 84606 84606 84380 84380 81833 81833 81126 81126 79992 79992 77760 81833 81693 81693 80816 80816 80170 81693 83207 83207 85693 85693 84379 86974 87933 87933 88794 83207 84380 86974 85693",
	"Hor 2 19747 12484 12484 14240",
	"Hya 18 42799 42402 42402 42313 42313 43109 43109 43234 43234 42799 43234 43813 43813 45336 45336 46776 46776 46509 46509 46390 46390 48356 48356 49841 49841 51069 51069 52943 52943 56343 56343 57936 57936 64166 64166 64962",
	"Hyi 4 2021 17678 17678 12394 12394 11001 11001 9236",
	"Ind 3 105319 101772 101772 103227 103227 105319",
	"Lac 6 109937 111104 111104 111022 111022 110609 110609 110538 110538 111169 111169 111022",
	"Mon 6 29651 30867 30867 34769 34769 30419 30419 29151 34769 39863 39863 37447",
	"Lep 14 28910 28103 28103 27288 27288 25985 25985 24305 25985 27654 27654 27072 27072 25606 25606 23685 25985 25606 24305 24845 24305 24327 23685 24305 24327 24244 24845 24873",
	"Leo 10 57632 54879 54879 49669 49669 49583 49583 50583 50583 54872 54872 57632 50583 50335 50335 48455 48455 47908 54872 54879",
	"Lup 13 77634 78970 78970 78384 78384 77634 78384 76297 76297 75141 75141 75177 75141 73273 76297 76552 76552 74395 74395 71860 74395 71536 71860 70576 71860 73273",
	"Lyn 7 45860 45688 45688 44700 44700 44248 44248 41075 41075 36145 36145 33449 33449 30060",
	"Lyr 5 91262 91971 91971 92420 92420 93194 93194 92791 92791 91971",
	"Ant 1 51172 48926",
	"Mic 2 105140 103738 103738 102831",
	"Mus 4 62322 57363 57363 61199 61199 61585 61585 62322",
	"Oct 3 107089 112405 112405 70638 70638 107089",
	"Aps 2 72370 81065 81065 81852",
	"Oph 7 86032 86742 84012 86742 86032 83000 83000 79882 79882 81377 81377 84012 84012 85755",
	"Ori 21 26727 26311 26311 25930 28691 29426 29426 29038 29038 27913 29426 28614 28614 27989 27989 26727 26727 27366 27366 24436 24436 25930 25930 25336 25336 26207 26207 27989 25336 22449 22449 22549 22549 22730 22730 23123 22449 22509 22509 22845 29038 28614",
	"Pav 13 100751 105858 105858 102395 102395 99240 99240 100751 99240 98495 98495 91792 91792 93015 93015 99240 93015 92609 92609 90098 90098 88866 88866 92609 88866 86929",
	"Peg 14 1067 113963 113881 112158 112158 109352 113881 112748 112748 112440 112440 109176 109176 107354 113963 112447 112447 112029 112029 109427 109427 107315 677 113881 677 1067 113881 113963",
	"Pic 2 32607 27530 27530 27321",
	"Per 10 17448 18246 18246 18614 18614 18532 18532 17358 17358 15863 15863 14328 14328 13268 15863 14576 14576 14354 14354 13254",
	"Equ 4 104521 104858 104858 105570 105570 104987 104987 104521",
	"CMi 1 37279 36188",
	"LMi 4 53229 51233 51233 49593 49593 46952 49593 53229",
	"Vul 1 95771 98543",
	"UMi 7 11767 85822 85822 82080 82080 77055 77055 79822 79822 75097 75097 72607 72607 77055",
	"Phe 11 5348 5165 5165 2072 2072 5348 5165 7083 7083 8837 8837 5165 5165 6867 6867 2072 2072 2081 2081 765 765 2072",
	"Psc 19 4889 5742 4889 6193 6193 5742 5742 7097 7097 8198 8198 9487 9487 8833 8833 7884 7884 7007 7007 4906 4906 3760 3760 1645 1645 118268 118268 116771 116771 116928 116928 115738 115738 114971 114971 115830 115830 116771",
	"PsA 6 113368 111954 111954 108661 108661 107608 107608 109422 109422 111188 111188 113246",
	"Vol 7 37504 34481 34481 39794 39794 37504 39794 35228 39794 41312 41312 44382 44382 39794",
	"Pup 7 39757 38146 38146 35264 35264 31685 31685 32768 32768 36377 36377 39429 39429 39757",
	"Ret 4 19780 19921 19921 18597 18597 17440 17440 19780",
	"Sgr 24 89931 90496 89642 90185 90185 88635 88635 87072 88635 89931 89931 90185 90185 93506 93506 92041 92041 89931 92041 90496 90496 89341 93506 93864 93864 92855 92855 92041 92855 93085 93085 93683 93683 94820 94820 95168 93864 96406 96406 98688 98688 98412 98412 98032 98032 95347 98032 95294",
	"Sco 12 85927 86670 86670 87073 87073 86228 86228 84143 84143 82671 82671 82514 82514 82396 82396 81266 81266 80763 80763 78401 80763 78265 80763 78820",
	"Ser 11 77516 77622 77622 77070 77070 76276 76276 77233 77233 78072 78072 77450 77450 77233 92946 89962 89962 86565 86565 86263 86263 84880",
	"Sex 1 51437 49641",
	"Men 1 25918 21949",
	"Tau 12 25428 21881 21881 20889 21421 26451 20205 20455 20205 18724 18724 15900 21421 20889 21421 20894 20894 20205 20889 20648 20648 20455 20455 17847",
	"Tel 1 90568 90422",
	"Tuc 3 110130 114996 114996 1599 114996 2484",
	"Tri 3 10670 10064 10064 8796 8796 10670",
	"Tra 3 82273 74946 74946 77952 77952 82273",
	"Aqr 14 106278 109074 109074 110395 110395 110960 110960 111497 111497 112961 112961 114855 114855 115438 109074 110003 110003 109139 110003 111123 111123 112716 112716 113136 113136 114341 102618 106278",
	"Vir 12 57380 60030 60030 61941 61941 65474 65474 69427 69427 69701 69701 71957 65474 66249 66249 68520 68520 72220 66249 63090 63090 63608 63090 61941",
	"Vel 10 39953 42536 42536 42913 42913 45941 45941 48774 48774 52727 52727 51986 51986 50191 50191 46651 46651 44816 44816 39953"
};
// @formatter:on
static struct ConstellationLineIndex {
	int num_segment;
	SkyCulture::Constellation constell;
	SkyCulture::Segment *segments;

	ConstellationLineIndex() :
			num_segment(0), constell(SkyCulture::Ari), segments(NULL) {
	}
} constellation_line_index[NUM_CONSTELL];

SkyCulture::SkyCulture() {
}

void SkyCulture::init(){
	char p[1024];
	char *save;
	const char *delim = " \n";
	for (int i = 0; i < NUM_CONSTELL; i++) {
		strcpy(p, constellation_lines_raw[i]);
		strtok_r(p, delim, &save);
		constellation_line_index[i].num_segment = strtol(
				strtok_r(NULL, delim, &save), NULL, 10); // Number of segment
		Segment *p = NULL;
		for (int j = 0; j < constellation_line_index[i].num_segment; j++) {
			Segment *q = new Segment;
			q->next = p;
			int id1 = strtol(strtok_r(NULL, delim, &save), NULL, 10);
			int id2 = strtol(strtok_r(NULL, delim, &save), NULL, 10); // Read ID of the stars of the segment
			q->star1 = StarCatalog::getInstance().searchByID(id1);
			q->star2 = StarCatalog::getInstance().searchByID(id2);
			q->x1 = cosf(q->star1->DEC * M_PI / 180.0f)
					* cosf(q->star1->RA * M_PI / 180.0f);
			q->y1 = cosf(q->star1->DEC * M_PI / 180.0f)
					* sinf(q->star1->RA * M_PI / 180.0f);
			q->z1 = sinf(q->star1->DEC * M_PI / 180.0f);
			q->x2 = cosf(q->star2->DEC * M_PI / 180.0f)
					* cosf(q->star2->RA * M_PI / 180.0f);
			q->y2 = cosf(q->star2->DEC * M_PI / 180.0f)
					* sinf(q->star2->RA * M_PI / 180.0f);
			q->z2 = sinf(q->star2->DEC * M_PI / 180.0f);
			p = q;
		}
		constellation_line_index[i].segments = p;
	}
}

SkyCulture::~SkyCulture() {
	for (int i = 0; i < NUM_CONSTELL; i++) {
		Segment *p = constellation_line_index[i].segments;
		while (p) {
			Segment *q = p->next;
			delete p;
			p = q;
		}
	}
}

void SkyCulture::_resetIterator(Iterator &it) {
	it.constell = 0;
	it.segment = constellation_line_index[0].segments;
}

SkyCulture::Segment* SkyCulture::_nextSegment(Iterator &it) {
	while (!it.segment) {
		it.constell++;
		if (it.constell == NUM_CONSTELL) {
			// Nothing more to iterate
			return NULL;
		}
		it.segment = constellation_line_index[it.constell].segments;
	}

	Segment *sg = it.segment;
	it.segment = it.segment->next;
	return sg;
}


SkyCulture SkyCulture::instance;

SkyCulture& SkyCulture::getInstance() {
	return instance;
}
