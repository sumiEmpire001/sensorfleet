#include <cassert>
#include <cmath>
#include <limits>
#include "SensorFleet.h"
int main(){
 using namespace sensorfleet;
 Fleet f;
 assert(f.register_device("soil-1",20,80));
 assert(!f.register_device("soil-1",20,80));
 assert(!f.register_device("",20,80));
 assert(!f.register_device("bad",80,20));
 assert(f.ingest("soil-1",18,1).status==Status::Alert);
 assert(f.ingest("soil-1",82,2).status==Status::Alert);
 assert(f.ingest("soil-1",50,3).status==Status::Normal);
 assert(f.ingest("soil-1",50,3).status==Status::Invalid);
 assert(f.ingest("soil-1",std::numeric_limits<double>::quiet_NaN(),4).status==Status::Invalid);
 assert(f.ingest("unknown",50,4).status==Status::Invalid);
 Summary s=f.summary("soil-1");
 assert(s.valid&&s.count==3&&s.alerts==2&&fabs(s.average-50)<1e-9);
 assert(!f.summary("missing").valid);
 for(int i=0;i<7;i++){char id[10];id[0]='a'+i;id[1]=0;assert(f.register_device(id,0,100));}
 assert(!f.register_device("overflow",0,100));
}
