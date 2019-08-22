#include "Messier.h"

__attribute__((section ("ExtFlashSection"))) SkyObjInfo messier_catalog[MESSIER_CATALOG_SIZE]={
	{1,"NGC1952",83.625,22.017,"Crab Nebula",0,8.4,8.4,0,SkyObjInfo::SKYOBJ_MESSIER},
	{2,"NGC7089",-36.625,0.817,"",0,6.5,6.5,0,SkyObjInfo::SKYOBJ_MESSIER},
	{3,"NGC5272",-154.45,28.383,"",0,6.4,6.4,0,SkyObjInfo::SKYOBJ_MESSIER},
	{4,"NGC6121",-114.1,-26.533,"",0,5.9,5.9,0,SkyObjInfo::SKYOBJ_MESSIER},
	{5,"NGC5904",-130.35,2.083,"",0,5.8,5.8,0,SkyObjInfo::SKYOBJ_MESSIER},
	{6,"NGC6405",-94.975,-32.217,"Butterfly Cluster",0,4.2,4.2,0,SkyObjInfo::SKYOBJ_MESSIER},
	{7,"NGC6475",-91.525,-34.817,"Ptolemy Cluster",0,3.3,3.3,0,SkyObjInfo::SKYOBJ_MESSIER},
	{8,"NGC6523",-89.05,-24.383,"Lagoon Nebula",0,5.8,5.8,0,SkyObjInfo::SKYOBJ_MESSIER},
	{9,"NGC6333",-100.2,-18.517,"",0,7.9,7.9,0,SkyObjInfo::SKYOBJ_MESSIER},
	{10,"NGC6254",-105.725,-4.1,"",0,6.6,6.6,0,SkyObjInfo::SKYOBJ_MESSIER},
	{11,"NGC6705",-77.225,-6.267,"Wild Duck Cluster",0,5.3,5.3,0,SkyObjInfo::SKYOBJ_MESSIER},
	{12,"NGC6218",-108.2,-1.95,"",0,6.6,6.6,0,SkyObjInfo::SKYOBJ_MESSIER},
	{13,"NGC6205",-109.575,36.467,"Great Globular",0,5.3,5.3,0,SkyObjInfo::SKYOBJ_MESSIER},
	{14,"NGC6402",-95.6,-3.25,"",0,7.6,7.6,0,SkyObjInfo::SKYOBJ_MESSIER},
	{15,"NGC7078",-37.5,12.167,"",0,6.4,6.4,0,SkyObjInfo::SKYOBJ_MESSIER},
	{16,"NGC6611",-85.3,-13.783,"Eagle Nebula",0,6,6,0,SkyObjInfo::SKYOBJ_MESSIER},
	{17,"NGC6618",-84.8,-16.183,"Omega Nebula",0,7,7,0,SkyObjInfo::SKYOBJ_MESSIER},
	{18,"NGC6613",-85.025,-17.133,"",0,6.9,6.9,0,SkyObjInfo::SKYOBJ_MESSIER},
	{19,"NGC6273",-104.35,-26.267,"",0,7.2,7.2,0,SkyObjInfo::SKYOBJ_MESSIER},
	{20,"NGC6514",-89.35,-23.033,"Trifid Nebula",0,8.5,8.5,0,SkyObjInfo::SKYOBJ_MESSIER},
	{21,"NGC6531",-88.85,-22.5,"",0,5.9,5.9,0,SkyObjInfo::SKYOBJ_MESSIER},
	{22,"NGC6656",-80.9,-23.9,"Sagittarius Cluster",0,5.1,5.1,0,SkyObjInfo::SKYOBJ_MESSIER},
	{23,"NGC6494",-90.8,-19.017,"",0,5.5,5.5,0,SkyObjInfo::SKYOBJ_MESSIER},
	{24,"IC4715",-85.775,18.483,"Sagittarius Star cloud",0,4.5,4.5,0,SkyObjInfo::SKYOBJ_MESSIER},
	{25,"IC4725",-82.1,-19.25,"",0,4.6,4.6,0,SkyObjInfo::SKYOBJ_MESSIER},
	{26,"NGC6694",-78.7,-9.4,"",0,8,8,0,SkyObjInfo::SKYOBJ_MESSIER},
	{27,"NGC6853",-60.1,22.717,"Dumbbell Nebula",0,8.1,8.1,0,SkyObjInfo::SKYOBJ_MESSIER},
	{28,"NGC6626",-83.875,-24.867,"",0,6.9,6.9,0,SkyObjInfo::SKYOBJ_MESSIER},
	{29,"NGC6913",-54.025,48.433,"Cooling Tower",0,6.6,6.6,0,SkyObjInfo::SKYOBJ_MESSIER},
	{30,"NGC7099",-34.9,-23.183,"",0,7.5,7.5,0,SkyObjInfo::SKYOBJ_MESSIER},
	{31,"NGC224",10.675,41.267,"Andromeda Galaxy",0,3.4,3.4,0,SkyObjInfo::SKYOBJ_MESSIER},
	{32,"NGC221",10.675,40.867,"",0,8.2,8.2,0,SkyObjInfo::SKYOBJ_MESSIER},
	{33,"NGC598",23.475,30.65,"Triangulum Galaxy",0,5.7,5.7,0,SkyObjInfo::SKYOBJ_MESSIER},
	{34,"NGC1039",40.5,42.783,"",0,5.2,5.2,0,SkyObjInfo::SKYOBJ_MESSIER},
	{35,"NGC2168",92.225,24.333,"",0,5.1,5.1,0,SkyObjInfo::SKYOBJ_MESSIER},
	{36,"NGC1960",84.025,34.133,"",0,6,6,0,SkyObjInfo::SKYOBJ_MESSIER},
	{37,"NGC2099",88.1,32.55,"",0,5.6,5.6,0,SkyObjInfo::SKYOBJ_MESSIER},
	{38,"NGC1912",82.175,35.833,"",0,6.4,6.4,0,SkyObjInfo::SKYOBJ_MESSIER},
	{39,"NGC7092",-36.95,48.433,"",0,4.6,4.6,0,SkyObjInfo::SKYOBJ_MESSIER},
	{40,"",-174.4,58.083,"Winnecke 4",0,8,8,0,SkyObjInfo::SKYOBJ_MESSIER},
	{41,"NGC2287",101.75,-20.017,"",0,4.5,4.5,0,SkyObjInfo::SKYOBJ_MESSIER},
	{42,"NGC1976",83.85,-5.45,"Orion Nebula",0,4,4,0,SkyObjInfo::SKYOBJ_MESSIER},
	{43,"NGC1982",83.9,-5.267,"De Mairan's Neb",0,6.8,6.8,0,SkyObjInfo::SKYOBJ_MESSIER},
	{44,"NGC2632",130.025,19.983,"Beehive Cluster",0,3.1,3.1,0,SkyObjInfo::SKYOBJ_MESSIER},
	{45,"NGC1952",56.75,24.117,"Pleiades",0,1.2,1.2,0,SkyObjInfo::SKYOBJ_MESSIER},
	{46,"NGC2437",115.45,-14.817,"",0,6.1,6.1,0,SkyObjInfo::SKYOBJ_MESSIER},
	{47,"NGC2422",114.15,-14.5,"",0,4.4,4.4,0,SkyObjInfo::SKYOBJ_MESSIER},
	{48,"NGC2548",123.45,-5.8,"",0,5.8,5.8,0,SkyObjInfo::SKYOBJ_MESSIER},
	{49,"NGC4472",-172.55,8,"",0,8.4,8.4,0,SkyObjInfo::SKYOBJ_MESSIER},
	{50,"NGC2323",105.8,-8.333,"",0,5.9,5.9,0,SkyObjInfo::SKYOBJ_MESSIER},
	{51,"NGC5194",-157.525,47.2,"Whirlpool Galaxy",0,8.1,8.1,0,SkyObjInfo::SKYOBJ_MESSIER},
	{52,"NGC7654",-8.95,61.583,"",0,6.9,6.9,0,SkyObjInfo::SKYOBJ_MESSIER},
	{53,"NGC5024",-161.775,18.167,"",0,7.7,7.7,0,SkyObjInfo::SKYOBJ_MESSIER},
	{54,"NGC6715",-76.225,-30.483,"",0,7.7,7.7,0,SkyObjInfo::SKYOBJ_MESSIER},
	{55,"NGC6809",-65,-30.967,"",0,7,7,0,SkyObjInfo::SKYOBJ_MESSIER},
	{56,"NGC6779",-70.85,30.183,"",0,8.2,8.2,0,SkyObjInfo::SKYOBJ_MESSIER},
	{57,"NGC6720",-76.6,33.033,"Ring Nebula",0,8.8,8.8,0,SkyObjInfo::SKYOBJ_MESSIER},
	{58,"NGC4579",-170.575,11.817,"",0,9.8,9.8,0,SkyObjInfo::SKYOBJ_MESSIER},
	{59,"NGC4621",-169.5,11.65,"",0,9.8,9.8,0,SkyObjInfo::SKYOBJ_MESSIER},
	{60,"NGC4649",-169.075,11.55,"",0,8.8,8.8,0,SkyObjInfo::SKYOBJ_MESSIER},
	{61,"NGC4303",-174.525,4.467,"face-on spiral",0,9.7,9.7,0,SkyObjInfo::SKYOBJ_MESSIER},
	{62,"NGC6266",-104.7,-30.117,"",0,6.6,6.6,0,SkyObjInfo::SKYOBJ_MESSIER},
	{63,"NGC5055",-150.3,42.033,"Sunflower Galaxy",0,8.6,8.6,0,SkyObjInfo::SKYOBJ_MESSIER},
	{64,"NGC4826",-165.825,21.683,"Black-eye Galaxy",0,8.5,8.5,0,SkyObjInfo::SKYOBJ_MESSIER},
	{65,"NGC3623",169.725,13.083,"Leo triplet",0,9.3,9.3,0,SkyObjInfo::SKYOBJ_MESSIER},
	{66,"NGC3627",170.05,12.983,"",0,9,9,0,SkyObjInfo::SKYOBJ_MESSIER},
	{67,"NGC2682",132.6,11.817,"",0,6.9,6.9,0,SkyObjInfo::SKYOBJ_MESSIER},
	{68,"NGC4590",-170.125,-26.75,"",0,8.2,8.2,0,SkyObjInfo::SKYOBJ_MESSIER},
	{69,"NGC6637",-82.15,-32.35,"",0,7.7,7.7,0,SkyObjInfo::SKYOBJ_MESSIER},
	{70,"NGC6681",-79.2,-32.3,"",0,8.1,8.1,0,SkyObjInfo::SKYOBJ_MESSIER},
	{71,"NGC6838",-61.55,18.783,"",0,4.4,4.4,0,SkyObjInfo::SKYOBJ_MESSIER},
	{72,"NGC6981",-46.625,-12.533,"",0,9.4,9.4,0,SkyObjInfo::SKYOBJ_MESSIER},
	{73,"NGC6994",-45.275,-12.633,"",0,4.4,4.4,0,SkyObjInfo::SKYOBJ_MESSIER},
	{74,"NGC628",24.175,15.783,"",0,9.2,9.2,0,SkyObjInfo::SKYOBJ_MESSIER},
	{75,"NGC6864",-58.475,-21.917,"",0,8.6,8.6,0,SkyObjInfo::SKYOBJ_MESSIER},
	{76,"NGC650",25.6,51.567,"Little Dumbbell",0,10.1,10.1,0,SkyObjInfo::SKYOBJ_MESSIER},
	{77,"NGC1068",40.675,0.017,"Seyfert Galaxy",0,8.8,8.8,0,SkyObjInfo::SKYOBJ_MESSIER},
	{78,"NGC2068",86.675,0.05,"",0,8,8,0,SkyObjInfo::SKYOBJ_MESSIER},
	{79,"NGC1904",81.125,-24.55,"",0,8,8,0,SkyObjInfo::SKYOBJ_MESSIER},
	{80,"NGC6093",-115.75,-22.983,"",0,7.2,7.2,0,SkyObjInfo::SKYOBJ_MESSIER},
	{81,"NGC3031",148.9,69.067,"Bode's Galaxy",0,6.8,6.8,0,SkyObjInfo::SKYOBJ_MESSIER},
	{82,"NGC3034",148.95,69.683,"Cigar Galaxy",0,8.4,8.4,0,SkyObjInfo::SKYOBJ_MESSIER},
	{83,"NGC5236",-155.75,-29.867,"Southern Pinwheel",0,10.1,10.1,0,SkyObjInfo::SKYOBJ_MESSIER},
	{84,"NGC4374",-173.725,12.883,"",0,9.3,9.3,0,SkyObjInfo::SKYOBJ_MESSIER},
	{85,"NGC4382",-173.65,18.183,"",0,9.3,9.3,0,SkyObjInfo::SKYOBJ_MESSIER},
	{86,"NGC4406",-173.45,12.95,"",0,9.2,9.2,0,SkyObjInfo::SKYOBJ_MESSIER},
	{87,"NGC4486",-172.3,12.4,"Virgo A",0,8.6,8.6,0,SkyObjInfo::SKYOBJ_MESSIER},
	{88,"NGC4501",-172,14.417,"",0,9.5,9.5,0,SkyObjInfo::SKYOBJ_MESSIER},
	{89,"NGC4552",-171.075,12.55,"",0,9.8,9.8,0,SkyObjInfo::SKYOBJ_MESSIER},
	{90,"NGC4569",-170.8,13.167,"",0,9.5,9.5,0,SkyObjInfo::SKYOBJ_MESSIER},
	{91,"NGC4548",-171.15,14.5,"",0,11,11,0,SkyObjInfo::SKYOBJ_MESSIER},
	{92,"NGC6341",-100.725,43.133,"",0,6.5,6.5,0,SkyObjInfo::SKYOBJ_MESSIER},
	{93,"NGC2447",116.15,-23.867,"",0,6.2,6.2,0,SkyObjInfo::SKYOBJ_MESSIER},
	{94,"NGC4736",-167.275,41.117,"",0,8.1,8.1,0,SkyObjInfo::SKYOBJ_MESSIER},
	{95,"NGC3351",161,11.7,"",0,9.7,9.7,0,SkyObjInfo::SKYOBJ_MESSIER},
	{96,"NGC3368",161.7,11.817,"",0,9.2,9.2,0,SkyObjInfo::SKYOBJ_MESSIER},
	{97,"NGC3587",168.7,55.017,"Owl Nebula",0,11.2,11.2,0,SkyObjInfo::SKYOBJ_MESSIER},
	{98,"NGC4192",-176.55,14.9,"",0,10.1,10.1,0,SkyObjInfo::SKYOBJ_MESSIER},
	{99,"NGC4254",-175.3,14.417,"",0,9.8,9.8,0,SkyObjInfo::SKYOBJ_MESSIER},
	{100,"NGC4321",-174.275,15.817,"",0,9.4,9.4,0,SkyObjInfo::SKYOBJ_MESSIER},
	{101,"NGC5457",-149.2,54.35,"Pinwheel Galaxy",0,7.7,7.7,0,SkyObjInfo::SKYOBJ_MESSIER},
	{102,"NGC5866",-133.375,55.767,"Spindle Galaxy",0,10.7,10.7,0,SkyObjInfo::SKYOBJ_MESSIER},
	{103,"NGC581",23.3,60.7,"",0,7.4,7.4,0,SkyObjInfo::SKYOBJ_MESSIER},
	{104,"NGC4594",-170,-11.617,"Sombrero",0,8.3,8.3,0,SkyObjInfo::SKYOBJ_MESSIER},
	{105,"NGC3379",161.95,12.583,"",0,9.3,9.3,0,SkyObjInfo::SKYOBJ_MESSIER},
	{106,"NGC4258",-175.25,47.3,"",0,8.3,8.3,0,SkyObjInfo::SKYOBJ_MESSIER},
	{107,"NGC6171",-111.875,-13.05,"",0,8.1,8.1,0,SkyObjInfo::SKYOBJ_MESSIER},
	{108,"NGC3556",167.875,55.667,"nearly edge-on",0,10,10,0,SkyObjInfo::SKYOBJ_MESSIER},
	{109,"NGC3992",179.4,53.383,"",0,9.8,9.8,0,SkyObjInfo::SKYOBJ_MESSIER},
	{110,"NGC205",10.125,41.683,"",0,8,8,0,SkyObjInfo::SKYOBJ_MESSIER},
};
