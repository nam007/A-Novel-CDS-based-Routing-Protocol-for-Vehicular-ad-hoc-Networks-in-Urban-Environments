#include "MyVeinsApp.h"
#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include<bits/stdc++.h>
#include<map>
using namespace std;
Define_Module(MyVeinsApp);

map<int, double> mv;
int hop[10]={0};
int h[10]={0};
static int data;
map<int, pair<double,double>> mp;
bool ids[10000]={0};
int m=0,n=0;
double dp[10]={0};
string ss[10];
Coord junc[10];
vector<int>::iterator it;

vector<int>backbone[10];
vector<int> junc_enter[10];
int bridge_node[10];
int flag[10]={0};


double sf=INT_MAX;
int b=0;
double al = 0.5;
double bt = 0.5;
double q;
int R =250;

int rsu_id[10];

int MyVeinsApp:: Find_back(int id){
    int i,j;
    bool fl=false;
    for(i=0;i<10;i++){
        for(j=0;j<backbone[i].size();j++){
            if(id == backbone[i][j])
            {
                fl=true;
                break;
            }
        }
        if(fl)
                break;
    }
    if(fl)
            return i;
    else
            return -1;
}


int MyVeinsApp:: Find_rsu(int id)
{
    for(int i=0;i<10;i++)
            if(rsu_id[i] == id)
                    return i;
    return -1;
}


bool MyVeinsApp:: return_Value(){
   int i=0;
   for(int i=0;i<10;i++)
           if(flag[i]==1)
             return true;

       return false;
}

double MyVeinsApp::calculateQ(int id){
    for(int i=0;i<v.size();i++){
        if(v[i].id == id){

            double y = sqrt(v[i].spd.x * v[i].spd.x + v[i].spd.y * v[i].spd.y);
            double f;
            if(1/3 > ((R-v[i].di )/R))
                f = 1/3;
            else
                f=((R-v[i].di )/R);
            if(!y)
                q=1;
            else
                q =al*f + bt*(v[i].vi / y);
        }
    }

    return q;
}



void MyVeinsApp::initialize(int stage) {
    ss[0]="190303315#13";
    ss[1]="-190326102#0";
    ss[2]="190326107#0";
    ss[3]="-190326102#2";
    ss[4]="190308970#29";
   // ss[5]="190308970#30";
   // ss[6]="237686044";
   // ss[7]="190308952#7";

    junc[0]=Coord(2377.78, 4678.1, 1.895);
    junc[1]=Coord(2364.59, 4762.59, 1.895);
    junc[2]=Coord(2700.85, 4825.08, 0.0);
    junc[3]=Coord(2533.1, 4795.33, 0.0);

    junc[4]=Coord(2319.03, 4965.07, 1.895);

//    junc[5]=Coord(2024.09, 3945.45, 0.0);
 //   junc[6]=Coord(1876.24, 3867.13, 0.0);
  //  junc[7]=Coord(1859.78, 3597.17, 0.0);

    rsu_id[0]=7;
    rsu_id[1]=8;
    rsu_id[2]=9;
    rsu_id[3]=10;
    rsu_id[4]=11;


      p.id=myId;
      p.x=curPosition.x;
      p.y=curPosition.y;



    BaseWaveApplLayer::initialize(stage);
    if (stage == 0) {
        //Initializing members and pointers of your application goes here
        EV << "Initializing " << par("appName").stringValue() << std::endl;
       // MyId =49;
    }
    else if (stage == 1) {
        //Initializing members that require initialized other modules goes here
    }
}

void MyVeinsApp::finish() {
    for(int i=0;i<5;i++)
                   cout<<"delay "<<i<<" "<<dp[i]<<endl;
    BaseWaveApplLayer::finish();
}

