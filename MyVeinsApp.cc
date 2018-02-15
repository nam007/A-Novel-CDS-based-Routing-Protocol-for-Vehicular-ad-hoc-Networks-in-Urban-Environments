#include "MyVeinsApp.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include<bits/stdc++.h>
#include<map>
using namespace std;
Define_Module(MyVeinsApp);

//ofstream myfile;

static int data;

bool ids[1000]={0};
int m=0,n=0;
//bool

string ss[10];


vector<int>::iterator it;

vector<int>backbone[10];


int flag[10]={0};

double sf=INT_MAX;
int b=0;
double al = 0.5;
double bt = 0.5;
double q;
int R =250;


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
                if(myId >7)
                v[i].rid=traciVehicle->getRoadId();//bsm->getRID();
                else
                    v[i].rid = "RSU";
                v[i].sid = bsm->getSenderAddress();
                v[i].pos = bsm->getSenderSpeed();
                v[i].dist=d;
                v[i].t=simTime();

                v[i].vi = vi;
                v[i].di = s;

             /*                   //

              if((r1==v[i].rid)&&(!flag[0])){
                  ids[myId]=1;
                  cout<<" ini 1 "<<myId<<endl;
                  backbone[0].push_back(myId);
                  flag[0]=1;
              }
              else if((r2==v[i].rid)&&(!flag[1])){
                  ids[myId]=1;
                  cout<<" ini 2 "<<myId<<endl;
                  backbone[1].push_back(myId);
                  flag[1]=1;
              }
              */
                for(int l =0;l<10;l++){
                if((ss[l]==v[i].rid)&&(!flag[l])){
                                  ids[myId]=1;
                                  cout<<" ini 1 "<<myId<<endl;
                                  backbone[l].push_back(myId);
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
        if(myId !=7)
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


                        if((sf>v[i].SF)&&(!ids[v[i].id])&&(v[i].id!=7)&&(bsm->getRid()==v[i].rid)){
                            sf=v[i].SF;
                            cout<<1<<" "<<endl;
                            j=i;
                        }


                    }

        }

        if(j!=-1){

            for(int l=0;l<10;l++){
                if(ss[l]==v[j].rid){
                                 //rd[0].first=1;

                                 backbone[l].push_back(v[j].id);
                                 ids[v[j].id]=1;
                                 cout<<" a "<<v[j].id<<" "<<v[j].rid<<endl;
                                 //m++;
                                 //backbone1.push_back(v[j].id);
                            }
            }

        }


        cout<<"Backbone 1"<<endl;
        for(int i=0;i<backbone[0].size();i++)
            cout<<backbone[0][i]<<" ";
        cout<<endl;

        cout<<"Backbone 2"<<endl;
        for(int i=0;i<backbone[1].size();i++)
            cout<<backbone[1][i]<<" ";
        cout<<endl;

}

void MyVeinsApp::onWSM(WaveShortMessage* wsm) {
    //Your application has received a data message from another car or RSU
    //code for handling the message goes here, see TraciDemo11p.cc for examples
    //traciVehicle->getRoadId();
}

void MyVeinsApp::onWSA(WaveServiceAdvertisment* wsa) {
    //Your application has received a service advertisement from another car or RSU
    //code for handling the message goes here, see TraciDemo11p.cc for examples

}

void MyVeinsApp::handleSelfMsg(cMessage* msg) {
    switch (msg->getKind()) {
        case SEND_BEACON_EVT: {
            BasicSafetyMessage* bsm = new BasicSafetyMessage();
            cout<<"beacon"<<endl;
            double q=calculateQ(myId);
            sf=abs(1-q);
            //cout<<" speed "<<mobility->getCurrentSpeed();
            populateWSM(bsm);

            bsm->setID(myId);
            bsm->setX(curPosition.x);
            bsm->setY(curPosition.y);
            //bsm->setb(b);
            bsm->setSF(sf);
            if(myId !=7)
                 bsm->setRid(mobility->getRoadId().c_str());
            else
                 bsm->setRid("RSU");


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

   cout<<"handle self"<<endl;



 //   BaseWaveApplLayer::handleSelfMsg(msg);


}

void MyVeinsApp::handlePositionUpdate(cObject* obj) {
    BaseWaveApplLayer::handlePositionUpdate(obj);
    //the vehicle has moved. Code that reacts to new positions goes here.
    //member variables such as currentPosition and currentSpeed are updated in the parent class

}
