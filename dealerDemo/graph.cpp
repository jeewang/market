#include "graph.h"

void load_graph(char* filename, Graph& g)
{
	ifstream input;
	input.open(filename);
	
	int src;
	int dst;
	int i = 0;
	while(input >> src >> dst)
	{
		Edge e;
		e.src = src;
		e.dst = dst;

		g.subGraphs[i].edges.push_back(e);
		g.subGraphs[i].vertices.insert(e.src);
		g.subGraphs[i].vertices.insert(e.dst);
		if(g.subGraphs[i].vertex2SubDegree.count(e.src) == 0){
			g.subGraphs[i].vertex2SubDegree[e.src] = 1;
		}
		else{
			g.subGraphs[i].vertex2SubDegree[e.src] += 1;
		}
		if(g.subGraphs[i].vertex2SubDegree.count(e.dst) == 0){
			g.subGraphs[i].vertex2SubDegree[e.dst] = 1;
		}
		else{
			g.subGraphs[i].vertex2SubDegree[e.dst] += 1;
		}

		g.allEdges.push_back(e);
		g.allVertices.insert(e.src);
		g.allVertices.insert(e.dst);
		if(g.vertex2AllDegree.count(e.src) == 0){
			g.vertex2AllDegree[e.src] = 1;
		}
		else{
			g.vertex2AllDegree[e.src] += 1;
		}
		if(g.vertex2AllDegree.count(e.dst) == 0){
			g.vertex2AllDegree[e.dst] = 1;
		}
		else{
			g.vertex2AllDegree[e.dst] += 1;
		}

		i = (i+1)%g.subGraphs.size();
	}
}

void load_graph_prepartition(char* filename, Graph& g)
{
	ifstream input;
	input.open(filename);
	
	int src;
	int dst;
	int part;
	while(input >> src >> dst >> part)
	{
		Edge e;
		e.src = src;
		e.dst = dst;

		g.subGraphs[part].edges.push_back(e);
		g.subGraphs[part].vertices.insert(e.src);
		g.subGraphs[part].vertices.insert(e.dst);
		if(g.subGraphs[part].vertex2SubDegree.count(e.src) == 0){
			g.subGraphs[part].vertex2SubDegree[e.src] = 1;
		}
		else{
			g.subGraphs[part].vertex2SubDegree[e.src] += 1;
		}
		if(g.subGraphs[part].vertex2SubDegree.count(e.dst) == 0){
			g.subGraphs[part].vertex2SubDegree[e.dst] = 1;
		}
		else{
			g.subGraphs[part].vertex2SubDegree[e.dst] += 1;
		}

		g.allEdges.push_back(e);
		g.allVertices.insert(e.src);
		g.allVertices.insert(e.dst);
		if(g.vertex2AllDegree.count(e.src) == 0){
			g.vertex2AllDegree[e.src] = 1;
		}
		else{
			g.vertex2AllDegree[e.src] += 1;
		}
		if(g.vertex2AllDegree.count(e.dst) == 0){
			g.vertex2AllDegree[e.dst] = 1;
		}
		else{
			g.vertex2AllDegree[e.dst] += 1;
		}
	}
}

double getVRF(Graph& g){
	int vertices = 0;
	for(int i = 0; i < g.subGraphs.size(); i++){
		vertices += g.subGraphs[i].vertices.size();
	}
	return vertices / 1.0 / g.allVertices.size();
}

double getBalance(Graph& g){
	int maxSize = 0;
	int minSize = 100000000;
	for(int i = 0; i < g.subGraphs.size(); i++){
		if(maxSize < g.subGraphs[i].edges.size()){
			maxSize = g.subGraphs[i].edges.size();
		}
		if(minSize > g.subGraphs[i].edges.size()){
			minSize = g.subGraphs[i].edges.size();
		}
	}
	cout << "maxSize " << maxSize << " minSize " << minSize << endl;
	return (maxSize - minSize) / 1.0 / (g.allEdges.size() / g.subGraphs.size());
}

void getVerticesAndDegree(subGraph& subg){
	subg.vertices.clear();
	subg.vertex2SubDegree.clear();
	for(int i = 0; i < subg.edges.size(); i++){
		subg.vertices.insert(subg.edges[i].src);
		subg.vertices.insert(subg.edges[i].dst);
		if(subg.vertex2SubDegree.count(subg.edges[i].src) == 0){
			subg.vertex2SubDegree[subg.edges[i].src] = 1;
		}
		else{
			subg.vertex2SubDegree[subg.edges[i].src] += 1;
		}
		if(subg.vertex2SubDegree.count(subg.edges[i].dst) == 0){
			subg.vertex2SubDegree[subg.edges[i].dst] = 1;
		}
		else{
			subg.vertex2SubDegree[subg.edges[i].dst] += 1;
		}
	}
}

