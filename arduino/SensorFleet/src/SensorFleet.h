#ifndef LORD_SUMI_SENSOR_FLEET_H
#define LORD_SUMI_SENSOR_FLEET_H
#include <math.h>
#include <stdint.h>
#include <string.h>
namespace sensorfleet {
enum class Status { Invalid, Normal, Alert };
struct Result {Status status;double value;Result(Status s,double v):status(s),value(v){}};
struct Summary {
 bool valid;uint32_t count,alerts;double average;
 Summary(bool v,uint32_t n,uint32_t a,double avg):
 valid(v),count(n),alerts(a),average(avg){}
};
class Fleet {
 public:
 static const int MAX_DEVICES=8;
 static const int ID_LENGTH=16;
 Fleet():used_(0){}
 bool register_device(const char* id,double minimum,double maximum){
  if(!id||!id[0]||strlen(id)>=ID_LENGTH||!isfinite(minimum)||!isfinite(maximum)||
     minimum>=maximum||used_>=MAX_DEVICES||find(id)>=0)return false;
  Device& d=devices_[used_++];
  strncpy(d.id,id,ID_LENGTH);d.id[ID_LENGTH-1]='\0';
  d.minimum=minimum;d.maximum=maximum;d.count=0;d.alerts=0;d.sum=0;
  d.last_timestamp=0;d.has_last=false;
  return true;
 }
 Result ingest(const char* id,double value,uint32_t timestamp){
  int index=find(id);
  if(index<0||!isfinite(value))return Result(Status::Invalid,value);
  Device& d=devices_[index];
  if((d.has_last&&timestamp<=d.last_timestamp)||d.count==UINT32_MAX||
     !isfinite(d.sum+value))return Result(Status::Invalid,value);
  Status status=value<d.minimum||value>d.maximum?Status::Alert:Status::Normal;
  d.count++;
  if(status==Status::Alert)d.alerts++;
  d.sum+=value;
  d.last_timestamp=timestamp;d.has_last=true;
  return Result(status,value);
 }
 Summary summary(const char* id)const{
  int index=find(id);
  if(index<0)return Summary(false,0,0,0);
  const Device& d=devices_[index];
  return Summary(true,d.count,d.alerts,d.count?d.sum/d.count:0);
 }
 private:
 struct Device {
  char id[ID_LENGTH];double minimum,maximum,sum;
  uint32_t count,alerts,last_timestamp;bool has_last;
 };
 Device devices_[MAX_DEVICES];int used_;
 int find(const char* id)const{
  if(!id)return -1;
  for(int i=0;i<used_;i++)if(strcmp(devices_[i].id,id)==0)return i;
  return -1;
 }
};
}
#endif