void MyVeinsApp::onBSM(BasicSafetyMessage* bsm) {


        double d=traci->getDistance(bsm->getSenderPos(),curPosition, 0);
       bool taken =true;
       double s;
       double vi;
       int z=0;

       for(int i=0;i<v.size();i++){
        if(v[i].id == myId){
                  s += v[i].dist;
                  double g= sqrt(v[i].spd.x * v[i].spd.x + v[i].spd.y * v[i].spd.y );
                   vi +=g;
                   z++;
                  }
             }
       if(z!=0){
          s = s/z;
          vi = vi/z;
       }
       else
       {
           s=0;
           vi=0;
       }
        for(int i=0;i!=v.size();i++){
            if(v[i].sid == bsm->getSenderAddress() && myId == v[i].id)
            {
              //  myfile<<"id \t"<<v[i].id<<" \t"<<"sender\t"<<v[i].sid<<"distance from rsu\t"<<d<<endl;
                taken =false;
                v[i].id=myId;
                v[i].x=curPosition.x;
                v[i].y=curPosition.y;

                //double q=calculateQ(myId);
                v[i].SF=bsm->getSF();
                if(myId !=7 && myId!=8 && myId!=9 && myId!=10 && myId!=11)
                v[i].rid=traciVehicle->getRoadId();//bsm->getRID();
                else
                    v[i].rid = "RSU";
                v[i].sid = bsm->getSenderAddress();
                v[i].pos = bsm->getSenderSpeed();
                v[i].dist=d;
                v[i].t=simTime();

                v[i].vi = vi;
                v[i].di = s;


                for(int l =0;l<5;l++){
                if((ss[l]==v[i].rid)&&(!flag[l]) && ids[myId]==0){
                                  ids[myId]=1;
                                //  cout<<" ini 1 "<<myId<<endl;
                                  backbone[l].push_back(myId);
                                  h[l]++;

                                  flag[l]=1;
                              }


                }
            }
        }


        if(taken){
      //      myfile<<"id \t"<<myId<<" \t"<<"sender\t"<<bsm->getSenderAddress()<<"distance from rsu\t"<<d<<endl;
        neighbour n;
        n.id = myId;
        n.sid = bsm->getSenderAddress();
        n.pos = bsm->getSenderPos();
        n.spd = bsm->getSenderSpeed();

        //double q=calculateQ(myId);
        n.SF=bsm->getSF();//bsm->getSF();
        if(myId !=7 && myId!=8 && myId!=9 && myId!=10 && myId!=11)
                        n.rid=traciVehicle->getRoadId();//bsm->getRID();
                        else
         n.rid = "RSU";
        n.dist=d;
        n.t=simTime();
        n.vi = vi;
        n.di = s;
        v.push_back(n);

        }
        long long int sf=INT_MAX;
        int j=-1;
        if(return_Value())//flag1||flag2){
        {
            for(int i=0;i<v.size();i++){
                        simtime_t a=simTime()-v[i].t;
                        if(a>10){
                            v.erase(v.begin()+i);



                       //     cout<<myId<<" Erased "<<v[i].t<<" is time "<<v[i].sid<<" sender id "<<a<<" diff "<<endl;
                        }


                        if((sf>v[i].SF)&&(!ids[v[i].id])&&(myId !=7 && myId!=8 && myId!=9 && myId!=10 && myId!=11)&&(bsm->getRid()==v[i].rid)){
                            sf=v[i].SF;

                            j=i;
                        }


                    }

        }

         //   BACKBONE CREATION

        if(j!=-1){

            for(int l=0;l<5;l++){
                if(ss[l]==v[j].rid){
                                 //rd[0].first=1;

                                 backbone[l].push_back(v[j].id);
                                 h[l]++;

                                 ids[v[j].id]=1;
                      //           cout<<" a "<<v[j].id<<" "<<v[j].rid<<endl;
                                 //m++;
                                 //backbone1.push_back(v[j].id);
                            }
            }

        }

  /*        for(int i=0;i<=4;i++){
               cout<<"Backbone "<<i<<"is"<<endl;
               for(int j=0;j<backbone[i].size();j++)
               {
                   cout<<backbone[i][j]<<" ";
               }
               cout<<endl;
           } */


           int is,ij;
           bool flag=false;
           for(int i=0;i<=4;i++){
               for(int j=0;j<backbone[i].size();j++){
                   if(backbone[i][j] == myId)
                   {
                       is=i;
                       ij=j;
                       flag=true;
                       break;
                   }
               }
           }
           if(flag){
               string sg;
               if(myId ==7 || myId==8 || myId==9 || myId==10 || myId==11)
                  sg="7";
               else
                  sg = traciVehicle->getRoadId();
               if(sg != ss[is])
               {
                   backbone[is].erase(backbone[is].begin() + ij);

                   ids[myId]=0;
        //           cout<<myId<<"erased";
               }
           }


           // BRIDGE NODE CREATION




           map<int,pair<double,double>> :: iterator it;
                  for(it=mp.begin();it!=mp.end();it++){
                      pair<double,double> ps;
                      int idd = it->first;
                      ps = it->second;
                      if(ps.first > ps.second && ps.second<=5 ){
                              int iu = Find_back(idd);
                                 junc_enter[iu].push_back(idd);
                            //     cout<<"current position is"<<curPosition<<endl;
                      }


                  }
                  int iid;
                  bool fl=false;
                  for(int i=0;i<5;i++){
                         //        cout<<"junction entering is "<<i<<"is"<<endl;
                      double mn=INT_MAX;

                          fl=false;
                                 for(int j=0;j<junc_enter[i].size();j++)
                                 {
                                     if(mv[junc_enter[i][j]]< mn){
                                         mn = mv[junc_enter[i][j]];
                                         iid =junc_enter[i][j];
                                         fl=true;
                                     }
                               //     cout<<"junc_enter"<<junc_enter[i][j]<<endl;
                                 }
                                 if(fl){
                                      if(hop[i]==0)
                                          hop[i] = h[i];
                                     bridge_node[i]=iid;

                                     WaveShortMessage* wsm = new WaveShortMessage();
                                     populateWSM(wsm);

                                     wsm->setRecipientAddress(rsu_id[i]);
                                     wsm->setName("RAP");
                                     wsm->setSenderAddress(iid);
                                     wsm->setWsmData("RAP");
                                     wsm->setWSMRid(ss[i]);
                                     wsm->setFl(0);
                                     sendDown(wsm);

                        //         cout<<"bridge node for "<<i<<"th road is "<<iid<<endl;
                                 }
                             }

}

