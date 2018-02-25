

#include "veins/modules/application/traci/TraCIDemoRSU11p.h"



Define_Module(TraCIDemoRSU11p);

void TraCIDemoRSU11p::onWSA(WaveServiceAdvertisment* wsa) {
    //if this RSU receives a WSA for service 42, it will tune to the chan
    if (wsa->getPsid() == 42) {
        mac->changeServiceChannel(wsa->getTargetChannel());
    }
}

void TraCIDemoRSU11p::onWSM(WaveShortMessage* wsm) {

    //this rsu repeats the received traffic update in 2 seconds plus some random delay
    if(myId ==wsm->getRecipientAddress() && wsm->getFl()==false){
        wsm->setFl(true);
        int l = wsm->getHop();

        simtime_t t = simTime();
        t = t - wsm->getCreationTime();
        cout<<"rsu creation "<<wsm->getCreationTime()<<" "<<t<<endl;
     //   cout<<"my id"<<myId<<"wsm info is "<<wsm->getWsmData()<<"sender "<<wsm->getSenderAddress()<<" road "<<wsm->getWSMRid()<<" recipeint id "<<wsm->getRecipientAddress()<<endl;
        wsm->setDelay(t);
        wsm->setHop(l+1);
        wsm->setSenderAddress(myId);
     //   cout<<"delay is "<<t<<" id id "<<myId<<endl;
        //BasicSafetyMessage* bsm = new BasicSafetyMessage();
        sendDown(wsm->dup());


    }
   // sendDelayedDown(wsm->dup(), 2 + u niform(0 .01,0.2));
}
