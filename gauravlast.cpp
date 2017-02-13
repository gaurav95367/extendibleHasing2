#include <bits/stdc++.h>
#define BUCKETSIZE 2
using namespace std;
int bucketnum=0;
bool kthbitfromlast(int num,int k){
	if(k==1)
		return num%2;
	return kthbitfromlast(num/2,k-1);}

struct bucket{
	int localdepth;
	int bucketid;
	vector<int> values;
	bucket(){
		localdepth=0;
		bucketid=bucketnum++;
	}
};

struct directory{
	int globaldepth;
	vector< bucket* > buckets;
	
	void print(){
		cout<<"globaldepth = "<<globaldepth<<endl;
		//int counter=0,counter2=0;
		for(int i=0;i<buckets.size();i++){
			//counter++;
			cout<<"bucket "<<i<<" = ";
			vector<int> bucketvalues = buckets[i]->values;
			for(int j=0;j<bucketvalues.size();j++)
				cout<<bucketvalues[j]<<" ";
			cout<<" end  bucket address = "<<buckets[i]<<" localdepth="<<buckets[i]->localdepth<<endl;
			// if(counter == 50)
			// 	break;
		}
		cout<<endl;
	}
	
	void insert(int val){
		//cout<<"now insert "<<val<<endl;
		bucket *b = buckets[val%buckets.size()];
		if(b->values.size() < BUCKETSIZE){
			b->values.push_back(val);
			//cout<<"here ! bitches for val="<<val<<endl;
			return;
		}
		
		
		
		if(b->localdepth < globaldepth){
			split(b);
		//	cout<<"done splitting"<<endl;
		//	print();
			for(int i=0;i<b->values.size();i++)
				insert(b->values[i]);
			free(b);
			insert(val);
			
			return;	}
		//now double the directory....
		globaldepth++;
		//cout<<"depth increased to ="<<globaldepth<<endl;
		buckets.resize(buckets.size()*2);
		for(int i=buckets.size()/2;i<buckets.size();i++)
			buckets[i] = buckets[i - buckets.size()/2];
		//print();
		//cout<<"again calling insert for val="<<val<<endl;
		insert(val);}
	
	void split(bucket *b){
		bucket *b1 = new bucket;
		bucket *b2 = new bucket;
		b1->localdepth = b->localdepth+1;
		b2->localdepth = b->localdepth+1;
		//cout<<"i am here"<<endl;
		for(int i=0;i<buckets.size();i++)
			if(buckets[i] == b){
				//cout<<"i="<<i<<endl;
				if(kthbitfromlast(i,b1->localdepth) == 0)
					buckets[i] =b1;
				else
					buckets[i] = b2;	
			
			}}
	
	bucket * search(int val){
		bucket *b = buckets[val%buckets.size()];
		for(int i=0;i<b->values.size();i++){
			if(b->values[i] == val)
				return b;}
		return NULL;}



	bucket* lazydelete(int val){
		bucket *b = search(val);
		for(int i=0;i<b->values.size();i++)
			if(b->values[i] == val){
				b->values.erase(b->values.begin()+i);
				break;
			}
		return b;	
	}
	void bucketmergedelete(int val)
	{
		bucket *b=lazydelete(val);
		bucket *brother=NULL;
		if(b->localdepth == 0)
			return;
		int num = pow(2,b->localdepth-1);
		int check;
		for(int i=0;i<buckets.size();i++)
			if(buckets[i] == b){
				check =i; 
				break;
			}
		for(int i=0;i<buckets.size();i++){
			if(buckets[i] !=b && i%num == check%num){
				brother = buckets[i];
				break;
			}

		}
		

		if(b->values.size()+brother->values.size() > BUCKETSIZE)
			return ;
		for(int i=0;i<brother->values.size();i++)
			b->values.push_back(brother->values[i]);
		for(int i=0;i<buckets.size();i++)
			if(buckets[i] == brother)
				buckets[i] = b;
		b->localdepth--;
		free(brother);	
	}

	bool checkDirectoryMergable(){
		if(buckets.size() == 1)
			return false;
		for(int i=buckets.size()/2;i<buckets.size();i++){
			if(buckets[i]->values.size()==0)
				continue;
			if(buckets[i -(buckets.size()/2)]->values.size()==0)
				continue;
			if(buckets[i] !=buckets[i - (buckets.size()/2)])
				return false;
		}
		return true;	
	}
	void collapsedirectory(){
		if(!checkDirectoryMergable())
			return;
		for(int i=0;i<buckets.size()/2;i++)
			if(buckets[i]->values.size() ==0)
				buckets[i] = buckets[i + (buckets.size()/2)];
		buckets.resize(buckets.size()/2);
		globaldepth--;
		for(int i=0;i<buckets.size();i++)
			if(buckets[i]->localdepth > globaldepth)
				buckets[i]->localdepth = globaldepth;
		cout<<"collapsedirectory sucessfully"<<endl;
		print();
		collapsedirectory();
	}
	void directorymergedelete(int val){
		cout<<"deleting "<<val<<endl;
		bucketmergedelete(val);
		cout<<"deleted now collapsing	 "<<endl;
		collapsedirectory();
	}
};



int main(){
	bucket *b = new bucket;
	b->localdepth=0;
	directory d;
	d.globaldepth = 0;
	d.buckets.push_back(b);

	 for(int i=0;i<=10;i++){
	 	d.insert(4*i);
//		d.print();	 	
	 }
	 d.insert(1);
	 d.insert(33);
	 d.print();
	 d.insert(65);
	 d.print();
	// d.insert(8);
	// d.insert(16);
	// d.insert(32);
	// d.print();


return 0;
}