void getSellEdge(Graph& g, int startDegree, int endDegree, double threshold){
	g.internalMarket.clear();
	for(int i = 0; i < g.subGraphs.size(); i++){
		int endEdges = g.subGraphs[i].edges.size() - 1;
		for(int j = 0; j <= endEdges; j++){
			int src = g.subGraphs[i].edges[j].src;
			int dst = g.subGraphs[i].edges[j].dst;
			int ver;
			if(g.vertex2AllDegree[src] < g.vertex2AllDegree[dst]){
				if(g.vertex2AllDegree[src] > 1){
					ver = src;
				}
				else{
					ver = dst;
				}
			}
			else{
				if(g.vertex2AllDegree[dst] > 1){
					ver = dst;
				}
				else{
					ver = src;
				}
			}
			if(g.vertex2AllDegree[ver] >= startDegree && g.vertex2AllDegree[ver] <= endDegree){
				if(g.subGraphs[i].vertex2SubDegree[ver] / 1.0 / g.vertex2AllDegree[ver] <= threshold){
					swap(g.subGraphs[i].edges[endEdges], g.subGraphs[i].edges[j]);
				    j--;
				    endEdges--;
				}
			}
		}
		g.subGraphs[i].money += g.subGraphs[i].edges.size() - endEdges - 1;
		// cout << i <<" "<< g.subGraphs[i].money << endl;
		// cout << i << " " << g.subGraphs[i].money << " " << g.subGraphs[i].edges.size() << " ";
		g.internalMarket.insert(g.internalMarket.end(), g.subGraphs[i].edges.begin() + endEdges + 1, g.subGraphs[i].edges.end());
		g.subGraphs[i].edges.erase(g.subGraphs[i].edges.begin() + endEdges + 1, g.subGraphs[i].edges.end());
		// cout << g.subGraphs[i].edges.size() << " ";
		getVerticesAndDegree(g.subGraphs[i]);
		// cout << g.subGraphs[i].vertices.size() << " " << g.subGraphs[i].vertex2SubDegree.size() << " " << endl;
	}
}

void arrangeInternalMarket(Graph& g){
	g.vertex2Edgesets.clear();
	g.internalMarket.insert(g.internalMarket.end(), g.leftover.begin(), g.leftover.end());
	for(int i = 0; i < g.internalMarket.size(); i++){
		int src = g.internalMarket[i].src;
		int dst = g.internalMarket[i].dst;
		int ver = g.vertex2AllDegree[src] < g.vertex2AllDegree[dst] ? src : dst;
		if(g.vertex2Edgesets.count(ver) == 0){
			EdgeSet edgeset;
			edgeset.edges.push_back(g.internalMarket[i]);
			edgeset.coreVertex = ver;
			g.vertex2Edgesets[ver] = edgeset;
		}
		else{
			g.vertex2Edgesets[ver].edges.push_back(g.internalMarket[i]);
		}
		g.vertex2Edgesets[ver].vertices.insert(src);
		g.vertex2Edgesets[ver].vertices.insert(dst);
	}
}

void buyEdges(Graph& g){
	g.leftover.clear();
	map<int, EdgeSet>::iterator iter;
	// cout << g.vertex2Edgesets.size() << endl;
	int m = 0, n = 0, k = 0;
	for(iter = g.vertex2Edgesets.begin(); iter != g.vertex2Edgesets.end(); iter++){
		bool selled = false;
		// 第一种情况，不完整小度点被购买
		for(int i = 0; i < g.subGraphs.size(); i++){
			if(g.subGraphs[i].vertices.find(iter->first) != g.subGraphs[i].vertices.end() && \
				g.subGraphs[i].money >= iter->second.edges.size()){

			
				g.subGraphs[i].edges.insert(g.subGraphs[i].edges.end(), iter->second.edges.begin(), iter->second.edges.end());
				g.subGraphs[i].money -= iter->second.edges.size();
				selled = true;
				break;
			}
		}
		// 第二种情况，完整小度点整体打包出售
		if(!selled && iter->second.edges.size() == g.vertex2AllDegree[iter->first]){
			// 选点交集最大的子图
			int part = -1;
			int maxt = 0;
			for(int i = 0; i < g.subGraphs.size(); i++){
				set<int> t;
				set_intersection(g.subGraphs[i].vertices.begin(), g.subGraphs[i].vertices.end(), \
					iter->second.vertices.begin(), iter->second.vertices.end(), inserter(t, t.begin()));
				if(t.size() > maxt && g.subGraphs[i].money >= iter->second.edges.size()){
					maxt = t.size();
					part = i;
				}
			}
			if(part >= 0 && g.subGraphs[part].money >= iter->second.edges.size()){
				g.subGraphs[part].edges.insert(g.subGraphs[part].edges.end(), iter->second.edges.begin(), iter->second.edges.end());
				g.subGraphs[part].money -= iter->second.edges.size();
				selled = true;
			}
		}
		// 第三种情况，留着下一轮卖
		if(!selled){
			for(int i = 0; i < iter->second.edges.size(); i++){
				g.leftover.push_back(iter->second.edges[i]);
			}
		}
	}
}

// 清仓，以边为单位来卖
void clearance(Graph& g){
	// 第一轮贪心分配
	// for(int i = 0; i < g.subGraphs.size(); i++){
	// 	if(g.subGraphs[i].money > 0){
	// 		for(int j = 0; j < g.leftover.size(); j++){

	// 		}
	// 	}
	// }
	// 第二轮随机分配
	for(int i = 0; i < g.subGraphs.size(); i++){
		while(g.subGraphs[i].money > 0){
			g.subGraphs[i].edges.push_back(g.leftover[g.leftover.size()-1]);
			g.subGraphs[i].money -= 1;
			g.leftover.pop_back();
		}
	}
}