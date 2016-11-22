/*
 * dcMSTree.cpp
 *
 *  Created on: 2015. 8. 10.
 *      Author: decoz
 */

#include "dcMSTree.h"
#include "math.h"
#include "stdio.h"

dcMSTree::dcMSTree() {
	// TODO Auto-generated constructor stub

}

dcMSTree::~dcMSTree() {
	// TODO Auto-generated destructor stub
}

void dcMSTree::input(Point p){
	list.push_back(p);
}

double dcMSTree::dist(Point p1, Point p2){

	int rx = p1.x - p2.x;
	int ry = p1.y - p2.y;
	return sqrt(rx*rx + ry*ry);

}

bool compareLink(plink l1, plink l2){
	if( l1.l > l2.l ) return false;
	else return true;
}

vector <Link> dcMSTree::createTree(){

	int s = list.size();
	printf("p_count:%d\n", s );


	vector <plink> v_l;
	vector <Link> v_L;

	for(int i=0;i<s;i++)
		for(int j=i + 1;j<s;j++){
			plink l;
			l.pi1 = i; l.pi2 = j;
			l.l = dist( list[i], list[j]);
			if(l.l > 0) v_l.push_back(l);
		}

	sort(v_l.begin(), v_l.end(), compareLink);

	int *g = new int[s];
	int gc = 0;

	for(int i=0;i<s;i++) g[i] = 0;
	for(int i=0;i<v_l.size();i++){
		int g1 = g[v_l[i].pi1];
		int g2 = g[v_l[i].pi2];
		int  pi1 = v_l[i].pi1, pi2 = v_l[i].pi2;

		if(g1 && g1 == g2  ) continue;

		if(!g1 && !g2) g[pi1] = g[pi2] = ++gc;

		if(g1 && !g2) g[ pi2  ] = g1;
		if(!g1 && g2) g[ pi1 ] = g2;

		if(g1 && g2)
			for(int j=0;j<s;j++) if( g[j] == g2 ) g[j] = g1;

		v_L.push_back(Link( list[pi1] , list[pi2] ) ) ;
	}

	printf("Link count : %d / %d\n", v_L.size(), v_l.size());
	return v_L;


};
