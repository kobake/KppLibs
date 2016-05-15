#pragma once

//ƒ†ƒj[ƒN‚ÈID‚ğì¬
class IDManager{
protected:
	int min_id;
	int max_id;
	int invalid_id;
	int *table;
	int ntable;
	int id_reserve;
public:
	IDManager* clone();
	IDManager(IDManager &right);
	IDManager(int _min_id,int _max_id,int _invalid_id=-1);
	~IDManager();
	bool availableMore();
	int createID();
	int createID(int new_id);
	void destroyID(int id);
	bool isValidID(int id);
	bool isAvailableID(int id);
};

