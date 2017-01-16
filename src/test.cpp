#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <time.h>
#include <vector>
#include "dynamiccuckoofilter.h"


using namespace std;

typedef struct{
	size_t item_num;
	double exp_FPR;
	string dataset_path;
}Config;


typedef struct{
//	size_t item_num;
//	double exp_FPR;
	int exp_BBN;
	double actual_FPR;
	int actual_BBN;
	int F_size;
	double space_cost;
	double I_time;
	double Q_time;
	double D_time;
	double C_rate;
}Metric;




Metric test(const Config config, string *data){

	Metric metric;
	metric.exp_BBN = 6;
	DynamicCuckooFilter* dcf = new DynamicCuckooFilter(config.item_num, config.exp_FPR);


	//**********insert**********
	metric.I_time = clock();
	for(size_t i = 0; i<config.item_num; i++){
		dcf->insertItem(data[i].c_str());
	}
	metric.I_time = clock() - metric.I_time;
	metric.I_time = metric.I_time/CLOCKS_PER_SEC;

	metric.space_cost = dcf->size_in_mb();

	//**********query**********

	int false_positive_count = 0;

	metric.Q_time = clock();
	for(size_t i = 0; i<config.item_num; i++){
		if(dcf->queryItem(data[i].c_str()) == false){
			cout << "Item not found" << endl;
		};
	}
	metric.Q_time = clock() - metric.Q_time;
	metric.Q_time = metric.Q_time/CLOCKS_PER_SEC;

	//calculate false
	for(size_t i = 0; i<config.item_num; i++){
		char item[10] = {0};
		sprintf(item, "%ld", i + 1000*1000);
		if(dcf->queryItem(item)){
			false_positive_count++;
		}
	}


	metric.actual_FPR = (double)false_positive_count/config.item_num;
//	printf("false positve rate: %f\n", actual_FPR);


	//**********delete**********


	size_t count = 0;
	metric.D_time = clock();
	while(count < config.item_num){
		dcf->deleteItem(data[count].c_str());
		count += 1; //delete all the items
	}
	metric.D_time = clock() - metric.D_time;
	metric.D_time = metric.D_time/CLOCKS_PER_SEC;


	//**********compact**********

	int size_before = dcf->cf_list->num;
	dcf->compact();
	int size_after = dcf->cf_list->num;

	metric.actual_BBN = size_before;
	metric.C_rate = (double)(size_before-size_after)/size_before;
	metric.F_size = dcf->getFingerprintSize();

	return metric;

}



string Get_Value(string config_buff){
	string value;
	int pos = config_buff.find("=", 0);
	if (pos != -1)
	{
		pos++;
		value = config_buff.substr(pos, config_buff.length());
	} else {
		exit(1);
	}

	while(1){
		pos = value.find(" ", 0);
		if(pos >= 0){
			value = value.substr(pos+1, config_buff.length());
		}
		else{
			break;
		}
	}

	return value;
}


Config Read_Config(const string path){
	ifstream in_config(path.c_str());
	string config_buff;
	Config configuration;
	getline(in_config, config_buff);
//	configuration.exp_FPR = atof(config_buff.c_str());
	configuration.exp_FPR = atof(Get_Value(config_buff).c_str());
	getline(in_config, config_buff);
//	configuration.item_num = atof(config_buff.c_str());
	configuration.item_num = atof(Get_Value(config_buff).c_str());
	getline(in_config, config_buff);
	configuration.dataset_path = Get_Value(config_buff);

	return configuration;
}


string* Read_Dataset(const Config config, const string path){
	ifstream in_input(path.c_str());
	string input_buff;
	if(!getline(in_input, input_buff)){
		cout << "Read File Error!" << endl;
	}
	string buff;
	stringstream ss(input_buff);

	vector<string> tokens;
	size_t item_count = 0;
	string *input_data = new string[config.item_num];
	while (ss >> buff){
		tokens.push_back(buff);
		input_data[item_count] = buff;
		item_count ++;
	}
	return input_data;
}

void Print_Info(Config config, Metric metric){

	ofstream out("output/result.txt");

	out << setw(10) << "item_num" << setw(15) << "exp_FPR"
		<< setw(15) << "actual_FPR" << setw(15) << "actual_BBN" << setw(15) << "F_size(bits)"
		<< setw(15) << "space_cost(MB)"
		<< setw(15) << "I_time(s)" << setw(15) << "Q_time(s)" << setw(15) << "D_time(s)" << setw(10) << "C_rate"
		<< endl;

	out << setw(10) << config.item_num << setw(15) << config.exp_FPR
		<< setw(15) << metric.actual_FPR << setw(15) << metric.actual_BBN << setw(15) << metric.F_size
		<< setw(15) << metric.space_cost
		<< setw(15) << metric.I_time << setw(15) << metric.Q_time << setw(15) << metric.D_time << setw(10) << metric.C_rate
		<< endl;
}




int main(int argc, char* argv[]){

	string config_path = "configuration/config.txt";
	Config config = Read_Config(config_path);

	string dataset_path = config.dataset_path;
	string *data = Read_Dataset(config, dataset_path);


    Metric metric = test(config, data);

    Print_Info(config, metric);

}

