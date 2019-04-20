#pragma once
#include <vector>


using namespace std;

class Queen {

public:

	int numOfQueensPlaced;
	bool isAttacked;
	bool isPlaced;
	vector<int> queens;
	

	void setCol();
	void setRow();


private:


};