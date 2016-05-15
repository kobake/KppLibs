#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "c_IDManager.h"

IDManager* IDManager::clone()
{
	return new IDManager(*this);
}

IDManager::IDManager(IDManager &right)
{
	min_id=right.min_id;
	max_id=right.max_id;
	invalid_id=right.invalid_id;
	ntable=right.ntable;
	table=(int*)malloc(sizeof(int)*ntable);
	memcpy(table,right.table,sizeof(int)*ntable);
	id_reserve=right.id_reserve;
}

IDManager::IDManager(int _min_id,int _max_id,int _invalid_id)
{
	min_id=_min_id;
	max_id=_max_id;
	invalid_id=_invalid_id;
	ntable=max_id-min_id+1;
	table=(int*)calloc(ntable,sizeof(int));
	id_reserve=invalid_id;
}

IDManager::~IDManager()
{
	free(table);
}

bool IDManager::availableMore()
{
	int i;
	if(id_reserve!=invalid_id)
		return true;
	for(i=0;i<ntable;i++){
		if(table[i]==0){
			id_reserve=min_id+i;
			return true;
		}
	}
	id_reserve=invalid_id;
	return false;
}

int IDManager::createID()
{
	if(id_reserve!=invalid_id){
		int id=id_reserve;
		table[id-min_id]=1;
		id_reserve=invalid_id;
		return id;
	}else{
		if(availableMore()){
			int id=id_reserve;
			table[id-min_id]=1;
			id_reserve=invalid_id;
			return id;
		}
	}
	return invalid_id;
}
int IDManager::createID(int new_id)
{
	if(isAvailableID(new_id)){
		table[new_id-min_id]=1;
		if(id_reserve==new_id){
			id_reserve=invalid_id;
		}
		return new_id;
	}
	return invalid_id;
}

void IDManager::destroyID(int id)
{
	if(id>=min_id && id<=max_id){
		table[id-min_id]=0;
	}
}
bool IDManager::isValidID(int id)
{
	if(id>=min_id && id<=max_id){
		return table[id-min_id]!=0;
	}else{
		return false;
	}
}

bool IDManager::isAvailableID(int id)
{
	if(id>=min_id && id<=max_id){
		return table[id-min_id]==0;
	}else{
		return false;
	}
}