void MyVeinsApp::onWSM(WaveShortMessage* wsm) {
    int id = Find_back(myId);
    if(wsm->getWSMRid()==traciVehicle->getRoadId()){
        cout<<"fl is "<<wsm->getFl()<<endl;
        if(wsm->getFl() == 0){

            sendDown(wsm->dup());
        }


      //  cout<<"my id  "<<myId<<" wsm info is "<<wsm->getWsmData()<<" road "<<wsm->getWSMRid()<<" recipeint id "<<wsm->getRecipientAddress()<<endl;
    }



}

void MyVeinsApp::onWSA(WaveServiceAdvertisment* wsa) {
    //Your application has received a service advertisement from another car or RSU
    //code for handling the message goes here, see TraciDemo11p.cc for examples

}

void MyVeinsApp::handleSelfMsg(cMessage* msg) {

    switch (msg->getKind()) {
        case SEND_BEACON_EVT: {
            BasicSafetyMessage* bsm = new BasicSafetyMessage();
          //  cout<<"beacon"<<endl;
            double q=calculateQ(myId);
            sf=abs(1-q);
            //cout<<" speed "<<mobility->getCurrentSpeed();
            populateWSM(bsm);

            bsm->setID(myId);
            bsm->setX(curPosition.x);
            bsm->setY(curPosition.y);
            //bsm->setb(b);
            bsm->setSF(sf);
            if(myId !=7 && myId!=8 && myId!=9 && myId!=10 && myId!=11)
                 bsm->setRid(mobility->getRoadId().c_str());
            else
                 bsm->setRid("RSU");
         //   cout<<"handle self "<<myId<<" "<<simTime()<<endl;

            sendDown(bsm);
            scheduleAt(simTime() + beaconInterval, sendBeaconEvt);
            break;
        }
        case SEND_WSA_EVT:   {
            WaveServiceAdvertisment* wsa = new WaveServiceAdvertisment();
            populateWSM(wsa);
            sendDown(wsa);
            scheduleAt(simTime() + wsaInterval, sendWSAEvt);
            break;
        }
        default: {
            if (msg)
                DBG_APP << "APP: Error: Got Self Message of unknown kind! Name: " << msg->getName() << endl;
            break;
        }
        }
 //   BaseWaveApplLayer::handleSelfMsg(msg);


}

void MyVeinsApp::handlePositionUpdate(cObject* obj) {
  //  cout<<"Handle position "<<myId<<endl;
    BaseWaveApplLayer::handlePositionUpdate(obj);
    double xv=curSpeed.x;
    double yv=curSpeed.y;
    double xy = sqrt(xv*xv + yv*yv);
    mv[myId]=xy;
    bool fl=0;
    int i,j;
    for( i=0;i<5;i++){
        for( j=0;j<backbone[i].size();i++){
            if(myId == backbone[i][j])
            {
                fl=1;
                break;
            }
        }
        if(fl)
               break;
    }
    if(fl){
    Coord prev;
    prev.x=p.x;
    prev.y=p.y;

    double dis = traci->getDistance(junc[i],prev,0);
    pair<double,double> pt;
    pt.first=dis;
    double ds;
    ds = sqrt(pow((junc[i].x - prev.x),2) + pow((junc[i].y - prev.y),2) );

    p.id=myId;
    p.x= curPosition.x;
    p.y=curPosition.y;
    double dis2 = traci->getDistance(junc[i],curPosition,0);

    pt.second = dis2;
    mp[myId]=pt;

    }
 //   cout<<"Initialize "<<myId<<" is"<<curPosition<<endl;
    //member variables such as currentPosition and currentSpeed are updated in the parent class

}
