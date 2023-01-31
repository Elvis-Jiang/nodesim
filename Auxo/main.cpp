#include <iostream>
#include "querysupportstruct.h"
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <math.h>
#include <set>
using namespace std;
int main (int argc, char* argv[])
{
    string dataset="../../data/lkml/rawdata.txt",result="lkml.txt",type="auxo";
    int fpl=16,width=1000,cols=4;
    Auxo *auxo;
    ComAuxo *pauxo;
    GSS_Chain *gss;


    for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], "-dataset") == 0) {
			dataset = argv[++i];
		}
		if (strcmp(argv[i], "-result") == 0) {
			result = argv[++i];
		}
		if (strcmp(argv[i], "-type") == 0) {
			type = argv[++i];
		}
		if (strcmp(argv[i], "-fpl") == 0) {
			fpl = atoi(argv[++i]);
		}
		if (strcmp(argv[i], "-width") == 0) {
			width = atoi(argv[++i]);
		}
		if (strcmp(argv[i], "-cols") == 0) {
			cols = atoi(argv[++i]);
		}
    }
    cout<<"dataset: "<<dataset<<" result: "<<result<<" cols: "<<cols<<" mode: "<<type<<endl;

    if(type == "auxo"){
        auxo = new Auxo(width,cols,16,fpl);
    }
    else if (type == "pauxo"){
        pauxo = new ComAuxo(width,cols,16,fpl);
    }
    else{
        gss = new GSS_Chain(width,cols,16,fpl);
    }

    ComAuxo *groundTruth = new ComAuxo(width,cols,16,32);

    int interval=100000;
    ofstream out1(result);
    int sampleSize = 1000;
    int64_t from[sampleSize],to[sampleSize];
    int nowPos=0;

	ifstream ifs;
    ifs.open(dataset);
	if (!ifs.is_open()) {
		cout << "Error in open file, Path = " << dataset << endl;
		return -1;
	}
	cout << "Inserting..." << endl;

    int64_t s, d;
    int w;
    unsigned int t;
    long flag = 1, level = 0, datanum = 0;

    timeval test_start, test_end;

    timeval bpoint_start, bpoint_end;
    gettimeofday( &bpoint_start, NULL);

    double MatrixTime=0;

    vector<int> target;
    int num=0,nowNum=interval;
    while(nowNum<1000000000){
        target.push_back(nowNum);
        target.push_back(4*nowNum);
        target.push_back(7*nowNum);
        nowNum*=10;
    }

    if(type=="auxo"){
        while (!ifs.eof()) {
            ifs >> s >> d >> w >> t;
            //cout<<s<<" "<<d<<endl;
            from[nowPos]=s;
            to[nowPos++]=d;
            nowPos%=sampleSize;
            if (ifs.fail())
                break;
            auxo->insert(to_string(s), to_string(d), w, MatrixTime);
            datanum++;

            if (datanum % interval == 0) {
                gettimeofday(&bpoint_end, NULL);
                double bpoint_time =
                        (bpoint_end.tv_sec - bpoint_start.tv_sec)*1000000.0 + (bpoint_end.tv_usec - bpoint_start.tv_usec);
                out1 << datanum <<": update time: "<<bpoint_time/interval<<"us";
                out1 << " overall memory cost: "<<(double)(auxo->memoryAllocated2+auxo->memoryAllocated1+auxo->memoryAllocated3)/131072<<" M";
                out1 << " no weight memory cost: "<<(double)(auxo->memoryAllocated2+auxo->memoryAllocated3)/131072<<" M";


//                {
//                    gettimeofday( &test_start, NULL);
//                    for(int i=0;i<100;i++){
//                        int pos=rand()%sampleSize;
//                        auxo->edgeQuery(to_string(from[pos]), to_string(to[pos]),MatrixTime);
//                    }
//                    gettimeofday( &test_end, NULL);
//                    out1<<" edge query time: "<<((test_end.tv_sec - test_start.tv_sec)* 1000000.0 +
//                    (test_end.tv_usec - test_start.tv_usec))/100;
//                 }
//
//                 {
//                    gettimeofday( &test_start, NULL);
//                    for(int i=0;i<100;i++){
//                        int pos=rand()%sampleSize;
//                        auxo->nodeValueQuery(to_string(from[pos]),0, MatrixTime);
//                    }
//                    gettimeofday( &test_end, NULL);
//                    out1<<" node out query time: "<<((test_end.tv_sec - test_start.tv_sec)* 1000000.0 +
//                                                 (test_end.tv_usec - test_start.tv_usec))/100;
//                 }
//
//                 {
//                    gettimeofday( &test_start, NULL);
//                    for(int i=0;i<100;i++){
//                        int pos=rand()%sampleSize;
//                        auxo->nodeValueQuery(to_string(to[pos]),1,MatrixTime);
//                    }
//                    gettimeofday( &test_end, NULL);
//                    out1<<" node in query time: "<<((test_end.tv_sec - test_start.tv_sec)* 1000000.0 +
//                                                     (test_end.tv_usec - test_start.tv_usec))/100;
//                 }

                 {
                    gettimeofday( &test_start, NULL);
                    for(int i=0;i<100;i++){
                        int pos1=rand()%sampleSize;
                        int pos2=rand()%sampleSize;
                        auxo->neighborSimilarity(to_string(from[pos1]),to_string(from[pos2]));
                    }
                    gettimeofday( &test_end, NULL);
                    out1<<"node similarity query time: "<<((test_end.tv_sec - test_start.tv_sec)* 1000000.0 +
                                                     (test_end.tv_usec - test_start.tv_usec))/100;
                 }
                 out1.flush();
                 out1<<endl;
                 gettimeofday(&bpoint_start, NULL);
            }

         }
    }
    else if (type == "pauxo"){
        while (!ifs.eof()) {
            ifs >> s >> d >> w >> t;
            from[nowPos]=s;
            to[nowPos++]=d;
            nowPos%=sampleSize;
            if (ifs.fail())
                break;
            pauxo->insert(to_string(s), to_string(d), w, MatrixTime);
//            groundTruth->insert(to_string(s), to_string(d), w, MatrixTime);
            datanum++;
//            if(target[num]==datanum){
//                int fake=pauxo->triangleCounting(),real=groundTruth->triangleCounting();
//                out1<<datanum<<" "<<abs(real-fake)/(double)real<<" "<<real<<" "<<fake<<endl;
//                num++;
//            }

            if (datanum % interval == 0) {
                gettimeofday(&bpoint_end, NULL);
                double bpoint_time =
                        (bpoint_end.tv_sec - bpoint_start.tv_sec)*1000000.0 + (bpoint_end.tv_usec - bpoint_start.tv_usec);
                out1 << datanum <<": update time: "<<bpoint_time/interval<<" ";
                out1 << " overall memory cost: "<<(double)(pauxo->memoryAllocated2+pauxo->memoryAllocated1+pauxo->memoryAllocated3)/131072<<" M";
                out1 << " no weight memory cost: "<<(double)(pauxo->memoryAllocated2+pauxo->memoryAllocated3)/131072<<" M";

//                {
//                    gettimeofday( &test_start, NULL);
//                    for(int i=0;i<100;i++){
//                        int pos=rand()%sampleSize;
//                        pauxo->edgeQuery(to_string(from[pos]), to_string(to[pos]),MatrixTime);
//                    }
//                    gettimeofday( &test_end, NULL);
//                    out1<<" edge query time: "<<((test_end.tv_sec - test_start.tv_sec)* 1000000.0 +
//                    (test_end.tv_usec - test_start.tv_usec))/100;
//                 }
//
//                 {
//                    gettimeofday( &test_start, NULL);
//                    for(int i=0;i<100;i++){
//                        int pos=rand()%sampleSize;
//                        pauxo->nodeValueQuery(to_string(from[pos]),0, MatrixTime);
//                    }
//                    gettimeofday( &test_end, NULL);
//                    out1<<" node out query time: "<<((test_end.tv_sec - test_start.tv_sec)* 1000000.0 +
//                                                 (test_end.tv_usec - test_start.tv_usec))/100;
//                 }
//
//                 {
//                    gettimeofday( &test_start, NULL);
//                    for(int i=0;i<100;i++){
//                        int pos=rand()%sampleSize;
//                        pauxo->nodeValueQuery(to_string(to[pos]),1,MatrixTime);
//                    }
//                    gettimeofday( &test_end, NULL);
//                    out1<<" node in query time: "<<((test_end.tv_sec - test_start.tv_sec)* 1000000.0 +
//                                                     (test_end.tv_usec - test_start.tv_usec))/100;
//                 }

                 {
                    gettimeofday( &test_start, NULL);
                    for(int i=0;i<100;i++){
                        int pos1=rand()%sampleSize;
                        int pos2=rand()%sampleSize;
                        pauxo->neighborSimilarity(to_string(from[pos1]),to_string(from[pos2]));
                    }
                    gettimeofday( &test_end, NULL);
                    out1<<"node similarity query time: "<<((test_end.tv_sec - test_start.tv_sec)* 1000000.0 +
                                                     (test_end.tv_usec - test_start.tv_usec))/100;
                 }
                 out1.flush();
                 out1<<endl;
                 gettimeofday(&bpoint_start, NULL);
            }

         }
    }
    else{
        while (!ifs.eof()) {
            ifs >> s >> d >> w >> t;
            from[nowPos]=s;
            to[nowPos++]=d;
            nowPos%=sampleSize;
            if (ifs.fail())
                break;
            gss->insert(to_string(s), to_string(d), w, MatrixTime);
            datanum++;

            if (datanum % interval == 0) {
                gettimeofday(&bpoint_end, NULL);
                double bpoint_time =
                        (bpoint_end.tv_sec - bpoint_start.tv_sec)*1000000.0 + (bpoint_end.tv_usec - bpoint_start.tv_usec);
                out1 << datanum <<": update time: "<<bpoint_time/interval;
                out1 << " overall memory cost: "<<(double)(gss->memoryAllocated2+gss->memoryAllocated1+gss->memoryAllocated3)/131072<<" M";
                out1 << " no weight memory cost: "<<(double)(gss->memoryAllocated2+gss->memoryAllocated3)/131072<<" M";

//                {
//                    gettimeofday( &test_start, NULL);
//                    for(int i=0;i<100;i++){
//                        int pos=rand()%sampleSize;
//                        gss->edgeQuery(to_string(from[pos]), to_string(to[pos]),MatrixTime);
//                    }
//                    gettimeofday( &test_end, NULL);
//                    out1<<" edge query time: "<<((test_end.tv_sec - test_start.tv_sec)* 1000000.0 +
//                    (test_end.tv_usec - test_start.tv_usec))/100;
//                 }
//
//                 {
//                    gettimeofday( &test_start, NULL);
//                    for(int i=0;i<100;i++){
//                        int pos=rand()%sampleSize;
//                        gss->nodeValueQuery(to_string(from[pos]),0, MatrixTime);
//                    }
//                    gettimeofday( &test_end, NULL);
//                    out1<<" node out query time: "<<((test_end.tv_sec - test_start.tv_sec)* 1000000.0 +
//                                                 (test_end.tv_usec - test_start.tv_usec))/100;
//                 }
//
//                 {
//                    gettimeofday( &test_start, NULL);
//                    for(int i=0;i<100;i++){
//                        int pos=rand()%sampleSize;
//                        gss->nodeValueQuery(to_string(to[pos]),1,MatrixTime);
//                    }
//                    gettimeofday( &test_end, NULL);
//                    out1<<" node in query time: "<<((test_end.tv_sec - test_start.tv_sec)* 1000000.0 +
//                                                     (test_end.tv_usec - test_start.tv_usec))/100;
//                 }

                 {
                    gettimeofday( &test_start, NULL);
                    for(int i=0;i<100;i++){
                        int pos1=rand()%sampleSize;
                        int pos2=rand()%sampleSize;
                        gss->neighborSimilarity(to_string(from[pos1]),to_string(from[pos2]));
                    }
                    gettimeofday( &test_end, NULL);
                    out1<<"node similarity query time: "<<((test_end.tv_sec - test_start.tv_sec)* 1000000.0 +
                                                     (test_end.tv_usec - test_start.tv_usec))/100;
                 }
                 out1.flush();
                 out1<<endl;
                 gettimeofday(&bpoint_start, NULL);
            }

         }
    }
    ifs.close();
    out1.close();
    return 0;
}
