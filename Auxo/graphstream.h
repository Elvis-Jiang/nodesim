#ifndef GRAPHSTREAM_H_INCLUDED
#define GRAPHSTREAM_H_INCLUDED

class Auxo
{
private:
	int w;
	int r;
	int p;
	int f;
	int rfnum;
	int halfR;
	int rper;
	unsigned int* fvalue;//fvalue[i] means (1<<f)-1;
	int *fper;//fper[i] means the number of fingerprint an uint64 can contain
	Blizzard_MPQ *mpq;

	vector<basketNode*> checkMatrices;
    vector<int> posfind;
    vector<int> weiRes;
    vector<int> matrixLevel;


	public:
	    double fullCount;
	    double loadFactor;
	    basketNode *mainTree;
	    ul valueCou;
	    uint64 memoryAllocated1,memoryAllocated2,memoryAllocated3;
	    double insertNum;
	    ul matrixSize;
		Auxo(int width, int range, int p_num, int f_num);
		~Auxo()
		{
			freeValueTree(mainTree);
			delete []fvalue;
			delete []fper;

		 }
		 Gbasket* initialGbasket(int fsize1,int fsize2);
		 void MatrixAppend(basketNode* basketCurr,int level);
		 bool insert(string s1, string s2,int weight,double& insertMatrix);
		 void cleanupBuffer();
		 int edgeQuery(string s1, string s2,double& queryMatrix);
		 bool query(string s1, string s2);
		 int nodeValueQuery(string s1, int type, double& queryMatrix);//src_type = 0 dst_type = 1
		 int nodeDegreeQuery(string s1, int type);//src_type = 0 dst_type = 1
		 void nodeSuccessorQuery(string s1, vector<string> &IDs);
		 void nodePrecursorQuery(string s2, vector<string> &IDs);
		 unsigned int getNodeValue(basketNode* basketCurr,int level,unsigned int *poses, unsigned int g1,bool type);
		 void freeValueTree(basketNode* root);
		 void sucessorQuery(basketNode* basketCurr,int level,unsigned int *poses, ul g1,vector<ul>& neis,ul prefix,int preLen,bool type);
		 bool reachQuery(string s1,string s2,double &queryTime);
};

#endif // GRAPHSTREAM_H_INCLUDED
