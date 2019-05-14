#ifndef PARTITION_H
#define PARTITION_H

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <stdint.h>
#include <algorithm>
#include <mpi.h>
#include <glog/logging.h>
#include <cmath>

#include "units.h"

using namespace std;

extern int procid, numprocs;
extern bool debug;

class Vertex;
class Edge;
class Partition;
class InstancePartitions;

class Vertex{
public:
	uint32_t ver;
	// uint32_t degree;
};

class Edge{
public:
	Vertex src;
	Vertex dst;

	bool operator<(const Edge& e) const{
		if(this->src.ver < e.src.ver){
			return true;
		}else if(this->src.ver == e.src.ver){
			return this->dst.ver < e.dst.ver;
		}else{
			return false;
		}
	}
};

class Partition{
public:
	void getVerticesAndDegree();
	
	void getVertexScore(InstancePartitions* ins_partition);
	void getHotColdVertices(InstancePartitions* ins_partition, double hot, double cold); // 热点与冷点比例
	
	void getEdgeScore();
	void getColdEdges(double cold); // 冷边所占比例

	// for debug
	void printHotVertices();
	void printColdEdges();
	void printAllVertices();
	void printAllEdges();

public:
	vector<Edge> edges;
	set<uint32_t> vertices;

	vector<uint32_t> hotVertices;
	vector<uint32_t> coldVertices;
	vector<Edge> hotEdges;
	vector<Edge> coldEdges;

	map<uint32_t, uint32_t> vertexPartDegree; // 局部度信息
	map<uint32_t, double> vertexScore;        // 点的score
	map<Edge, double> edgeSocre;              // 边的score
};

// 每一个进程所拥有的全部partition 类
class InstancePartitions{
public:
	InstancePartitions(uint32_t allparts, uint32_t* numparts);
	void getAllVerticesDegree();
	void getVRF();
	void getBalance();

	void getAllHotVertices();

	void computeEdgesMatchPartitions();
	double computerMatchScore(Edge e, int part);

	void distributeAllColdEdges();
	void updateAllPartitions();

	void InstanceInit();

	bool InstanceIteration(); 
	bool InstanceIteration(int times);

	// for debug
	void print4debug(int part);           // 全局第几个partition

public:
	
	uint32_t allparts;                    // 总partitions数
    vector<uint32_t> numparts;            // 每个进程partitions数
	uint32_t startpart;                   // 本进程起始part值

	uint32_t nedges_global;               // 全部边数
	uint32_t nvertices_global;            // 全部点数

	vector<Partition*> partitions;        // 进程包含的partitions
	map<uint32_t, uint32_t> vertexDegree; // 每个进程保留全部点度信息

	vector<uint32_t> allHotVertices;      // 收集全部partitions的hotVertices
	vector<double> allHotVerticesScore;   // 所有热点对应的分值
	vector<uint32_t> InstanceIndexStart;  // 进程所含热点的起始点（包含该点）
	vector<uint32_t> InstanceIndexEnd;    // 进程所含热点的终止点（不包含该点）
	vector<uint32_t> PartitionIndexStart; // partition所含热点的起始点（包含该点）
	vector<uint32_t> PartitionIndexEnd;   // partition所含热点的终止点（不包含该点）
	vector<uint32_t> PartitionIndexLen;   // partition所含热点的数量

	map<Edge, int> coldEdges2Partition;   // 记录每一条边分配给了哪个partition

	double balance_RSD = 0;
	double balance_MAX_MIN = 0;
	double VRF = 0;
};

#endif