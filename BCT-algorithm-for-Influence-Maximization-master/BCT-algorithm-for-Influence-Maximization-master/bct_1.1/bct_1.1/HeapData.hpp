#ifndef _HEAP_DATA_H_
#define _HEAP_DATA_H_

template<class T1, class T2>
struct InfCost{
	T1 *v1;
	T2 *v2;
public:
	InfCost(T1 *u1, T2 *u2):v1(u1),v2(u2){};
	
	bool operator() (int &i, int &j) const{
		return v1[i]/v2[i] < v1[j]/v2[j];
	}
};

#endif
