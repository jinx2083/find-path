#include "path.h"
#include <queue>
#include <stack>
using namespace std;

path::path(const PNG & im, pair<int,int> s, pair<int,int> e)
   :start(s),end(e),image(im){
    BFS();
}

void path::BFS(){
	// initialize working vectors
	vector<vector<bool>> V(image.height(), vector<bool> (image.width(),false));
	vector<vector<pair<int,int>>> P(image.height(), vector<pair<int,int>> (image.width(),end));

    buildTableHepler(P,V,start);
	pathPts = assemble(P,start,end);
}

PNG path::render(){

    PNG result(image);
    for(int i=0;i<pathPts.size();i++){
        RGBAPixel * p = result.getPixel(pathPts[i].first,pathPts[i].second);
        p->r = 255;
        p->b = 0;
        p->g = 0;
    }

    return result;

}

vector<pair<int,int>> path::getPath() { return pathPts;}

int path::length() { return pathPts.size();}

bool path::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next){


    //case1: if next is within image
    int x = next.first;
    int y = next.second;
    if ((x<0 || x>=image.width()) || (y<0 || y>=image.height())){
    return false;
    }
    else {
        //if next is unvisited
        if (v[next.second][next.first]){
            return false;
        }
        else {
            RGBAPixel currP;
            RGBAPixel nextP;
            currP = * image.getPixel(curr.first,curr.second);
            nextP = * image.getPixel(next.first,next.second);
            if (!closeEnough(currP,nextP)){
                return false;
            }
            else return true;
        }
    }

}

vector<pair<int,int>> path::neighbors(pair<int,int> curr) {
	vector<pair<int,int>> n;

    int x = curr.first;
    int y = curr.second;
    n.emplace_back(pair<int,int>(x-1,y));
    n.emplace_back(pair<int,int>(x,y+1));
    n.emplace_back(pair<int,int>(x+1,y));
    n.emplace_back(pair<int,int>(x,y-1));

    return n;
}

vector<pair<int,int>> path::assemble(vector<vector<pair<int,int>>> & p,pair<int,int> s, pair<int,int> e) {

	stack<pair<int,int>> S;


    if (p[e.second][e.first]==end){
        S.push(s);
    }
    else {
        pair<int,int> curr = e;
        while (curr!=s){
            int x = p[curr.second][curr.first].first;
            int y = p[curr.second][curr.first].second;
            S.push(curr);
            curr.first = x;
            curr.second = y;
        }
        if(S.top()!=s){
            S.push(s);
        }
    }
    int size = S.size();

    vector<pair<int,int>> output;
    while(!S.empty()){
        output.push_back(S.top());
        S.pop();
    }

    return output;

}

bool path::closeEnough(RGBAPixel p1, RGBAPixel p2){
   int dist = (p1.r-p2.r)*(p1.r-p2.r) + (p1.g-p2.g)*(p1.g-p2.g) +
               (p1.b-p2.b)*(p1.b-p2.b);

   return (dist <= 80);
}

void path::buildTableHepler(vector<vector<pair<int, int>>> &p,vector<vector<bool>> &v, pair<int, int> s) {

    queue<pair<int,int>> q;
    q.push(s);
    p[s.second][s.first].first = s.first;
    p[s.second][s.first].second = s.second;
    v[s.second][s.first] = true;


    while (!q.empty()){
        pair<int,int> curr = q.front();
        v[curr.second][curr.first] = true;
        q.pop();

        vector<pair<int,int>> neighborsCord = neighbors(curr);
        for (int i=0;i<4;i++){
            if(good(v,curr,neighborsCord[i]) ) {
                    q.push(neighborsCord[i]);
                    v[neighborsCord[i].second][neighborsCord[i].first] = true;
                    p[neighborsCord[i].second][neighborsCord[i].first].first = curr.first;
                    p[neighborsCord[i].second][neighborsCord[i].first].second = curr.second;
            }
        }
    }

}
